#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"
#include "calcRank.hpp"

#include <cmath>
#include <iostream> // For testing

// Order of the functions: HM > OP > EW

// thanks to a + b = b + a we only need to define
// EW + HM / EW + OP / OP + HM / HM + HM
// all the other combinations fall back on these

// HM + HM interface
template <class datatype>
HierarchicalMatrix<datatype>* HierarchicalMatrix<datatype>::operator+( const HierarchicalMatrix<datatype>& addedMatrix ){

      HierarchicalMatrix<datatype>* temp = new HierarchicalMatrix<datatype>(); // create an empty HM to return later

      temp->nDim = this->nDim;      // filling in the values
      temp->mDim = this->mDim;

      for(unsigned int a = 0; a < 2; a++){
            for(unsigned int b = 0; b < 2; b++){
                  temp->indiceRange[a][b] = this->indiceRange[a][b];
            }
      }

      for(unsigned int a=0; a<2; a++){          // filling the new Matrix with the sum of the old ones, while checking for empty blocks to avoid crashes
            for(unsigned int b=0; b<2; b++){

                  if(matrix[a][b] != nullptr && addedMatrix.matrix[a][b] != nullptr)
                  {
                        temp->matrix[a][b] = *matrix[a][b] + addedMatrix.matrix[a][b];
                  }
                  else if(matrix[a][b] != nullptr && addedMatrix.matrix[a][b] == nullptr)
                  {
                        temp->matrix[a][b] = matrix[a][b];
                  }
                  else if(matrix[a][b] == nullptr && addedMatrix.matrix[a][b] != nullptr)
                  {
                        temp->matrix[a][b] = addedMatrix.matrix[a][b];
                  }
                  else
                  {
                        temp->matrix[a][b] = nullptr;
                  }
            }
      }

      return temp;
}


// Inclusion of += operator, calls the + function
template <class datatype>
HierarchicalMatrix<datatype>* HierarchicalMatrix<datatype>::operator+=( const HierarchicalMatrix<datatype>& addedMatrix ){

      *this = *(*this + addedMatrix);

      return this;
}


// HM + Block
template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( Block<datatype>* addedBlock ){
      // Block is of unknown type so we call the funktion "backwards" to find out
      return *addedBlock + this;
}


// HM + HM (works the same as l.14)
template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( HierarchicalMatrix<datatype>* addedMatrix ){

      HierarchicalMatrix<datatype>* temp = new HierarchicalMatrix<datatype>();

      temp->nDim = this->nDim;
      temp->mDim = this->mDim;

      for(unsigned int a=0; a<2; a++){
            for(unsigned int b=0; b<2; b++){

                  if(matrix[a][b] != nullptr && addedMatrix->matrix[a][b] != nullptr)
                  {
                        temp->matrix[a][b] = *matrix[a][b] + addedMatrix->matrix[a][b];
                  }
                  else if(matrix[a][b] != nullptr && addedMatrix->matrix[a][b] == nullptr)
                  {
                        temp->matrix[a][b] = matrix[a][b];
                  }
                  else if(matrix[a][b] == nullptr && addedMatrix->matrix[a][b] != nullptr)
                  {
                        temp->matrix[a][b] = addedMatrix->matrix[a][b];
                  }
                  else
                  {
                        temp->matrix[a][b] = nullptr;
                  }
            }
      }

      for(unsigned int a = 0; a < 2; a++){
            for(unsigned int b = 0; b < 2; b++){
                  temp->indiceRange[a][b] = this->indiceRange[a][b];
            }
      }
      return temp;
}


