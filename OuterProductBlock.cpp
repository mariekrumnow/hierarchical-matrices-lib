#include "OuterProductBlock.hpp"

template <class datatype, unsigned int xDim, unsigned int yDim>
OuterProductBlock<datatype, xDim, yDim>::OuterProductBlock(const datatype originalBlock[xDim][yDim], int I, int J)
{

}

template <class datatype, unsigned int xDim, unsigned int yDim>
Block<datatype, xDim, yDim>& OuterProductBlock<datatype, xDim, yDim>::coarse(){

}

//---------------------------------------------------------------------------------------

template <class datatype, unsigned int xDim, unsigned int yDim>
OuterProductBlock<datatype, xDim, yDim>& OuterProductBlock<datatype, xDim, yDim>::operator+( const OuterProductBlock<datatype, xDim, yDim>& addedBlock ){

}

template <class datatype, unsigned int xDim, unsigned int yDim>
OuterProductBlock<datatype, xDim, yDim>& OuterProductBlock<datatype, xDim, yDim>::operator+( const EntrywiseBlock<datatype, xDim, yDim>& addedBlock ){

}

template <class datatype, unsigned int xDim, unsigned int yDim>
OuterProductBlock<datatype, xDim, yDim>& OuterProductBlock<datatype, xDim, yDim>::operator+( const HierarchicalMatrix<datatype, xDim>& addedBlock ){

}

//---------------------------------------------------------------------------------------

template <class datatype, unsigned int xDim, unsigned int yDim>
OuterProductBlock<datatype, xDim, yDim>& OuterProductBlock<datatype, xDim, yDim>::operator*( const OuterProductBlock<datatype, xDim, yDim>& multBlock ){

}

template <class datatype, unsigned int xDim, unsigned int yDim>
OuterProductBlock<datatype, xDim, yDim>& OuterProductBlock<datatype, xDim, yDim>::operator*( const EntrywiseBlock<datatype, xDim, yDim>& multBlock ){

}

template <class datatype, unsigned int xDim, unsigned int yDim>
OuterProductBlock<datatype, xDim, yDim>& OuterProductBlock<datatype, xDim, yDim>::operator*( const HierarchicalMatrix<datatype, xDim>& multBlock ){

}
