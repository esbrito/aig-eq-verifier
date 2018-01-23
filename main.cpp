#include <cstdlib>
#include "aagReader.h"
#include "gerentebdd.h"
#include "aigAPI.h"

using namespace std;


int main()
{

    AigAux aigAux; 


    AAGReader reader("xor.aag");
    Aig* aig = reader.readFile();
    aigAux.print_aig(aig);
    cout << "Sat:" <<"\n";
    cout << aig->sat;
    cout <<"\n";


    AAGReader reader2("xorEquivalent.aag");
    Aig* aig2 = reader2.readFile();
    aigAux.print_aig(aig2);
    cout << "Sat:" <<"\n";
    cout << aig2->sat;
    cout <<"\n";


    std::vector<string> equations = aigAux.create_equations_from_aig(aig);

    std::vector<string> equations2 = aigAux.create_equations_from_aig(aig2);

    cout << "Equations 1:" <<"\n";

    for(int indexOut = 0; indexOut < aig->no ; indexOut++){
        cout << equations.at(indexOut)  << "\n";;
    }

    cout << "Equations 2:" <<"\n";

    for(int indexOut = 0; indexOut < aig->no ; indexOut++){
        cout << equations2.at(indexOut)  << "\n";;
    }
    

    gerentebdd g;
    set<string> conjunto_variaveis;
    nodobdd *nd1=g.create_from_equation(equations.at(0), conjunto_variaveis);
    nodobdd *nd2=g.create_from_equation(equations2.at(0), conjunto_variaveis);

    if (nd2 == nd1)
    {
        cout << "SAO EQUIVALENTES"
             << "\n";
        return true;
    }
    else
    {
        cout << "NAO EQUIVALENTES"
             << "\n";
        return false;
    }
}
