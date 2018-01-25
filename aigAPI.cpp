
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include "aig.h"
#include "aigAPI.h"

vector<int> hasBeenGenerated;

void AigAux::print_aig(Aig *aig)
{
    for (int indexNode = 1; indexNode <= (aig->ni + aig->na); indexNode++)
    {
        if (aig->left.at(indexNode) == 0 && aig->right.at(indexNode) == 0)
        {
        }
        else
        {
            cout << "Nodo: " << indexNode;
            if (aig->left.at(indexNode) % 2 == 0)
            {
                cout << " " << aig->left.at(indexNode) / 2;
            }
            else
            {
                cout << " !" << aig->left.at(indexNode) / 2;
            }
            cout << " AND ";
            if (aig->right.at(indexNode) % 2 == 0)
            {
                cout << " " << aig->right.at(indexNode) / 2;
            }
            else
            {
                cout << " !" << aig->right.at(indexNode) / 2;
            }
            cout << '\n';
        }
    }

    for (int indexIn = 0; indexIn < aig->ni; indexIn++)
    {
        if (aig->ins.at(indexIn) % 2 == 0)
        {
            cout << aig->ins.at(indexIn) / 2 << " é entrada\n";
        }
        else
        {
            cout << "!" << aig->ins.at(indexIn) / 2 << " é entrada\n";
        }
    }

    for (int indexOut = 0; indexOut < aig->no; indexOut++)
    {
        if (aig->outs.at(indexOut) % 2 == 0)
        {
            cout << aig->outs.at(indexOut) / 2 << " é saída\n";
        }
        else
        {
            cout << "!" << aig->outs.at(indexOut) / 2 << " é saída\n";
        }
    }
}

string getAnd(std::string andString, Aig *aig, int outNode)
{
    //cout << andString << "\n";
    int positiveOutNode = -1;
    if (outNode % 2 != 0)
    {
        positiveOutNode = outNode - 1;
    }

    if (!(std::find(aig->ins.begin(), aig->ins.end(), outNode) != aig->ins.end()) &&
        !(std::find(aig->ins.begin(), aig->ins.end(), positiveOutNode) != aig->ins.end()))
    {
        //cout << "recursing left("<< outNode <<")\n";
        if (outNode % 2 != 0)
        {
            andString.append("!");
        }
        andString.append("(");

        andString = getAnd(andString, aig, aig->left.at(outNode / 2));
        andString.append(" * ");
        // cout << "recursing right("<< outNode <<")\n";
        andString = getAnd(andString, aig, aig->right.at(outNode / 2));
        andString.append(")");
        return andString;
    }
    else
    {
        // cout << "input("<< outNode <<")\n";
        int inputValue = 0;
        for (int indexIn = 0; indexIn < aig->ni; indexIn++)
        {
            // cout << "****" << aig->ins.at(indexIn) << "----" << outNode/2 << "****" ;
            if (aig->ins.at(indexIn) == outNode && outNode % 2 == 0)
            {
                inputValue = aig->ins.at(indexIn) / 2;
                break;
            }
            if (aig->ins.at(indexIn) == outNode && outNode % 2 != 0)
            {
                inputValue = aig->ins.at(indexIn) / 2;
                break;
            }
            if (aig->ins.at(indexIn) == outNode + 1 && outNode % 2 == 0)
            {
                inputValue = aig->ins.at(indexIn) / 2;
                break;
            }
            if (aig->ins.at(indexIn) == outNode - 1 && outNode % 2 != 0)
            {
                inputValue = aig->ins.at(indexIn) / 2;
                break;
            }
        }
        // cout << "in value "  << inputValue << "\n";
        if (outNode % 2 == 0)
        {
            andString.append(" var" + std::to_string(inputValue));
            // cout << "String: " << andString << '\n';
            return andString;
        }
        else
        {
            andString.append(" !var" + std::to_string(inputValue));
            //  cout << "String: " << andString << '\n';
            return andString;
        }
    }
}
std::vector<string> AigAux::create_equations_from_aig(Aig *aig)
{
    std::vector<string> equations;
    for (int indexOut = 0; indexOut < aig->no; indexOut++)
    {
        int outNode = aig->outs.at(indexOut);
        equations.push_back(getAnd("", aig, outNode));
    }

    return equations;
}


string getSat(std::string satString, Aig *aig, int outNode, string aigNumber = "")
{


        if(aig->left.at(outNode / 2) == 0 && aig->right.at(outNode / 2) == 0) {
            return satString;
        }
      
        stringstream varA;
        stringstream varA_neg;

        stringstream varB;
        stringstream varB_neg;

        stringstream varC;
        stringstream varC_neg;


        if(aig->left.at(outNode / 2) % 2 == 0){
            varA << "!var" << (aig->left.at(outNode / 2)/2);
            varA_neg << "var" << (aig->left.at(outNode / 2)/2);

        }else{
            varA << "var" << (aig->left.at(outNode / 2)/2);
            varA_neg << "!var" << (aig->left.at(outNode / 2)/2);
        }

        if(aig->right.at(outNode / 2) % 2 == 0){
            varB << "!var" << (aig->right.at(outNode / 2) /2);
            varB_neg << "var" << (aig->right.at(outNode/ 2) /2);

        }else{
            varB << "var" << (aig->right.at(outNode/2) /2 );
            varB_neg << "!var" << (aig->right.at(outNode/2)/2 );

        }

        varC << "var" << (outNode / 2) << aigNumber;
        varC_neg << "!var" << (outNode / 2) << aigNumber;
       
        stringstream ss;
        ss << "(" << varA.str() << "+" << varB.str() << "+" << varC.str() << ")" << "(" << varA_neg.str() << "+"<< varC_neg.str() << ")" << "("<< varB_neg.str() << "+"<< varC_neg.str() <<")";


        if(!(satString.find(ss.str()) != string::npos)){
             satString.append(ss.str());
        }
        
        string leftString = getSat(satString, aig, aig->left.at(outNode / 2));
        

        string rightString = getSat(leftString, aig, aig->right.at(outNode / 2));
        

        return rightString;
}

