#ifndef HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
#define HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H

#include "Block.hpp"

#include <list>
#include <vector>


/// Depicts a non-admissible or full rank part of the original matrix
template <class datatype>
class EntrywiseBlock: public Block<datatype>{

protected:
      datatype ** block; ///< mDim * nDim array / Exact part of the original matrix

public:
      /// Copies part of the matrix to be stored in a block
      EntrywiseBlock(datatype ** originalBlock, unsigned int mDim, unsigned int nDim, std::vector<unsigned int> iInd, std::vector<unsigned int> jInd);

      Block<datatype>* coarse( double accuracy, bool checkForLeaf ) final;
      unsigned int getStorage() final;

      datatype* operator*( const datatype vector[] );

      // Block<datatype>& operator*( const Block<datatype>& multBlock );

      Block<datatype>* operator+( Block<datatype>* addedBlock );
      Block<datatype>* operator+( HierarchicalMatrix<datatype>* addedBlock );
      Block<datatype>* operator+( OuterProductBlock<datatype>* addedBlock );
      Block<datatype>* operator+( EntrywiseBlock* addedBlock );

      /// Frees all memory allocated for the block array
      ~EntrywiseBlock();
};

#endif // HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
