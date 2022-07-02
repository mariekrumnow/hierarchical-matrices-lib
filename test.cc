#include "Exceptions.hpp"

#include "EntrywiseBlock.hpp"
#include "OuterProductBlock.hpp"
#include "HierarchicalMatrix.hpp"

#include "functions/addition.cpp"
#include "functions/coarse.cpp"
#include "functions/constructor.cpp"
#include "functions/destructor.cpp"
#include "functions/inversion.cpp"
#include "functions/luDecomposition.cpp"
#include "functions/matrixMatrixMult.cpp"
#include "functions/matrixVectorMult.cpp"

#include <iostream>
#include <list>
#include <vector>

#define SIZE 2

int main(int argc, char** argv){
      double dataArr[SIZE][SIZE]=
                              { {1.0, 2.0},
                              {2.0, 4.0}};
                              // { {1.0, 2.0, 3.0},
                              // {2.0, 4.0, 6.0},
                              // {3.0, 8.0, 9.0}};
                            // { {1.0, 2.0, 0.0, 0.0},
                            //   {2.0, 4.0, 6.0, 0.0},
                            //   {0.0, 8.0, 9.0, 12.0},
                            //   {0.0, 0.0, 12.0, 16.0}};
                              // { {1.0, 2.0, 0.0, 0.0, 0.0},
                              //  {2.0, 4.0, 6.0, 0.0, 0.0},
                              //  {0.0, 8.0, 9.0, 12.0, 0.0},
                              //  {0.0, 0.0, 12.0, 16.0, 20.0},
                              //  {0.0, 0.0, 0.0, 20.0, 25.0}};
      double ** data = new double*[SIZE];
      for(int i=0; i<SIZE; i++){
            data[i] = new double[SIZE];

            for(int j=0; j<SIZE; j++){
                  data[i][j] = dataArr[i][j];
            }
      }

      std::list<std::vector<unsigned int>> indices;

      // Puts 1 indice in each vector
      std::vector<unsigned int> indice[1];
      for(int i=0; i<SIZE; i++){
            indice[i].push_back(i);
            indices.push_back(indice[i]);
      }

      // Testing constructor
      HierarchicalMatrix<double> exampleBlock(data, &indices, SIZE);

      // Testing addition
      // exampleBlock + exampleBlock;

      // Testing matrix-vector-multiplication
      // double xVector[SIZE];
      // for(int i=0; i<SIZE; i++){
      //       xVector[i] = i;
      // }
      // double* result = exampleBlock * xVector;

      std::cout << std::endl << "Success!";

      return 0;
}
