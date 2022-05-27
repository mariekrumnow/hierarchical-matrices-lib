#include "EntrywiseBlock.hpp"

template <class datatype, unsigned int xDim, unsigned int yDim>
EntrywiseBlock<datatype, xDim, yDim>::EntrywiseBlock(datatype originalBlock[xDim][yDim], int I, int J)
      :block(originalBlock)
{

}

template <class datatype, unsigned int xDim, unsigned int yDim>
Block<datatype, xDim, yDim>& EntrywiseBlock<datatype, xDim, yDim>::coarse(){

}

//Verrechnungfkten
