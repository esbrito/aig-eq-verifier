#ifndef AIG_H
#define AIG_H


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>

using namespace std;


class Aig{
public:
      int ni;
      int no; 
      int na;
    void print_aig(Aig* aig);
    std::vector<int> left;
    std::vector<int> right;
    std::vector<int> outs;
    std::vector<int> ins;
};
#endif