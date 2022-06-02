#include "OuterProductBlock.hpp"

#include <iostream>

template <class datatype>
OuterProductBlock<datatype>::OuterProductBlock(const datatype (&originalBlock)[xDim][yDim], unsigned int rank)
      :Block<datatype>::Block(), k(rank)
{
      // Zeilen bzw Spalten rausfinden, die linear unabhängig sind
      // --> Zeilen aus Algorithmus für k rausfindbar?
      // Zeilen/Spalten so zerlegen (dividieren?), dass aus Mult wieder Matrix entsteht

      for(unsigned int i=0; i< xDim; i++){
            u[i] = new datatype[k];
            // u[i][0] = originalBlock[i][0];
      }

      for(unsigned int i=0; i< yDim; i++){
            v[i] = new datatype[k];
            // v[i][1] = originalBlock[0][i];
      }

      // k = Rang der Matrix? Weil Rang = Maximale Anzahl linear unabhängiger Zeilen/Spalten = k (S.10)
      // Berechnung des Rangs (k) des Blocks via
      // https://www.geeksforgeeks.org/program-for-rank-of-matrix/
      k = yDim;

      datatype copy[xDim][yDim]; // Kopie wird verändert!
      for(unsigned int i=0; i< xDim; i++){
            for(unsigned int j=0; j < yDim; j++){
                  copy[i][j] = originalBlock[i][j];
            }
      }


      for (unsigned int row = 0; row < k; row++){
            if (copy[row][row]){
                  for (unsigned int col = 0; col < xDim; col++){
                        if (col != row){
                              datatype mult = copy[col][row] / copy[row][row]; // Kann failen wenn int erlaubt
                              for (unsigned int i = 0; i < k; i++){
                                    copy[col][i] -= mult * copy[row][i];
                              }
                        }
                  }
            }
            else {
                  bool reduce = true;

                  for (unsigned int i = row + 1; i < xDim; i++) {
                        if (copy[i][row]) {
                              for (unsigned int j=0; j < k; j++) {
                                    datatype temp = copy[row][j];
                                    copy[row][j] = copy[i][j];
                                    copy[i][j] = temp;
                              }
                              reduce = false;
                              break;
                        }
                  }

                  if (reduce) {
                        k--;
                        for (unsigned int i=0; i < xDim; i++){
                              copy[i][row] = copy[i][k];
                        }
                  }

                  row--;
            }
      } // Rang k FERTIG
}

template <class datatype>
Block<datatype>& OuterProductBlock<datatype>::coarse(){

}

//---------------------------------------------------------------------------------------

template <class datatype>
OuterProductBlock<datatype>& OuterProductBlock<datatype>::operator+( const OuterProductBlock<datatype>& addedBlock ){

}

template <class datatype>
OuterProductBlock<datatype>& OuterProductBlock<datatype>::operator+( const EntrywiseBlock<datatype>& addedBlock ){

}

template <class datatype>
OuterProductBlock<datatype>& OuterProductBlock<datatype>::operator+( const HierarchicalMatrix<datatype>& addedBlock ){

}

//---------------------------------------------------------------------------------------

template <class datatype>
OuterProductBlock<datatype>& OuterProductBlock<datatype>::operator*( const OuterProductBlock<datatype>& multBlock ){

}

template <class datatype>
OuterProductBlock<datatype>& OuterProductBlock<datatype>::operator*( const EntrywiseBlock<datatype>& multBlock ){

}

template <class datatype>
OuterProductBlock<datatype>& OuterProductBlock<datatype>::operator*( const HierarchicalMatrix<datatype>& multBlock ){

}
