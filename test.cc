//Per strg+Slash die gewünschten Dateien am besten EINZELN wieder aktivieren und kompilieren, um den Code auf Fehler zu prüfen

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
      // Funktioniert, aber gibt Warning, dass exampleBlock unbenutzt ist!
      // 
      // double daten[2][2] = { {1.1, 2.2}, {3.3, 4.4} };
      // EntrywiseBlock<double, 2, 2> * exampleBlock = new EntrywiseBlock<double, 2, 2>(daten, 1, 1);

      // Kann nur ausgegeben werden, wenn die Attribute temporär public sind!
      //
      // std::cout << exampleBlock->indiceI << exampleBlock->indiceJ << '\n';
      // for(int i=0; i<2; i++){
      //       for(int j=0; j<2; j++){
      //             std::cout << exampleBlock->block[i][j] << std::endl;
      //       }
      // }

    return 0;
}
