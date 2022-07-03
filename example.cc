
// This file serves as an example of what can currently be done with the library
// and how to use the different functions

#include "hierarchical_matrices_lib.cpp"

#include <iostream>
#include <list>
#include <vector>

#define SIZE 2

int main(int argc, char** argv){
      // Example matrices
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
      std::vector<unsigned int> indice[SIZE];
      for(int i=0; i<SIZE; i++){
            indice[i].push_back(i);
            indices.push_back(indice[i]);
      }

      // Initialization
      HierarchicalMatrix<double> exampleBlock(data, &indices, SIZE);

      // Addition
      // exampleBlock + exampleBlock;

      // Matrix-vector-multiplication
      // double xVector[SIZE];
      // for(int i=0; i<SIZE; i++){
      //       xVector[i] = i;
      // }
      // double* result = exampleBlock * xVector;

      std::cout << std::endl << "Success!";

      return 0;
}
