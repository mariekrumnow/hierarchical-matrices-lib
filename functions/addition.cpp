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

      for(int a=0; a<2; a++){
            for(int b=0; b<2; b++){
                  *matrix[a][b] + addedMatrix.matrix[a][b];
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

      matrix[0][0] = martix[0][0] + addedBlock.martix[0][0];
      matrix[0][1] = martix[0][1] + addedBlock.martix[0][1];
      matrix[1][0] = martix[1][0] + addedBlock.martix[1][0];
      matrix[1][1] = martix[1][1] + addedBlock.martix[1][1];

      return this;
}

template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( OuterProductBlock<datatype>* addedBlock ){
      // HM + OP

      matrix[0][0] = martix[0][0] + newA;
      matrix[0][1] = martix[0][1] + newB;
      matrix[1][0] = martix[1][0] + newC;
      matrix[1][1] = martix[1][1] + newD;

      return this;
}

template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){ // FERTIG? Ind Vectoren für Add und Mult nicht benötigt -> problem wenn wegfallen?
      // HM + EW
      datatype* BlockA = new datatype[addedBlock.nDim/2 * addedBlock.mDim/2];
      datatype* BlockB = new datatype[addedBlock.nDim/2 * addedBlock.mDim/2];
      datatype* BlockC = new datatype[addedBlock.nDim/2 * addedBlock.mDim/2];
      datatype* BlockD = new datatype[addedBlock.nDim/2 * addedBlock.mDim/2];

      int a, b;

      for(a = 0; a < addedBlock.nDim/2; a++)
      {
            for(b = 0; b < addedBlock.nDim/2; b++)
            {
                  BlockA[a][b] = addedBlock.block[a][b];
            }
      }

      for(a = 0; a < addedBlock.nDim/2; a++)
      {
            for(b = 0; b < addedBlock.nDim/2; b++)
            {
                  BlockB[a][b] = addedBlock.block[a][b + addedBlock.nDim/2];
            }
      }

      for(a = 0; a < addedBlock.nDim/2; a++)
      {
            for(b = 0; b < addedBlock.nDim/2; b++)
            {
                  BlockC[a][b] = addedBlock.block[a + addedBlock.nDim/2][b];
            }
      }

      for(a = 0; a < addedBlock.nDim/2; a++)
      {
            for(b = 0; b < addedBlock.nDim/2; b++)
            {
                  BlockD[a][b] = addedBlock.block[a + addedBlock.nDim/2][b + addedBlock.nDim/2];
            }
      }

      newA = new EntrywiseBlock<datatype>(BlockA, addedBlock.mDim/2, addedBlock.nDim/2, NULL, NULL);
      newB = new EntrywiseBlock<datatype>(BlockB, addedBlock.mDim/2, addedBlock.nDim/2, NULL, NULL);
      newC = new EntrywiseBlock<datatype>(BlockC, addedBlock.mDim/2, addedBlock.nDim/2, NULL, NULL);
      newD = new EntrywiseBlock<datatype>(BlockD, addedBlock.mDim/2, addedBlock.nDim/2, NULL, NULL);


      matrix[0][0] = martix[0][0] + newA;
      matrix[0][1] = martix[0][1] + newB;
      matrix[1][0] = martix[1][0] + newC;
      matrix[1][1] = martix[1][1] + newD;

      delete BlockA;
      delete newA;
      delete BlockB;
      delete newB;
      delete BlockC;
      delete newC;
      delete BlockD;
      delete newD;

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
      int a = 0;
      int b = 0;
      datatype* newu = new datatype[mDim * (k + addedBlock.k)];
      datatype* newx = new datatype[(k + addedBlock.k) * (k + addedBlock.k)];
      datatype* newv = new datatype[(k + addedBlock.k) * nDim];

      // u befüllen
      for(a = 0; a < k; a++)
      {
            for(b = 0; b < mDim; b++)
            {
                  newu[a][b] = u[a][b];
            }
      }
      for(a = 0; a < addedBlock.k; a++)
      {
            for(b = 0; b < mDim; b++)
            {
                  newu[a + k][b] = addedBlock.u[a][b];
            }
      }

      // v befüllen
      for(a = 0; a < nDim; a++)
      {
            for(b = 0; b < k; b++)
            {
                  newv[a][b] = v[a][b];
            }
      }
      for(a = 0; a < nDim; a++)
      {
            for(b = 0; b < addedBlock.k; b++)
            {
                  newu[a][b + k] = addedBlock.u[a][b];
            }
      }

      // x befüllen
      for(a = 0; a < k; a++)
      {
            for(b = 0; b < k; b++)
            {
                  newx[a][b] = x[a][b];
            }
      }
      for(a = 0; a < addedBlock.k; a++)
      {
            for(b = 0; b < addedBlock.k; b++)
            {
                  newu[a + k][b + k] = addedBlock.u[a][b];
            }
      }

      &u = &newu;
      &v = &newv;
      &x = &newx;
      k = k + addedBlock.k;

      return this;
}

template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){ // fast FERTIG? Löschen? k? 
      // OP + EW

      unsigned int newk;  // Berechnung von Construkter klauen? Marie?

      temp = new OuterProductBlock<datatype>(addedBlock.block, addedBlock.mDim, addedBlock.nDim, *addedBlock.iInd, *addedBlock.jInd, newk);

      return this + temp;
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

      for(int a = 0; a < mDim; a++)
      {
            for(int b = 0; b < nDim; b++)
            {
                  block[a][b] = block[a][b] + addedBlock.block[a][b];
            }
      }

      return this;
}
