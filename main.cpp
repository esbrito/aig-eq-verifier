#include <cstdlib>
#include "aagReader.h"
#include "gerentebdd.h"
#include "aigAPI.h"

using namespace std;

int main()
{

    AigAux aigAux;

    AAGReader reader("xor.aag");
    Aig *aig = reader.readFile();
    aigAux.print_aig(aig);

    AAGReader reader2("xorEq.aag");
    Aig *aig2 = reader2.readFile();
    aigAux.print_aig(aig2);
    std::vector<string> sats =  aigAux.create_sats_eq_from_aig(aig, aig2);

    ofstream satCnf;
    satCnf.open("sat.cnf");
    satCnf << aigAux.create_sats_cnf_from_aig(aig, aig2).at(0);
    
    std::vector<string> equations = aigAux.create_equations_from_aig(aig);

    std::vector<string> equations2 = aigAux.create_equations_from_aig(aig2);

    for (int indexOut = 0; indexOut < aig->no; indexOut++)
    {
        cout << "Sat aigs  para a saida:" << aig->outs.at(indexOut) /2 << "\n";
        cout << sats.at(indexOut) << "\n";

        gerentebdd g;
        set<string> conjunto_variaveis;
        nodobdd *nd1 = g.create_from_equation(equations.at(indexOut), conjunto_variaveis);
        nodobdd *nd2 = g.create_from_equation(equations2.at(indexOut), conjunto_variaveis);

        if (nd2 == nd1)
        {
            cout << "EQUAÇÃO DA SAIDA DO NÓ " << aig->outs.at(indexOut) /2 << " DE AMBOS AIGS SAO EQUIVALENTES"
            << "\n";
        }else
        {
            cout << "NAO EQUIVALENTES"
                 << "\n";
            return false;
        }
    }

     cout << "OS AIGS SAO TOTALMENTE EQUIVALENTES!"
            << "\n";
    return true;
}
