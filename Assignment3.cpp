/* SPM-18 Assignment 3 Tajammul Mustafa*/
#include<string>
#include <math.h>  
#include<iostream>
#include<vector>
#include<thread>
#include<sstream>
#include<map>
#include<atomic>
#include"parser.h"
#include"reduce.h"

using namespace std;

const float A = 0.5*(sqrt(5) - 1);
Parser task;

std::string output;
std::atomic_int total(0);
std::atomic_int synchronize(0);

typedef struct {
	int start;
	int end;
} RANGE;

typedef struct {
	std::map<std::string, int> map;

}REDUCE;

Reduce rd;
void mapper(Parser& tq, Reduce *worker_internal, int nw, RANGE range);
void reduce(int nw, int id, Reduce& rd);
int hash_(std::string str, int m);

int main(int argc, char * argv[]) {
	if (argc == 1) {
		std::cout << "Usage is: " << argv[0] << "  Input Output workers delay " << std::endl;
		return(0);
	}

	string input = (argv[1]);
	output = (argv[2]);
	int nw = atoi(argv[3]);
	
	

	auto t1 = std::chrono::high_resolution_clock::now();
	std::vector<std::thread> t1_;
	std::vector<std::thread> tid_;

	/*Create an object for each worker*/
	Reduce *worker_ds = new Reduce[nw];
	vector<RANGE> ranges(nw);
	
	
	int m = task.file_split(input);
	int delta{ m / nw };
	for (int i = 0; i<nw; i++) {                     
		ranges[i].start = i * delta;
		ranges[i].end = (i != (nw - 1) ? (i + 1)*delta : m);
	}

    /* Mapper phase */
	for (int id = 0; id<nw; id++) {
		tid_.push_back(std::thread(mapper, std::ref(task), worker_ds, nw, ranges[id]));

	}
	for (int i = 0; i<nw; i++)
		tid_[i].join();

	/* Reduce phase */
	for (int id = 0; id<nw; id++) {
		t1_.push_back(std::thread(reduce, nw, id, std::ref(worker_ds[id])));

	}
	for (int i = 0; i<nw; i++)
		t1_[i].join();

	
	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = t2 - t1;
	rd.compute(output); 
	std::cout << "\nTOTAL TIME ELASPED :  " << elapsed.count() << "  Seconds" << std::endl;
	
	return 0;
}

void mapper(Parser& tq, Reduce *worker_ds, int nw,  RANGE range)
{
	/* Use an independent map for each worker*/
	std::map<std::string, int> localMap;
	vector<REDUCE> local(nw);
	string token;

	/* Keep a local copy */
	std::vector<std::string> partition;

	/* work on chunks rather than on lines */
	partition = tq.getPartition(range.start, range.end);
	
	int t = 0;
	
	for (auto i : partition) {
		t++;
		total++;
		stringstream ss(i);
		while (getline(ss, token, ' ')) {
			int dest = hash_(token, nw);
			if (local[dest].map.find(token) != local[dest].map.end()) {
				
				local[dest].map[token]++;
			}
			else {
				local[dest].map[token] = 1;	
			}
		}
	}
	
	int x = 0;
	for (auto i : local) {
		while (true) {   
			bool check = rd.shuffle(worker_ds,i.map, x);
			if (check) {
				break;
			}
		} 
		x++;
	}
	std::cout << "\n Tasks Recived " << t << std::endl;	
}


void reduce(int nw, int id, Reduce& rd)
{
	std::map<std::string, int> localMap;
	for (auto const& item : rd.pairs) {
		if (localMap.count(item.Key) > 0) {
			localMap[item.Key] += item.Val;
			
		}
		else
			localMap[item.Key] = item.Val;
	}
	std::string str;
	for (std::map<std::string, int>::iterator it = localMap.begin(); it != localMap.end(); ++it) {
		str += it->first + " => " + std::to_string(it->second) + "\n";
	}
	
	while (true) {
		if (rd.join(str)) {
			break;
		}
	} 
}

int hash_(std::string str, int nw)
{
	
	int l = str.size();
	int i = (int)str[0];
	int k = 31 * l + 7 * i*i;
	float kA = k * A;
	return floor(nw*(kA - floor(kA)));
}



