#include <cstdlib>
#include "aagReader.h"
#include "gerentebdd.h"

using namespace std;


int main()
{

    AAGReader reader("xor.aag");
    Aig* aig = reader.readFile();

    AAGReader reader2("xorEquivalent.aag");
    Aig* aig2 = reader2.readFile();

    gerentebdd g;
    std::vector<string> equations = g.create_equations_from_aig(aig);

    std::vector<string> equations2 = g.create_equations_from_aig(aig2);

    
    for(int indexOut = 0; indexOut < aig->no ; indexOut++){
        cout << equations.at(indexOut)  << "\n";;
    }

    for(int indexOut = 0; indexOut < aig->no ; indexOut++){
        cout << equations2.at(indexOut)  << "\n";;
    }
    
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
