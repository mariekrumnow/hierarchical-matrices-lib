
// This file serves as an example of what can currently be done with the library
// and how to use the different functions

#include "hierarchical_matrices_lib.cpp"

#include <cmath>
#include <iostream>
#include <list>
#include <vector>

#define SIZE 4

int main(int argc, char** argv){
      // Example matrices
      // Generated via MatLab with n = dim, k = how small data is
      // A = eye(n); for i=1:n, for j=1:n, A(i,j) = 1./(1+abs(i-j)).^k; end; end;
      double dataArr[SIZE][SIZE]=
                              // { {1.0, 2.0},
                              // {2.0, 4.0}};
                              // { {1.0, 2.0, 3.0},
                              // {2.0, 4.0, 6.0},
                              // {3.0, 8.0, 9.0}};
                            { {1.0, 0.125, 0.037037, 0.0156},
                              {0.125, 1.0, 0.125, 0.037037},
                              {0.037037, 0.125, 1.0, 0.125},
                              {0.0156, 0.037037, 0.125, 1.0}};
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
      // Puts 2 indices in each vector (as far as possible)
      int bruh = ceil(SIZE /2);
      std::vector<unsigned int> indice[bruh];
      for(int i=0; i<SIZE; i+=2){
            bruh = ceil(i/2);
            indice[bruh].push_back(i);
            indices.push_back(indice[bruh]);
      }

      // Initialization
      HierarchicalMatrix<double> exampleBlock(data, &indices, SIZE);

      // Coarsening
      exampleBlock.coarse(0.1);

      // Addition
      exampleBlock + exampleBlock;
      exampleBlock += exampleBlock;

      // Matrix-vector-multiplication
      double xVector[SIZE];
      for(int i=0; i<SIZE; i++){
            xVector[i] = i;
      }
      double* result = exampleBlock * xVector;

      std::cout << std::endl << "Success!";

      return 0;
}