// HM + OP
template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( OuterProductBlock<datatype>* addedBlock ){

      unsigned int a, b;

      // Calculating the new Dimensions of the smaller Blocks to create matching Blocks
      unsigned int newNDimL = (matrix[0][0]->indiceRange[kRangeI][kTop] - matrix[0][0]->indiceRange[kRangeI][kBottom]);
      unsigned int newNDimR = (this->indiceRange[kRangeI][kTop] - matrix[0][0]->indiceRange[kRangeI][kTop]);
      unsigned int newMDimT = (matrix[0][0]->indiceRange[kRangeJ][kTop] - matrix[0][0]->indiceRange[kRangeJ][kBottom]);
      unsigned int newMDimB = (this->indiceRange[kRangeJ][kTop] - matrix[0][0]->indiceRange[kRangeI][kTop]);

      // Creating the U and V of these Blocks; still empty
      datatype** Utop = new datatype*[addedBlock->k];
      for(a=0; a < addedBlock->k; a++){
            Utop[a] = new datatype[newMDimT];
      }
      datatype** Ubottom = new datatype*[addedBlock->k];
      for(a=0; a < addedBlock->k; a++){
            Ubottom[a] = new datatype[newMDimB];
      }
      datatype** Vleft = new datatype*[newNDimL];
      for(a=0; a < newNDimL; a++){
            Vleft[a] = new datatype[addedBlock->k];
      }
      datatype** Vright = new datatype*[newNDimR];
      for(a=0; a < newNDimR; a++){
            Vright[a] = new datatype[addedBlock->k];
      }

      // Filling U
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < newMDimT; b++)
            {
                  Utop[a][b] = addedBlock->u[a][b];
            }
      }

      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < newMDimB; b++)
            {
                  Ubottom[a][b] = addedBlock->u[a][b + newMDimT];
            }
      }

      // Filling V
      for(a = 0; a < newNDimL; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  Vleft[a][b] = addedBlock->v[a][b];
            }
      }

      for(a = 0; a < newNDimR; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  Vright[a][b] = addedBlock->v[a + newNDimL][b];
            }
      }

      // Creation and filling of new OP-Objects
      OuterProductBlock<datatype>* newA = new OuterProductBlock<datatype>();

      newA->nDim = newNDimL;
      newA->mDim = newMDimT;
      newA->k = addedBlock->k;
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < newMDimT; b++)
            {
                  newA->u[a][b] = Utop[a][b];
            }
      }
      for(a = 0; a < newNDimL; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newA->v[a][b] = Vleft[a][b];
            }
      }
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newA->x[a][b] = addedBlock->x[a][b];
            }
      }

      OuterProductBlock<datatype>* newB = new OuterProductBlock<datatype>();

      newB->nDim = newNDimR;
      newB->mDim = newMDimT;
      newB->k = addedBlock->k;
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < newMDimT; b++)
            {
                  newB->u[a][b] = Utop[a][b];
            }
      }
      for(a = 0; a < newNDimR; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newB->u[a][b] = Vright[a][b];
            }
      }
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newB->x[a][b] = addedBlock->x[a][b];
            }
      }

      OuterProductBlock<datatype>* newC = new OuterProductBlock<datatype>();

      newC->nDim = newNDimL;
      newC->mDim = newMDimB;
      newC->k = addedBlock->k;
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < newMDimB; b++)
            {
                  newC->u[a][b] = Ubottom[a][b];
            }
      }
      for(a = 0; a < newNDimL; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newC->u[a][b] = Vleft[a][b];
            }
      }
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newC->x[a][b] = addedBlock->x[a][b];
            }
      }

      OuterProductBlock<datatype>* newD = new OuterProductBlock<datatype>();

      newD->nDim = newNDimR;
      newD->mDim = newMDimB;
      newD->k = addedBlock->k;
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < newMDimB; b++)
            {
                  newD->u[a][b] = Ubottom[a][b];
            }
      }
      for(a = 0; a < newNDimR; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newD->u[a][b] = Vright[a][b];
            }
      }
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newD->x[a][b] = addedBlock->x[a][b];
            }
      }

      // Creating a new HM to return later
      HierarchicalMatrix<datatype>* temp = new HierarchicalMatrix<datatype>();

      temp->nDim = this->nDim;
      temp->mDim = this->mDim;

      for(a = 0; a < 2; a++){
            for(b = 0; b < 2; b++){
                  temp->indiceRange[a][b] = this->indiceRange[a][b];
            }
      }

      // Filling it with the sum of the blocks of the HM and the new OP-blocks, while checking for empty blocks
      if(matrix[0][0] != nullptr)
      {
            temp->matrix[0][0] = *matrix[0][0] + newA;
      }
      else
      {
            temp->matrix[0][0] = newA;
      }

      if(matrix[0][1] != nullptr)
      {
            temp->matrix[0][1] = *matrix[0][1] + newB;
      }
      else
      {
            temp->matrix[0][1] = newB;
      }

      if(matrix[1][0] != nullptr)
      {
            temp->matrix[1][0] = *matrix[1][0] + newC;
      }
      else
      {
            temp->matrix[1][0] = newC;
      }

      if(matrix[1][1] != nullptr)
      {
            temp->matrix[1][1] = *matrix[1][1] + newD;
      }
      else
      {
            temp->matrix[1][1] = newD;
      }

      // Cleanup of temporay objects
      for(a=0; a < newMDimT; a++){
            delete[] Utop[a];
      }
      delete[] Utop;
      delete newA;

      for(a=0; a <newMDimB; a++){
            delete[] Ubottom[a];
      }
      delete[] Ubottom;
      delete newB;

      for(a=0; a < addedBlock->k; a++){
            delete[] Vleft[a];
      }
      delete[] Vleft;
      delete newC;

      for(a=0; a < addedBlock->k; a++){
            delete[] Vright[a];
      }
      delete[] Vright;
      delete newD;

      return temp;
}


