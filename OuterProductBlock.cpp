#include "OuterProductBlock.hpp"

template <class datatype, unsigned int xDim, unsigned int yDim>
OuterProductBlock<datatype, xDim, yDim>::OuterProductBlock(const datatype originalBlock[xDim][yDim], unsigned int I, unsigned int J)
      :Block<datatype, xDim, yDim>::Block(I, J)
{
      // k = 2; // Rang k ermitteln

      for(unsigned int i=0; i< xDim; i++){
            u[i] = new datatype[k];
            // u[i][0] = originalBlock[i][0];
      }

      for(unsigned int i=0; i< yDim; i++){
            v[i] = new datatype[k];
            // v[i][1] = originalBlock[0][i];
      }
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
