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

int main(int argc, char** argv){
      double daten1[3][3]= { {1.0, 2.0, 3.0},
                              {2.0, 4.0, 6.0},
                            {3.0, 8.0, 9.0}};
      double ** daten = new double*[3];
      for(int i=0; i<3; i++){
            daten[i] = new double[3];

            for(int j=0; j<3; j++){
                  daten[i][j] = daten1[i][j];
            }
      }
      std::vector<unsigned int> indice1 (0);
      std::vector<unsigned int> indice2 (1);
      std::vector<unsigned int> indice3 (2);
      std::list<std::vector<unsigned int>> indices;
      indices.push_back(indice1);
      indices.push_back(indice2);
      indices.push_back(indice3);

      HierarchicalMatrix<double> * exampleBlock = new HierarchicalMatrix<double>(daten, &indices, 2, 2);

      std::cout << "Warnungsvermeider: " << sizeof(exampleBlock); // Vermeidet Warning, dass exampleBlock unbenutzt ist

      return 0;
}
