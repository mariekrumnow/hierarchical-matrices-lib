#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"
#include <iostream> // For testing
#include <cmath>

// Bitte nicht mehr die Funktionsköpfe ändern oder falls nötig Bescheid geben!

// Eingabevektor hat Größe Block<datatype>::nDim
// Ausgabevektor hat Größe Block<datatype>::mDim

// * Hierachische Matrix
template <class datatype>
datatype* HierarchicalMatrix<datatype>::operator*( const datatype vector[] ){
    datatype* mergMatrix = new datatype[this->mDim];              //result
    unsigned int halfDim = ceil(this->mDim/2.0);                    //to adapt the Dimension of the vector

    datatype* tmpMatrix[4];   //Temp Matrix to save the results from the calculation of matrix * vector
    datatype v1[halfDim];     //Splitted Vector
    datatype v2[halfDim];

    for (unsigned int i = 0; i < halfDim; i++) {                 //splitting of the vector
        v1[i] = vector[i];
    }
    for (unsigned int i = halfDim; i < this->mDim; i++) {
        v2[i] = vector[i];
    }
      for(unsigned int a=0; a<2; a++) {
          for (unsigned int b = 0; b < 2; b++) {
              if(a == 0)
              {
                  if (b == 0 && matrix[a][b]!= nullptr)             //A
                  {

                      tmpMatrix[0] = *matrix[a][b] * v1;
                  }
                  if(b==1 && matrix[a][b] != nullptr)              //C
                  {
                      tmpMatrix[2] = *matrix[a][b] * v1;
                  }

              }
              else {
                  if (a == 1 && b == 0 && matrix[a][b] != nullptr)     //B
                  {
                      tmpMatrix[1] = *matrix[a][b] * v2;
                  }
                  if (a == 1 && b == 1 && matrix[a][b] != nullptr)         //D
                  {
                      tmpMatrix[3] = *matrix[a][b] * v2;
                  }
              }
          }
      }

    for (unsigned int i = 0; i < 4; i++) {
        if(i ==0) //A
        {
            for (unsigned int j = 0; j < matrix[0][0]->indiceRange[0][1]; j++) {
                mergMatrix[j] = tmpMatrix[i][j];
            }
        }
        if(i == 1) //B
        {
            for (unsigned int j = matrix[0][1]->indiceRange[0][0]; j < matrix[0][1]->indiceRange[0][1]; j++) {
                mergMatrix[j] = tmpMatrix[i][j];
            }
        }
        if(i == 2) //C
        {
            for (unsigned int j = matrix[1][0]->indiceRange[0][0]; j < matrix[1][0]->indiceRange[0][1]; j++) {
                mergMatrix[j] = tmpMatrix[i][j];
            }
        }
        if(i == 3) //D
        {
            for (unsigned int j = matrix[1][1]->indiceRange[0][0]; j < matrix[1][1]->indiceRange[0][1]; j++) {
                mergMatrix[j] = tmpMatrix[i][j];
            }
        }

    }
    //mergMatrix[0] = tmpMatrix[0][0] + tmpMatrix[0][1]; //A+B
    //mergMatrix[1] = tmpMatrix[1][0] + tmpMatrix[1][1]; //C+D
    return mergMatrix;
}
// * OuterProduct Form
template <class datatype>
datatype* OuterProductBlock<datatype>::operator*( const datatype vector[] ){
      datatype* result = new datatype[Block<datatype>::mDim];                       // Results from the class
      datatype** Ux [this->mDim][this->k];    // Ux = U * x, x U have to multiplie U with x

      datatype* vUx = new datatype[Block<datatype>::mDim];                          // vUx = Ux * v, v ist the Vector
      datatype* VvUx = new datatype[Block<datatype>::mDim];                           // VvUx = vUx * V
                          //u*x*v, Range Ndim * k

        for(unsigned int i = 0; i < this->mDim; i++)                 // Matrix Matrix Multiplication of u and x
            for(unsigned int j = 0; j < this->k; j++)
                for(unsigned int l = 0; l < this->k; l++)
                {
                   **Ux[i][j] += this->u[i][l] * this->x[l][j];
                }

    /*for(unsigned int a=0; a < dim; a++){
        delete[] distance[a];
    }*/

     /* for (int i = indiindiceRange[0][0]; i < indiindiceRange[0][1]; i++ )
      {
          Vvx[i] = Vx * vector[i];
      }*/

     //Passt das alles vond en Dimensionen her? Kann ich immer inddindice Range nehemen?

    for (unsigned int i = 0;
    i < this->mDim; i++) { // Row
        for (unsigned int j = 0; j <  this->k; j++) { //Col
            vUx[i] += **Ux[i][j] * vector[j];
        }
    }

    for (unsigned int i = 0; i < this->nDim; i++) {
        for (unsigned int j = 1; j < this->k; j++) {
            VvUx[i] += this->v[i][j] * vUx[j];
        }
    }
    return VvUx;
}


// EntrywiseBlock
template <class datatype>
datatype* EntrywiseBlock<datatype>::operator*( const datatype vector[] ){
      datatype* result = new datatype[Block<datatype>::mDim];

        for (unsigned int i = 0; i < this->mDim; i++) {
            for (unsigned int j = 0; j < this->nDim; j++) {

                result[i] += block[i][j] * vector[j];
            }
        }
      return result;
}
