#pragma once
#include<mutex>
#include<map>

std::string strOne;
std::string *wString = &strOne;

typedef struct
{
	std::string Key;
	int Val;
} KEYPAIR;

class Reduce
{
public:
	Reduce();
	~Reduce();
	std::vector<KEYPAIR> pairs;
	
	void shuffle(Reduce *reduce,std::map<std::string, int> map ,int dest ) {
		KEYPAIR y;
		std::unique_lock<std::mutex> lock(mtx);
		for (std::map<std::string, int>::iterator it = map.begin(); it != map.end(); ++it) {
			y.Key = it->first;
			y.Val= it->second;
			reduce[dest].pairs.push_back(y);	
		}
		return ;
	}

	void compute(std::string filename) {
		std::ofstream myfile;
		myfile.open(filename, std::ios_base::app);
		//std::cout << *wString;
		myfile << *wString;
		myfile.close();
	}
	bool join(std::string wstr) {
		std::unique_lock<std::mutex> lock(mtx_file);
		*wString += wstr; 
		return true;
	}
	
private:

	std::mutex mtx;
	std::mutex mtx_file;

};

Reduce::Reduce()
{
}

Reduce::~Reduce()
{
}


#pragma once