// HM + EW
template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){

      unsigned int a, b;

      // Calculating the new Dimensions of the smaller Blocks to create matching blocks
      unsigned int newNDimL = (matrix[0][0]->indiceRange[kRangeI][kTop] - matrix[0][0]->indiceRange[kRangeI][kBottom]);
      unsigned int newNDimR = (this->indiceRange[kRangeI][kTop] - matrix[0][0]->indiceRange[kRangeI][kTop]);
      unsigned int newMDimT = (matrix[0][0]->indiceRange[kRangeJ][kTop] - matrix[0][0]->indiceRange[kRangeJ][kBottom]);
      unsigned int newMDimB = (this->indiceRange[kRangeJ][kTop] - matrix[0][0]->indiceRange[kRangeI][kTop]);

      // Creating these blocks
      EntrywiseBlock<datatype>* newA = new EntrywiseBlock<datatype>();
      newA->nDim = newNDimL;
      newA->mDim = newMDimT;
      EntrywiseBlock<datatype>* newB = new EntrywiseBlock<datatype>();
      newB->nDim = newNDimR;
      newB->mDim = newMDimT;
      EntrywiseBlock<datatype>* newC = new EntrywiseBlock<datatype>();
      newC->nDim = newNDimL;
      newC->mDim = newMDimB;
      EntrywiseBlock<datatype>* newD = new EntrywiseBlock<datatype>();
      newD->nDim = newNDimR;
      newD->mDim = newMDimB;

      // Temporary martices to save the new blocks
      datatype** A = new datatype*[newMDimT];
      for(a=0; a < newMDimT; a++){
            A[a] = new datatype[newNDimL];
      }
      datatype** B = new datatype*[newMDimT];
      for(a=0; a < newMDimT; a++){
            B[a] = new datatype[newNDimR];
      }
      datatype** C = new datatype*[newMDimB];
      for(a=0; a < newMDimB; a++){
            C[a] = new datatype[newNDimL];
      }
      datatype** D = new datatype*[newMDimB];
      for(a=0; a < newMDimB; a++){
            D[a] = new datatype[newNDimR];
      }

      // Filling these blocks
      for(a = 0; a < newMDimT; a++)
      {
            for(b = 0; b < newNDimL; b++)
            {
                  A[a][b] = addedBlock->block[a][b];
            }
      }

      for(a = 0; a < newMDimT; a++)
      {
            for(b = 0; b < newNDimR; b++)
            {
                  B[a][b] = addedBlock->block[a][b + newNDimL];
            }
      }

      for(a = 0; a < newMDimB; a++)
      {
            for(b = 0; b < newNDimL; b++)
            {
                  C[a][b] = addedBlock->block[a + newMDimT][b];
            }
      }

      for(a = 0; a < newMDimB; a++)
      {
            for(b = 0; b < newNDimR; b++)
            {
                  D[a][b] = addedBlock->block[a + newMDimT][b + newNDimL];
            }
      }

      // Tranfering blocks into the new EW-objects
      for(a = 0; a < newMDimT; a++)
      {
            for(b = 0; b < newNDimL; b++)
            {
                  newA->block[a][b] = A[a][b];
            }
      }

      for(a = 0; a < newMDimT; a++)
      {
            for(b = 0; b < newNDimR; b++)
            {
                  newB->block[a][b] = B[a][b];
            }
      }

      for(a = 0; a < newMDimB; a++)
      {
            for(b = 0; b < newNDimL; b++)
            {
                  newC->block[a][b] = C[a][b];
            }
      }

      for(a = 0; a < newMDimB; a++)
      {
            for(b = 0; b < newNDimR; b++)
            {
                  newD->block[a][b] = D[a][b];
            }
      }

      // Creating a new HM to return later
      HierarchicalMatrix<datatype>* temp = new HierarchicalMatrix<datatype>();

      temp->nDim = this->nDim;
      temp->mDim = this->mDim;

      // Filling it with the sum of the blocks of the HM and the new EW-blocks, while checking for empty blocks
      for(a = 0; a < 2; a++){
            for(b = 0; b < 2; b++){
                  temp->indiceRange[a][b] = this->indiceRange[a][b];
            }
      }

      if(matrix[0][0] != nullptr)
      {
            temp->matrix[0][0] = *matrix[0][0] + newA;
      }
      else
      {
            temp->matrix[0][0] = newA;
      }

      if(matrix[0][1] != nullptr)
      {
            temp->matrix[0][1] = *matrix[0][1] + newB;
      }
      else
      {
            temp->matrix[0][1] = newB;
      }

      if(matrix[1][0] != nullptr)
      {
            temp->matrix[1][0] = *matrix[1][0] + newC;
      }
      else
      {
            temp->matrix[1][0] = newC;
      }

      if(matrix[1][1] != nullptr)
      {
            temp->matrix[1][1] = *matrix[1][1] + newD;
      }
      else
      {
            temp->matrix[1][1] = newD;
      }

      // Cleanup of temporary objects
      delete newA;
      delete newB;
      delete newC;
      delete newD;
      for(a=0; a < newMDimT; a++){
            delete[] A[a];
      }
      delete[] A;
      for(a=0; a < newMDimT; a++){
            delete[] B[a];
      }
      delete[] B;
      for(a=0; a < newMDimB; a++){
            delete[] C[a];
      }
      delete[] C;
      for(a=0; a < newMDimB; a++){
            delete[] D[a];
      }
      delete[] D;

      return temp;
}


