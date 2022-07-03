#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

#include <iostream> // For testing

// Bitte nicht mehr die Funktionsköpfe ändern oder falls nötig BEscheid geben!

// Reihenfolge der Funktionen: HM > OP > EW

// Da bei Addition die Reihenfolge egal ist, werden in folgenden nur die umgedrehte Funktion aufgerufen:
// EW + HM / EW + OP / OP + HM


// HierarchicalMatrix: public functions --> Noch alle (2) Funktionen offen
template <class datatype>
HierarchicalMatrix<datatype>* HierarchicalMatrix<datatype>::operator+( const HierarchicalMatrix<datatype>& addedMatrix ){
      // std::cout << " HM+HM ";

      for(unsigned int a=0; a<2; a++){
            for(unsigned int b=0; b<2; b++){
                  matrix[a][b] = matrix[a][b] + addedMatrix->matrix[a][b];
            }
      }
      return this; // Ist noch falsch
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
Block<datatype>* HierarchicalMatrix<datatype>::operator+( HierarchicalMatrix<datatype>* addedBlock ){ // FERTIG
      // HM + HM | Hier vllt auch nur wieder die public Funktion aufrufen?

      matrix[0][0] = *matrix[0][0] + addedBlock->matrix[0][0];
      matrix[0][1] = *matrix[0][1] + addedBlock->matrix[0][1];
      matrix[1][0] = *matrix[1][0] + addedBlock->matrix[1][0];
      matrix[1][1] = *matrix[1][1] + addedBlock->matrix[1][1];

      return this;
}

template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( OuterProductBlock<datatype>* addedBlock ){
      // HM + OP

      // matrix[0][0] = matrix[0][0] + newA;
      // matrix[0][1] = matrix[0][1] + newB;
      // matrix[1][0] = matrix[1][0] + newC;
      // matrix[1][1] = matrix[1][1] + newD;

      return this;
}

template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){ // FERTIG? Ind Vectoren für Add und Mult nicht benötigt -> problem wenn wegfallen?
      // HM + EW

      unsigned int a, b;


      datatype** BlockA = new datatype*[addedBlock->nDim/2];
      for(a=0; a < addedBlock->nDim/2; a++){
            BlockA[a] = new datatype[addedBlock->mDim/2];
      }
      datatype** BlockB = new datatype*[addedBlock->nDim/2];
      for(a=0; a < addedBlock->nDim/2; a++){
            BlockB[a] = new datatype[addedBlock->mDim/2];
      }
      datatype** BlockC = new datatype*[addedBlock->nDim/2];
      for(a=0; a < addedBlock->nDim/2; a++){
            BlockA[a] = new datatype[addedBlock->mDim/2];
      }
      datatype** BlockD = new datatype*[addedBlock->nDim/2];
      for(a=0; a < addedBlock->nDim/2; a++){
            BlockA[a] = new datatype[addedBlock->mDim/2];
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  BlockA[a][b] = addedBlock->block[a][b];
            }
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  BlockB[a][b] = addedBlock->block[a][b + addedBlock->nDim/2];
            }
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  BlockC[a][b] = addedBlock->block[a + addedBlock->nDim/2][b];
            }
      }

      for(a = 0; a < addedBlock->nDim/2; a++)
      {
            for(b = 0; b < addedBlock->nDim/2; b++)
            {
                  BlockD[a][b] = addedBlock->block[a + addedBlock->nDim/2][b + addedBlock->nDim/2];
            }
      }

      EntrywiseBlock<datatype>* newA = new EntrywiseBlock<datatype>();
      EntrywiseBlock<datatype>* newB = new EntrywiseBlock<datatype>();
      EntrywiseBlock<datatype>* newC = new EntrywiseBlock<datatype>();
      EntrywiseBlock<datatype>* newD = new EntrywiseBlock<datatype>();


      matrix[0][0] = *matrix[0][0] + newA;
      matrix[0][1] = *matrix[0][1] + newB;
      matrix[1][0] = *matrix[1][0] + newC;
      matrix[1][1] = *matrix[1][1] + newD;

      delete BlockA;
      delete &newA;
      delete BlockB;
      delete &newB;
      delete BlockC;
      delete &newC;
      delete BlockD;
      delete &newD;

      return this;
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
      datatype** newU = new datatype*[addedBlock->mDim];
      for(a=0; a < addedBlock->mDim; a++){
            newU[a] = new datatype[k + addedBlock->k];
      }
      datatype** newX = new datatype*[k + addedBlock->k];
      for(a=0; a < (k + addedBlock->k); a++){
            newX[a] = new datatype[k + addedBlock->k];
      }
      datatype** newV = new datatype*[(k + addedBlock->k)];
      for(a=0; a < (k + addedBlock->k); a++){
            newV[a] = new datatype[addedBlock->nDim];
      }

      // u befüllen
      for(a = 0; a < k; a++)
      {
            for(b = 0; b < addedBlock->mDim; b++)
            {
                  newU[a][b] = u[a][b];
            }
      }
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->mDim; b++)
            {
                  newU[a + k][b] = addedBlock->u[a][b];
            }
      }

      // v befüllen
      for(a = 0; a < addedBlock->nDim; a++)
      {
            for(b = 0; b < k; b++)
            {
                  newV[a][b] = v[a][b];
            }
      }
      for(a = 0; a < addedBlock->nDim; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newU[a][b + k] = addedBlock->u[a][b];
            }
      }

      // x befüllen
      for(a = 0; a < k; a++)
      {
            for(b = 0; b < k; b++)
            {
                  newX[a][b] = x[a][b];
            }
      }
      for(a = 0; a < addedBlock->k; a++)
      {
            for(b = 0; b < addedBlock->k; b++)
            {
                  newU[a + k][b + k] = addedBlock->u[a][b];
            }
      }

      u = newU;
      v = newV;
      x = newX;
      k = k + addedBlock->k;

      return this;
}

template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){ // fast FERTIG? Löschen? k? 
      // OP + EW

      unsigned int newk;  // Berechnung von Construkter klauen? Marie? Wird outgesourced!

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

      for(unsigned int a = 0; a < addedBlock->mDim; a++)
      {
            for(unsigned int b = 0; b < addedBlock->nDim; b++)
            {
                  block[a][b] = block[a][b] + addedBlock->block[a][b];
            }
      }

      return this;
}
