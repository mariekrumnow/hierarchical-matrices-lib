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
      std::cout << " HM+HM ";

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

// HierarchicalMatrix: private functions --> Noch 3 Funktionen offen
template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( Block<datatype>* addedBlock ){ // FERTIG
      return *addedBlock + this;
}

template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( HierarchicalMatrix<datatype>* addedBlock ){
      // HM + HM | Hier vllt auch nur wieder die public Funktion aufrufen?
}

template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( OuterProductBlock<datatype>* addedBlock ){
      // HM + OP
}

template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){
      // HM + EW
}


// OuterProductBlock --> Noch 2 Funktionen offen
template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( Block<datatype>* addedBlock ){ // FERTIG
      std::cout << " OP+?? ";

      return *addedBlock + this;
}

template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( HierarchicalMatrix<datatype>* addedBlock ){ // FERTIG
      // OP + HM == HM + OP | call function with swapped parameters
      return *addedBlock + this;
}

template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( OuterProductBlock<datatype>* addedBlock ){
      std::cout << " OP+OP ";

      // OP + OP
}

template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){
      // OP + EW
}


// EntrywiseBlock --> Noch 1 Funktion offen
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
Block<datatype>* EntrywiseBlock<datatype>::operator+( EntrywiseBlock<datatype>* addedBlock ){
      // EW + EW
}
