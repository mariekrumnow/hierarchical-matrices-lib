#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

// HierarchicalMatrix
template <class datatype>
Block<datatype>& HierarchicalMatrix<datatype>::operator+( const Block<datatype>& addedMatrix ){

}

// HierarchicalMatrix
template <class datatype>
Block<datatype>& HierarchicalMatrix<datatype>::operator+=( const HierarchicalMatrix<datatype>& addedMatrix ){

}


// OuterProductBlock
template <class datatype>
Block<datatype>& OuterProductBlock<datatype>::operator+( const Block<datatype>& addedBlock ){

}


// EntrywiseBlock
template <class datatype>
Block<datatype>& EntrywiseBlock<datatype>::operator+( const Block<datatype>& addedBlock ){

}
