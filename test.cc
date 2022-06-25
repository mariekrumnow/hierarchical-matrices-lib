#include "Exceptions.hpp"

#include "EntrywiseBlock.hpp"
#include "OuterProductBlock.hpp"
#include "HierarchicalMatrix.hpp"

#include "functions/addition.cpp"
#include "functions/coarse.cpp"
#include "functions/constructor.cpp"
#include "functions/inversion.cpp"
#include "functions/luDecomposition.cpp"
#include "functions/matrixMatrixMult.cpp"
#include "functions/matrixVectorMult.cpp"

#include <iostream>
#include <list>
#include <vector>

#define SIZE 4

int main(int argc, char** argv){
      double daten1[SIZE][SIZE]=
                              // { {1.0, 2.0},
                              // {2.0, 4.0}};
                              // { {1.0, 2.0, 3.0},
                              // {2.0, 4.0, 6.0},
                              // {3.0, 8.0, 9.0}};
                            { {1.0, 2.0, 0.0, 0.0},
                              {2.0, 4.0, 6.0, 0.0},
                              {0.0, 8.0, 9.0, 12.0},
                              {0.0, 00.0, 12.0, 16.0}};
      double ** daten = new double*[SIZE];
      for(int i=0; i<SIZE; i++){
            daten[i] = new double[SIZE];

            for(int j=0; j<SIZE; j++){
                  daten[i][j] = daten1[i][j];
            }
      }
      std::vector<unsigned int> indice1 (0);
      std::vector<unsigned int> indice2 (1);
      std::vector<unsigned int> indice3 (2);
      std::vector<unsigned int> indice4 (3);
      std::list<std::vector<unsigned int>> indices;
      indices.push_back(indice1);
      indices.push_back(indice2);
      indices.push_back(indice3);
      indices.push_back(indice4);

      HierarchicalMatrix<double> * exampleBlock = new HierarchicalMatrix<double>(daten, &indices, SIZE);

      return 0;
}