std::vector<string> AigAux::create_sats_eq_from_aig(Aig *aig, Aig *aig2)
{
    std::vector<string> sats;
    sats.clear();
    for (int indexOut = 0; indexOut < aig->no; indexOut++)
    {
        int outNode = aig->outs.at(indexOut);

        stringstream xorCompare;
        xorCompare << "(" << "!var" << aig->outs.at(indexOut) / 2 << "aig1" << "+" <<  "!var" << aig->outs.at(indexOut) / 2 << "aig2" << "+" << "!dif" <<")" 
        << "(" << "!var" << aig->outs.at(indexOut) / 2 << "aig1" << "+" <<  "var" << aig->outs.at(indexOut) / 2 << "aig2" << "+" << "dif" <<")"
        << "(" << "var" << aig->outs.at(indexOut) / 2 << "aig1" << "+" <<  "!var" << aig->outs.at(indexOut) / 2 << "aig2" << "+" << "dif" <<")"
        << "(" << "var" << aig->outs.at(indexOut) / 2 << "aig1" << "+" <<  "var" << aig->outs.at(indexOut) / 2 << "aig2" << "+" << "!dif" <<")"
        << "(dif)";
        sats.push_back(getSat("", aig, outNode, "aig1")
        .append(getSat("", aig2, outNode, "aig2")
        .append(xorCompare.str())));
    }
    return sats;
}


string getSatCnf(std::string satString, Aig *aig, int outNode, int aigNumber = 0)
{


        if(aig->left.at(outNode / 2) == 0 && aig->right.at(outNode / 2) == 0) {
            return satString;
        }
      
        stringstream varA;
        stringstream varA_neg;

        stringstream varB;
        stringstream varB_neg;

        stringstream varC;
        stringstream varC_neg;


        if(aig->left.at(outNode / 2) % 2 == 0){
            varA << " -" << (aig->left.at(outNode / 2)/2);
            varA_neg << " " << (aig->left.at(outNode / 2)/2);

        }else{
            varA << " " << (aig->left.at(outNode / 2)/2);
            varA_neg << " -" << (aig->left.at(outNode / 2)/2);
        }

        if(aig->right.at(outNode / 2) % 2 == 0){
            varB << " -" << (aig->right.at(outNode / 2) /2);
            varB_neg << " " << (aig->right.at(outNode/ 2) /2);

        }else{
            varB << " " << (aig->right.at(outNode/2) /2 );
            varB_neg << " -" << (aig->right.at(outNode/2)/2 );

        }

        varC << " " << (outNode / 2) + aigNumber;
        varC_neg << " -" << (outNode / 2) + aigNumber;
       
        stringstream ss;
        ss  << varA.str() << " " << varB.str() << varC.str() << " 0\n"  << varA_neg.str() << " "<< varC_neg.str() << " 0\n"
         << varB_neg.str() << " "<< varC_neg.str() <<" 0\n";


        if(!(satString.find(ss.str()) != string::npos)){
             satString.append(ss.str());
        }
        
        string leftString = getSatCnf(satString, aig, aig->left.at(outNode / 2));
        

        string rightString = getSatCnf(leftString, aig, aig->right.at(outNode / 2));
        

        return rightString;
}

std::vector<string> AigAux::create_sats_cnf_from_aig(Aig *aig, Aig *aig2)
{
    std::vector<string> sats;
    sats.clear();
    for (int indexOut = 0; indexOut < aig->no; indexOut++)
    {
        int outNode = aig->outs.at(indexOut);

        stringstream header;
        stringstream xorCompare;

        header << "p cnf " << aig->na + aig->ni  + 1 << " " << 3*aig->na + 5 << "\n";

        xorCompare << "-" << aig->outs.at(indexOut) / 2 + 1000  <<  " -" << aig->outs.at(indexOut) / 2 + 2000  << " -" << "3000" << " 0\n" 
        << "-" << aig->outs.at(indexOut) / 2 + 1000  << " "  << aig->outs.at(indexOut) / 2 +2000 <<  " " << "3000" << " 0\n"
        << " " << aig->outs.at(indexOut) / 2 + 1000  << " -"  << aig->outs.at(indexOut) / 2 +2000 <<  " " << "3000" << " 0\n"
        << " " << aig->outs.at(indexOut) / 2 + 1000  << " "  << aig->outs.at(indexOut) / 2 +2000 <<  " -" << "3000" << " 0\n"
        << "3000" << " 0\n";
        sats.push_back(getSatCnf(header.str(), aig, outNode, 1000)
        .append(getSatCnf("", aig2, outNode, 2000)
        .append(xorCompare.str())));
    }
    return sats;
}