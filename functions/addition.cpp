#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"
#include "calcRank.hpp"

#include <iostream> // For testing

// Bitte nicht mehr die Funktionsköpfe ändern oder falls nötig BEscheid geben!

// Reihenfolge der Funktionen: HM > OP > EW

// Da bei Addition die Reihenfolge egal ist, werden in folgenden nur die umgedrehte Funktion aufgerufen:
// EW + HM / EW + OP / OP + HM


// HierarchicalMatrix: public functions --> Noch alle (2) Funktionen offen
template <class datatype>
HierarchicalMatrix<datatype>* HierarchicalMatrix<datatype>::operator+( const HierarchicalMatrix<datatype>& addedMatrix ){
      // std::cout << " HM+HM ";

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
            }
      }

      for(unsigned int a = 0; a < 2; a++){
            for(unsigned int b = 0; b < 2; b++){
                  temp->indiceRange[a][b] = this->indiceRange[a][b];
            }
      }
      return temp; 
}

template <class datatype>
HierarchicalMatrix<datatype>* HierarchicalMatrix<datatype>::operator+=( const HierarchicalMatrix<datatype>& addedMatrix ){
      // Hier oder in der Funktion hierüber einfach sich gegenseitig aufrufen, z.B. return a+b
      // siehe https://en.cppreference.com/w/cpp/language/operators unter "Binary arithmetic operators"
}

// HierarchicalMatrix: private functions --> Noch 2 Funktionen offen
template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( Block<datatype>* addedBlock ){ // FERTIG
      return *addedBlock + this;
}

template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( HierarchicalMatrix<datatype>* addedMatrix ){ // FERTIG
      // HM + HM | Hier vllt auch nur wieder die public Funktion aufrufen?

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
            }
      }

      for(unsigned int a = 0; a < 2; a++){
            for(unsigned int b = 0; b < 2; b++){
                  temp->indiceRange[a][b] = this->indiceRange[a][b];
            }
      }
      return temp; 
}

template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( OuterProductBlock<datatype>* addedBlock ){
      // HM + OP
      unsigned int a, b;


      datatype** Uoben = new datatype*[addedBlock->k];
      for(a=0; a < addedBlock->k; a++){
            Uoben[a] = new datatype[addedBlock->mDim/2];
      }
      datatype** Uunten = new datatype*[addedBlock->k];
      for(a=0; a < addedBlock->k; a++){
            Uunten[a] = new datatype[addedBlock->mDim/2];
      }
      datatype** Vlinks = new datatype*[addedBlock->nDim/2];
      for(a=0; a < addedBlock->nDim/2; a++){
            Vlinks[a] = new datatype[addedBlock->k];
      }
      datatype** Vrechts = new datatype*[addedBlock->nDim/2];
      for(a=0; a < addedBlock->nDim/2; a++){
            Vrechts[a] = new datatype[addedBlock->k];
      }

      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->mDim/2; b++)
            {
                  Uoben[a][b] = addedBlock->u[a][b];
            }
      }

      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->mDim/2; b++)
            {
                  Uunten[a][b] = addedBlock->u[a][b + addedBlock->mDim/2];
            }
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  Vlinks[a][b] = addedBlock->v[a][b];
            }
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  Vrechts[a][b] = addedBlock->v[a + addedBlock->nDim/2][b];
            }
      }

      OuterProductBlock<datatype>* newA = new OuterProductBlock<datatype>();

      newA->nDim = this->nDim/2;
      newA->mDim = this->mDim/2;
      newA->k = addedBlock->k;
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->mDim/2; b++)
            {
                  newA->u[a][b] = Uoben[a][b];
            }
      }
      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newA->v[a][b] = Vlinks[a][b];
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

      newB->nDim = this->nDim/2;
      newB->mDim = this->mDim/2;
      newB->k = addedBlock->k;
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->mDim/2; b++)
            {
                  newB->u[a][b] = Uoben[a][b];
            }
      }
      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newB->u[a][b] = Vrechts[a][b];
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

      newC->nDim = this->nDim/2;
      newC->mDim = this->mDim/2;
      newC->k = addedBlock->k;
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->mDim/2; b++)
            {
                  newC->u[a][b] = Uunten[a][b];
            }
      }
      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newC->u[a][b] = Vlinks[a][b];
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

      newD->nDim = this->nDim/2;
      newD->mDim = this->mDim/2;
      newD->k = addedBlock->k;
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->mDim/2; b++)
            {
                  newD->u[a][b] = Uunten[a][b];
            }
      }
      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newD->u[a][b] = Vrechts[a][b];
            }
      }
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newD->x[a][b] = addedBlock->x[a][b];
            }
      }

      HierarchicalMatrix<datatype>* temp = new HierarchicalMatrix<datatype>();

      temp->nDim = this->nDim;
      temp->mDim = this->mDim;

      for(a = 0; a < 2; a++){
            for(b = 0; b < 2; b++){
                  temp->indiceRange[a][b] = this->indiceRange[a][b];
            }
      }
      return temp; 

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

      for(a=0; a < addedBlock->mDim/2; a++){
            delete[] Uoben[a];
      }
      delete[] Uoben;
      delete newA;

      for(a=0; a < addedBlock->mDim/2; a++){
            delete[] Uunten[a];
      }
      delete[] Uunten;
      delete newB;

      for(a=0; a < addedBlock->k; a++){
            delete[] Vlinks[a];
      }
      delete[] Vlinks;
      delete newC;

      for(a=0; a < addedBlock->k; a++){
            delete[] Vrechts[a];
      }
      delete[] Vrechts;
      delete newD;

      return temp;
}

