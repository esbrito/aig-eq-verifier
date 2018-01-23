//copyright Prof. Andre Reis - UFRGS


#include "aig.h"


void Aig::print_aig(Aig* aig){
for(int indexNode = 1; indexNode <= (aig->ni + aig->na) ; indexNode++){
        if(aig->left.at(indexNode) == 0 && aig->right.at(indexNode) == 0){
       
        }else{
            cout << "Nodo: " << indexNode;
            if(aig->left.at(indexNode)%2 == 0){
                cout << " " << aig->left.at(indexNode) / 2;
            }else{
                cout << " !" << aig->left.at(indexNode) / 2;
            }
            cout << " AND " ;
            if( aig->right.at(indexNode)%2 == 0){
                cout << " " << aig->right.at(indexNode) / 2;
            }else{
                cout << " !" << aig->right.at(indexNode) / 2;
            }
            cout << '\n';
        }
    }

    for(int indexIn = 0; indexIn < aig->ni ; indexIn++){
    if(aig->ins.at(indexIn)%2 == 0){
                cout << aig->ins.at(indexIn) / 2 <<  " é entrada\n";
            }else{
                cout << "!" << aig->ins.at(indexIn) / 2 <<  " é entrada\n";
            }
        }

    for(int indexOut = 0; indexOut < aig->no ; indexOut++){
    if(aig->outs.at(indexOut)%2 == 0){
                cout << aig->outs.at(indexOut) / 2 <<  " é saída\n";
            }else{
                cout << "!" << aig->outs.at(indexOut) / 2 <<  " é saída\n";
            }
        }

    
    
}