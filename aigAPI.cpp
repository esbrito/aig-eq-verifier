
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