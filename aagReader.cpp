#ifndef AAG_H
#define AAG_H

#include "aagReader.h"
#include "gerentebdd.h"

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


AAGReader::AAGReader(string sourcePath)
{
    source.open(sourcePath.c_str());
    debug.open("aagComentado.txt");
}

Aig* AAGReader::readFile()
{

    Aig* aig = new Aig();
    gerentebdd g;
    //treating header
    source.getline(buf, 250, '\n');
    string s=buf;
    line.str(s);
    line >> word;

    if(strcmp("aag",word.c_str())!=0)
    {
        return NULL;
    }

    int nNodes, nInputs, nFFs, nOutputs, nAnds;
    line >> word;
    nNodes = atoi(word.c_str());
    line >> word;
    nInputs = atoi(word.c_str());
    line >> word;
    nFFs = atoi(word.c_str());
    line >> word;
    nOutputs = atoi(word.c_str());
    line >> word;
    nAnds = atoi(word.c_str());

    if (nNodes != nInputs + nFFs + nAnds) {
        return NULL;
    }

    if (nFFs != 0) {
        return NULL;
    }

    debug << s << "\nThe file header is ok!\n\n";

    AigNode nodes[nNodes + 1];
    AigNode outs[nOutputs];
    AigNode ins[nInputs];

    aig->ni = nInputs;
    aig->no = nOutputs;
    aig->na = nAnds;

        
    std::vector<int> left_vec(nNodes);
    std::vector<int> right_vec(nNodes);
    std::vector<int> outs_vec(nOutputs);
    std::vector<int> ins_vec(nInputs);

     for(int clear = 0; clear <= nNodes; clear++){
        left_vec.push_back(0);
        right_vec.push_back(0);
        aig->left.push_back(0);
        aig->right.push_back(0);
    }

    for(int clear = 0; clear <= nOutputs; clear++){
        aig->outs.push_back(0);
        outs_vec.push_back(0);
    }

    for(int clear = 0; clear <= nInputs; clear++){
        aig->ins.push_back(0);
        ins_vec.push_back(0);
    }
    //treating inputs
    for (int i = 0; i < nInputs; i++) {
        debug << "read the input" << i << " from the file\n";
        debug << "   create in" << i << " and add it to an input list and the all nodes list\n";
        source.getline(buf, 250, '\n');
        string s=buf;
        line.str(s);
        line >> word;

        std::vector<std::string> nodes = split(s, ' ');
        ins_vec.at(i) = (atoi(nodes.at(0).c_str()));
    }

    //treating outputs
    debug << "\n";
    for (int i = 0; i < nOutputs; i++) {
        debug << "read the output" << i << " from the file\n";
        debug << "   create out" << i << " and add it to an output list and the all nodes list\n";
        source.getline(buf, 250, '\n');
        string s=buf;
        line.str(s);
        line >> word;
       //cout << "##" << s  << "##" << "\n";
        std::vector<std::string> nodes = split(s, ' ');
        outs_vec.at(i) = (atoi(nodes.at(0).c_str()));
    }

    //connecting ands
    debug << "\n";

     stringstream sat;
    for (int i = 0; i < nAnds; i++) {
        debug << "read the and" << i << " output and inputs\n";
        debug << "   connect the and" << i << " and set the inversion of this pins\n";
        source.getline(buf, 250, '\n');
        string s=buf;
        line.str(s);
        line >> word;
        std::vector<std::string> nodes = split(s, ' ');

        stringstream varA;
        stringstream varA_neg;

        stringstream varB;
        stringstream varB_neg;

        stringstream varC;
        stringstream varC_neg;


        if(atoi(nodes.at(1).c_str()) % 2 == 0){
            varA << "!var" << (atoi(nodes.at(1).c_str()) / 2);
            varA_neg << "var" << (atoi(nodes.at(1).c_str()) / 2);

        }else{
            varA << "var" << (atoi(nodes.at(1).c_str()) / 2);
            varA_neg << "!var" << (atoi(nodes.at(1).c_str()) / 2);
        }

        if(atoi(nodes.at(2).c_str()) % 2 == 0){
            varB << "!var" << (atoi(nodes.at(2).c_str()) / 2);
            varB_neg << "var" << (atoi(nodes.at(2).c_str()) / 2);

        }else{
            varB << "var" << (atoi(nodes.at(2).c_str()) / 2);
            varB_neg << "!var" << (atoi(nodes.at(2).c_str()) / 2);

        }

        if(atoi(nodes.at(0).c_str()) % 2 == 0){
            varC << "var" << (atoi(nodes.at(0).c_str()) / 2);
            varC_neg << "!var" << (atoi(nodes.at(0).c_str()) / 2);

        }else{
            varC << "!var" << (atoi(nodes.at(0).c_str()) / 2);
            varC_neg << "var" << (atoi(nodes.at(0).c_str()) / 2);
        }
        sat << "(" << varA.str() << "+" << varB.str() << "+" << varC.str() << ")" << "(" << varA_neg.str() << "+"<< varC_neg.str() << ")" << "("<< varB_neg.str() << "+"<< varC_neg.str() <<")";


        left_vec.at(atoi(nodes.at(0).c_str())/2) = atoi(nodes.at(1).c_str());
        right_vec.at(atoi(nodes.at(0).c_str())/2) = atoi(nodes.at(2).c_str());
        
    }

  
 
    int i=0;
    debug << "\n";
    while(source)
    {
        source.getline(buf, 250, '\n');
        s=buf;
        line.seekg(0);line.str(s);
        line >> word;

        if(strcmp("c",word.substr(0).c_str())==0){
            debug << "the comments began. Ignore the file from here!\n";
            break;
        } else if(strcmp(word.substr(0).c_str(),"i")==0){

        } else if(strcmp(word.substr(0).c_str(),"o")==0){

        } else if(strcmp(word.substr(0).c_str(),"l")==0){

        }
    }


    aig->left = left_vec;
    aig->right = right_vec;
    aig->outs = outs_vec;
    aig->ins = ins_vec;
    aig->sat = sat.str();

    return aig;
}
#endif