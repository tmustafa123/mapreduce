#include <fstream>
#include <string>
#include <vector>
#include<list>

class Parser
{
public:
	Parser();
	int file_split(std::string fileName){
		int i = 0;
		std::ifstream in(fileName.c_str());
		std::string str;
		while (std::getline(in, str)){
			if (str.size() > 0) {
				listOfStrs.push_back(str);
				i++;
			}
		}

		in.close();
		return i;
	}

	std::vector<std::string> getPartition(int start, int end) {
		std::vector<std::string> line;
		line.assign(std::next(listOfStrs.begin(), start), std::next(listOfStrs.begin(), end));
		return line;
	}

	~Parser();


private:
	std::list<std::string> listOfStrs;
	

};

Parser::Parser()
{
}

Parser::~Parser()
{
}