template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){ // FERTIG? Ind Vectoren für Add und Mult nicht benötigt -> problem wenn wegfallen?
      // HM + EW

      unsigned int a, b;

      EntrywiseBlock<datatype>* newA = new EntrywiseBlock<datatype>();
      newA->nDim = this->nDim/2;
      newA->mDim = this->mDim/2;
      EntrywiseBlock<datatype>* newB = new EntrywiseBlock<datatype>();
      newB->nDim = this->nDim/2;
      newB->mDim = this->mDim/2;
      EntrywiseBlock<datatype>* newC = new EntrywiseBlock<datatype>();
      newC->nDim = this->nDim/2;
      newC->mDim = this->mDim/2;
      EntrywiseBlock<datatype>* newD = new EntrywiseBlock<datatype>();
      newD->nDim = this->nDim/2;
      newD->mDim = this->mDim/2;


      datatype** A = new datatype*[addedBlock->nDim/2];
      for(a=0; a < addedBlock->nDim/2; a++){
            A[a] = new datatype[addedBlock->mDim/2];
      }
      datatype** B = new datatype*[addedBlock->nDim/2];
      for(a=0; a < addedBlock->nDim/2; a++){
            B[a] = new datatype[addedBlock->mDim/2];
      }
      datatype** C = new datatype*[addedBlock->nDim/2];
      for(a=0; a < addedBlock->nDim/2; a++){
            C[a] = new datatype[addedBlock->mDim/2];
      }
      datatype** D = new datatype*[addedBlock->nDim/2];
      for(a=0; a < addedBlock->nDim/2; a++){
            D[a] = new datatype[addedBlock->mDim/2];
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  A[a][b] = addedBlock->block[a][b];
            }
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  B[a][b] = addedBlock->block[a][b + addedBlock->nDim/2];
            }
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  C[a][b] = addedBlock->block[a + addedBlock->nDim/2][b];
            }
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  D[a][b] = addedBlock->block[a + addedBlock->nDim/2][b + addedBlock->nDim/2];
            }
      }

      ///////////////////////////////////////////////

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  newA->block[a][b] = A[a][b];
            }
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  newB->block[a][b] = B[a][b];
            }
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  newC->block[a][b] = C[a][b];
            }
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  newD->block[a][b] = D[a][b];
            }
      }

      HierarchicalMatrix<datatype>* temp = new HierarchicalMatrix<datatype>();

      temp->nDim = this->nDim;
      temp->mDim = this->mDim;

      for(a = 0; a < 2; a++){
            for(b = 0; b < 2; b++){
                  temp->indiceRange[a][b] = this->indiceRange[a][b];
            }
      }
      return temp; 

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

      delete newA;
      delete newB;
      delete newC;
      delete newD;
      for(a=0; a < dim; a++){
            delete[] tempU[addedBlock->mDim];
      }
      delete[] tempU;
      for(a=0; a < dim; a++){
            delete[] tempU[addedBlock->mDim];
      }
      delete[] tempU;
      for(a=0; a < dim; a++){
            delete[] tempU[addedBlock->mDim];//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      }
      delete[] tempU;
      for(a=0; a < dim; a++){
            delete[] tempU[addedBlock->mDim];
      }
      delete[] tempU;

      return temp;
}


