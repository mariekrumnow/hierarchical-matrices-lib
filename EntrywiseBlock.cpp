#include "EntrywiseBlock.hpp"

template <class datatype, unsigned int dim>
EntrywiseBlock<datatype, dim>::EntrywiseBlock(datatype originalBlock[dim][dim], int I, int J)
      :block(originalBlock)
{

}

template <class datatype, unsigned int dim>
Block<datatype, dim>& EntrywiseBlock<datatype, dim>::coarse(){

}

//Verrechnungfkten
