#include "EntrywiseBlock.hpp"

template <class datatype, unsigned int xDim, unsigned int yDim>
EntrywiseBlock<datatype, xDim, yDim>::EntrywiseBlock(datatype( &originalBlock)[xDim][yDim], unsigned int I, unsigned int J)
      :Block<datatype, xDim, yDim>::Block(I, J), block(originalBlock)
{
      // Sonst nix mehr nötig zu machen, muss ja nur 1:1 eingespeichert werden
}

template <class datatype, unsigned int xDim, unsigned int yDim>
Block<datatype, xDim, yDim>& EntrywiseBlock<datatype, xDim, yDim>::coarse(){

}

//---------------------------------------------------------------------------------------

template <class datatype, unsigned int xDim, unsigned int yDim>
EntrywiseBlock<datatype, xDim, yDim>& EntrywiseBlock<datatype, xDim, yDim>::operator+( const EntrywiseBlock<datatype, xDim, yDim>& addedBlock ){

}

template <class datatype, unsigned int xDim, unsigned int yDim>
EntrywiseBlock<datatype, xDim, yDim>& EntrywiseBlock<datatype, xDim, yDim>::operator+( const OuterProductBlock<datatype, xDim, yDim>& addedBlock ){

}

template <class datatype, unsigned int xDim, unsigned int yDim>
EntrywiseBlock<datatype, xDim, yDim>& EntrywiseBlock<datatype, xDim, yDim>::operator+( const HierarchicalMatrix<datatype, xDim>& addedBlock ){

}

//---------------------------------------------------------------------------------------

template <class datatype, unsigned int xDim, unsigned int yDim>
EntrywiseBlock<datatype, xDim, yDim>& EntrywiseBlock<datatype, xDim, yDim>::operator*( const EntrywiseBlock<datatype, xDim, yDim>& multBlock ){

}

template <class datatype, unsigned int xDim, unsigned int yDim>
EntrywiseBlock<datatype, xDim, yDim>& EntrywiseBlock<datatype, xDim, yDim>::operator*( const OuterProductBlock<datatype, xDim, yDim>& multBlock ){

}

template <class datatype, unsigned int xDim, unsigned int yDim>
EntrywiseBlock<datatype, xDim, yDim>& EntrywiseBlock<datatype, xDim, yDim>::operator*( const HierarchicalMatrix<datatype, xDim>& multBlock ){

}
