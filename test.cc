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
      double daten[2][2] = { {1.1, 2.2}, {3.3, 4.4} };
      HierarchicalMatrix<double, 2> * exampleBlock = new HierarchicalMatrix<double, 2>(daten);

      std::cout << sizeof(exampleBlock); // Vermeidet Warning, dass exampleBlock unbenutzt ist

      return 0;
}
