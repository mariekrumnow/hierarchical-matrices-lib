#include "EntrywiseBlock.hpp"

template <class datatype>
EntrywiseBlock<datatype>::EntrywiseBlock(datatype( &originalBlock)[xDim][yDim])
      :Block<datatype>::Block(), block(originalBlock)
{
      // Sonst nix mehr nötig zu machen, muss ja nur 1:1 eingespeichert werden
}

template <class datatype>
Block<datatype>& EntrywiseBlock<datatype>::coarse(){

}

//---------------------------------------------------------------------------------------

template <class datatype>
EntrywiseBlock<datatype>& EntrywiseBlock<datatype>::operator+( const EntrywiseBlock<datatype>& addedBlock ){

}

template <class datatype>
EntrywiseBlock<datatype>& EntrywiseBlock<datatype>::operator+( const OuterProductBlock<datatype>& addedBlock ){

}

template <class datatype>
EntrywiseBlock<datatype>& EntrywiseBlock<datatype>::operator+( const HierarchicalMatrix<datatype>& addedBlock ){

}

//---------------------------------------------------------------------------------------

template <class datatype>
EntrywiseBlock<datatype>& EntrywiseBlock<datatype>::operator*( const EntrywiseBlock<datatype>& multBlock ){

}

template <class datatype>
EntrywiseBlock<datatype>& EntrywiseBlock<datatype>::operator*( const OuterProductBlock<datatype>& multBlock ){

}

template <class datatype>
EntrywiseBlock<datatype>& EntrywiseBlock<datatype>::operator*( const HierarchicalMatrix<datatype>& multBlock ){

}
