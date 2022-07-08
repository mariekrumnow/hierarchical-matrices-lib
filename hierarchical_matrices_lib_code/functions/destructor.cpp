#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"


// HierarchicalMatrix
template <class datatype>
HierarchicalMatrix<datatype>::~HierarchicalMatrix(){
      delete matrix[0][0];
      delete matrix[0][1];
      delete matrix[1][0];
      delete matrix[1][1];
}


// OuterProductBlock
template <class datatype>
OuterProductBlock<datatype>::~OuterProductBlock(){
      unsigned int i=0;
      for(; i < Block<datatype>::mDim; i++){
        delete[] u[i];
      }

      for(i=0; i < k; i++){
        delete[] x[i];
      }

      for(i=0; i < Block<datatype>::nDim; i++){
        delete[] v[i];
      }
}


// EntrywiseBlock
template <class datatype>
EntrywiseBlock<datatype>::~EntrywiseBlock(){
      for(unsigned int i=0; i < Block<datatype>::mDim; i++){
        delete[] block[i];
      }
}
