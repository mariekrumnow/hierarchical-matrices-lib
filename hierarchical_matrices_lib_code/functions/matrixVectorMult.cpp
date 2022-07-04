#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

#include <iostream> // For testing

// Bitte nicht mehr die Funktionsköpfe ändern oder falls nötig Bescheid geben!

// Eingabevektor hat Größe Block<datatype>::nDim
// Ausgabevektor hat Größe Block<datatype>::mDim


// HierarchicalMatrix
template <class datatype>
datatype* HierarchicalMatrix<datatype>::operator*( const datatype vector[] ){
      // std::cout << " HM*vector ";
      datatype* result = new datatype[Block<datatype>::mDim];
      for(int a=0; a<2; a++){
            for(int b=0; b<2; b++){
                  *matrix[a][b] * vector; // Nicht richtig so, Vektor muss gekürzt werden, damit er mit der nDim von matrix übereinstimmt, aber so geht der Aufruf
            }
      }
      return result;
}


// OuterProductBlock
template <class datatype>
datatype* OuterProductBlock<datatype>::operator*( const datatype vector[] ){
      // std::cout << " OP*vector ";
      datatype* result = new datatype[Block<datatype>::mDim];
      return result;
}


// EntrywiseBlock
template <class datatype>
datatype* EntrywiseBlock<datatype>::operator*( const datatype vector[] ){
      // std::cout << " EW*vector ";
      datatype* result = new datatype[Block<datatype>::mDim];
      return result;
}
