#include "Exceptions.hpp"

#include "EntrywiseBlock.hpp"
#include "OuterProductBlock.hpp"
#include "HierarchicalMatrix.hpp"

#include "EntrywiseBlock.cpp"
#include "OuterProductBlock.cpp"

#include "functions/addition.cpp"
#include "functions/coarse.cpp"
#include "functions/constructor.cpp"
#include "functions/inversion.cpp"
#include "functions/luDecomposition.cpp"
#include "functions/matrixMatrixMult.cpp"
#include "functions/matrixVectorMult.cpp"

#include <iostream>

int main(int argc, char** argv){
      double daten1[2][2]= { {1.1, 2.2}, {3.3, 4.4} };
      double ** daten = new double*[2];
      for(int i=0; i<2; i++){
            daten[i] = new double[2];

            for(int j=0; j<2; j++){
                  daten[i][j] = daten1[i][j];
                  std::cout << daten[i][j] << " ";
            }
      }
      HierarchicalMatrix<double> * exampleBlock = new HierarchicalMatrix<double>(daten, 2, 2);

      std::cout << "Warnungsvermeider: " << sizeof(exampleBlock); // Vermeidet Warning, dass exampleBlock unbenutzt ist

      return 0;
}
