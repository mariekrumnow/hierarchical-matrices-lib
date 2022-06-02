#include "EntrywiseBlock.hpp"

template <class datatype>
EntrywiseBlock<datatype>::EntrywiseBlock(datatype ** originalBlock, unsigned int yDim, unsigned int xDim /*, List-container*/)
      :Block<datatype>::Block(xDim, yDim), block(originalBlock)
{
      // Sonst nix mehr nötig zu machen, muss ja nur 1:1 eingespeichert werden
}

template <class datatype>
Block<datatype>& EntrywiseBlock<datatype>::coarse(){

}

//---------------------------------------------------------------------------------------

template <class datatype>
Block<datatype>& EntrywiseBlock<datatype>::operator+( const Block<datatype>& addedBlock ){

}

//---------------------------------------------------------------------------------------

template <class datatype>
Block<datatype>& EntrywiseBlock<datatype>::operator*( const Block<datatype>& multBlock ){

}