// OP + Block
template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( Block<datatype>* addedBlock ){
      // Block is of unknown type so we call the funktion "backwards" to find out
      return *addedBlock + this;
}


template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( HierarchicalMatrix<datatype>* addedBlock ){
      // OP + HM == HM + OP | call function with swapped parameters
      return *addedBlock + this;
}


// OP + OP
template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( OuterProductBlock<datatype>* addedBlock ){

      unsigned int a = 0;
      unsigned int b = 0;

      // Creating a new OP to return later
      OuterProductBlock<datatype>* temp = new OuterProductBlock<datatype>();

      // Creating temporay U X V
      datatype** tempU = new datatype*[addedBlock->mDim];
      for(a=0; a < addedBlock->mDim; a++){
            tempU[a] = new datatype[k + addedBlock->k];
      }
      datatype** tempX = new datatype*[k + addedBlock->k];
      for(a=0; a < (k + addedBlock->k); a++){
            tempX[a] = new datatype[k + addedBlock->k];
      }
      datatype** tempV = new datatype*[(k + addedBlock->k)];
      for(a=0; a < (k + addedBlock->k); a++){
            tempV[a] = new datatype[addedBlock->nDim];
      }

      // Filling the temporary variables, which also calculates the result

      // Filling tempu
      for(a = 0; a < k; a++)
      {
            for(b = 0; b < addedBlock->mDim; b++)
            {
                  tempU[a][b] = u[a][b];
            }
      }

      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->mDim; b++)
            {
                  tempU[b][a+k] = addedBlock->u[b][a];
            }
      }

      // Filling tempv
      for(a = 0; a < addedBlock->nDim; a++)
      {
            for(b = 0; b < k; b++)
            {
                  tempV[a][b] = v[a][b];
            }
      }
      for(a = 0; a < addedBlock->nDim; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  tempV[a][b + k] = addedBlock->u[a][b];
            }
      }

      // Filling tempx
      for(a = 0; a < (k + addedBlock->k); a++)
      {
            for(b = 0; b < (k + addedBlock->k); b++)
            {
                  tempX[a][b] = 0;
            }
      }
      for(a = 0; a < k; a++)
      {
            for(b = 0; b < k; b++)
            {
                  tempX[a][b] = x[a][b];
            }
      }
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  tempX[a + k][b + k] = addedBlock->u[a][b];
            }
      }

      // Filling U of new OP
      temp->u = new datatype*[addedBlock->mDim];
      for(a=0; a < addedBlock->mDim; a++){
            temp->u[a] = new datatype[k + addedBlock->k];
      }
      for(a = 0; a < (k + addedBlock->k); a++)
      {
            for(b = 0; b < addedBlock->mDim; b++)
            {
                  temp->u[b][a] = tempU[b][a];
            }
      }

      // Filling V of new OP
      temp->v = new datatype*[addedBlock->nDim];
      for(a=0; a < addedBlock->nDim; a++){
            temp->v[a] = new datatype[k + addedBlock->k];
      }
      for(a = 0; a < addedBlock->nDim; a++)
      {
            for(b = 0; b < (k + addedBlock->k); b++)
            {
                  temp->v[a][b] = tempV[a][b];
            }
      }

      // Filling X of new OP
      temp->x = new datatype*[k + addedBlock->k];
      for(a=0; a < k + addedBlock->k; a++){
            temp->x[a] = new datatype[k + addedBlock->k];
      }
      for(a = 0; a < (k + addedBlock->k); a++)
      {
            for(b = 0; b < (k + addedBlock->k); b++)
            {
                  temp->x[a][b] = tempX[a][b];
            }
      }

      temp->nDim = this->nDim;
      temp->mDim = this->mDim;

      for(a = 0; a < 2; a++){
            for(b = 0; b < 2; b++){
                  temp->indiceRange[a][b] = this->indiceRange[a][b];
            }
      }

      temp->k = k + addedBlock->k;

      // Cleanup of temporary objects
      for(a=0; a < addedBlock->mDim; a++){
            delete[] tempU[a];
      }
      delete[] tempU;
      for(a=0; a < (k + addedBlock->k); a++){
            delete[] tempX[a];
      }
      delete[] tempX;
      for(a=0; a < addedBlock->nDim; a++){
            delete[] tempV[a];
      }
      delete[] tempV;

      return temp;
}