// OuterProductBlock --> Noch 0 Funktionen offen
template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( Block<datatype>* addedBlock ){ // FERTIG
      // std::cout << " OP+?? ";

      return *addedBlock + this;
}

template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( HierarchicalMatrix<datatype>* addedBlock ){ // FERTIG
      // OP + HM == HM + OP | call function with swapped parameters
      return *addedBlock + this;
}

template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( OuterProductBlock<datatype>* addedBlock ){ // FERTIG? Benötigt Segen von Marie evtl Probleme wegen new und löschen
      // std::cout << " OP+OP ";

      // OP + OP
      unsigned int a = 0;
      unsigned int b = 0;

      OuterProductBlock<datatype>* temp = new OuterProductBlock<datatype>();

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

      // tempu befüllen
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
                  tempU[a + k][b] = addedBlock->u[a][b];
            }
      }

      // tempv befüllen
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

      // tempx befüllen
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

      // u befüllen
      for(a = 0; a < (k + addedBlock->k); a++)
      {
            for(b = 0; b < addedBlock->mDim; b++)
            {
                  temp->u[a][b] = tempU[a][b];
            }
      }
     

      // v befüllen
      for(a = 0; a < addedBlock->nDim; a++)
      {
            for(b = 0; b < (k + addedBlock->k); b++)
            {
                  temp->v[a][b] = v[a][b];
            }
      }
      
      // x befüllen
      for(a = 0; a < (k + addedBlock->k); a++)
      {
            for(b = 0; b < (k + addedBlock->k); b++)
            {
                  temp->x[a][b] = x[a][b];
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

      for(a=0; a < dim; a++){
            delete[] tempU[addedBlock->mDim];
      }
      delete[] tempU;
      for(a=0; a < dim; a++){
            delete[] tempX[addedBlock->k + k]; ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      }
      delete[] tempX;
      for(a=0; a < dim; a++){
            delete[] tempV[addedBlock->k + k];
      }
      delete[] tempV;

      return temp;
}

template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){ // fast FERTIG? Löschen? k? 
      // OP + EW

      unsigned int newk = calcRank(addedBlock->mDim, addedBlock->nDim, addedBlock->block);  

      std::vector<unsigned int> iInd;
      iInd.push_back(this->indiceRange[kRangeI][kBottom]);
      iInd.push_back(this->indiceRange[kRangeI][kTop]);

      std::vector<unsigned int> jInd;
      jInd.push_back(this->indiceRange[kRangeJ][kBottom]);
      jInd.push_back(this->indiceRange[kRangeJ][kTop]);

      OuterProductBlock<datatype>* temp = new OuterProductBlock<datatype>(addedBlock->block, addedBlock->mDim, addedBlock->nDim, iInd, jInd, 7); 

      return *this + temp;
}


// EntrywiseBlock --> Noch 0 Funktion offen
template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::operator+( Block<datatype>* addedBlock ){ // FERTIG
      return *addedBlock + this;
}

template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::operator+( HierarchicalMatrix<datatype>* addedBlock ){ // FERTIG
      // EW + HM == HM + EW | call function with swapped parameters
      return *addedBlock + this;
}

template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::operator+( OuterProductBlock<datatype>* addedBlock ){ // FERTIG
      // EW + OP == OP + EW | call function with swapped parameters
      return *addedBlock + this;
}

template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){ //FERTIG
      // EW + EW

      EntrywiseBlock<datatype>* temp = new EntrywiseBlock<datatype>();

      temp->nDim = this->nDim;
      temp->mDim = this->mDim;

      for(unsigned int a = 0; a < addedBlock->mDim; a++)
      {
            for(unsigned int b = 0; b < addedBlock->nDim; b++)
            {
                  temp->block[a][b] = block[a][b] + addedBlock->block[a][b];
            }
      }

      for(unsigned int a = 0; a < 2; a++){
            for(unsigned int b = 0; b < 2; b++){
                  temp->indiceRange[a][b] = this->indiceRange[a][b];
            }
      }
      return temp; 
}
