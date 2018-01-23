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

class AigNode
        {
        
public: 
        //virtual ~nodoaig()=0;
        //virtual void xxx()=0;
        };

class AndNode:public AigNode{
    int i0;
    int i1;
    int saida;

public:
    AndNode(int, int, int);
    //void imprime(ostream& saida);
    friend class Aig;  //sera que precisa?
};


class InputNode:public AigNode{
    int saida;
public:
    InputNode(int);
    //void imprime(ostream& saida);
    friend class Aig;  //sera que precisa?
};

class OutputNode:public AigNode{
    int i0;

public:
    OutputNode(int);
    //void imprime(ostream& saida);
    friend class Aig;  //sera que precisa?
};



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
    string sat;

};
#endif