// OP + EW
template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){

      // Since OP + EW is not simply possible we transform the EW into OP
      // Calculating the nesesary params
      unsigned int newk = calcRank(addedBlock->mDim, addedBlock->nDim, addedBlock->block);

      std::vector<unsigned int> iInd;
      iInd.push_back(this->indiceRange[kRangeI][kBottom]);
      iInd.push_back(this->indiceRange[kRangeI][kTop]);

      std::vector<unsigned int> jInd;
      jInd.push_back(this->indiceRange[kRangeJ][kBottom]);
      jInd.push_back(this->indiceRange[kRangeJ][kTop]);

      OuterProductBlock<datatype>* temp = new OuterProductBlock<datatype>(addedBlock->block, addedBlock->mDim, addedBlock->nDim, iInd, jInd, 7);

      // Calling OP + OP
      return *this + temp;
}


// EW + Block
template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::operator+( Block<datatype>* addedBlock ){
      return *addedBlock + this; // Block is of unknown type so we call the funktion "backwards" to find out
}


template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::operator+( HierarchicalMatrix<datatype>* addedBlock ){
      // EW + HM == HM + EW | call function with swapped parameters
      return *addedBlock + this;
}


template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::operator+( OuterProductBlock<datatype>* addedBlock ){
      // EW + OP == OP + EW | call function with swapped parameters
      return *addedBlock + this;
}


// EW + EW
template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){

      // Creating a new EW to return later
      EntrywiseBlock<datatype>* temp = new EntrywiseBlock<datatype>();

      temp->nDim = this->nDim;
      temp->mDim = this->mDim;

      for(unsigned int a = 0; a < 2; a++){
            for(unsigned int b = 0; b < 2; b++){
                  temp->indiceRange[a][b] = this->indiceRange[a][b];
            }
      }

      // Adding the both EW into the new one
      for(unsigned int a = 0; a < addedBlock->mDim; a++)
      {
            for(unsigned int b = 0; b < addedBlock->nDim; b++)
            {
                  temp->block[a][b] = block[a][b] + addedBlock->block[a][b];
            }
      }


      return temp;
}
