
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include "aig.h"

class AigAux{
public:
    std::vector<string> create_sats_eq_from_aig(Aig* aig, Aig* aig2);
    std::vector<string> create_sats_cnf_from_aig(Aig* aig, Aig* aig2);
    std::vector<string> create_equations_from_aig(Aig* aig);
    void print_aig(Aig* aig);
};