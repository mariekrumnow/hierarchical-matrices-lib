#ifndef HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
#define HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H

#include "Block.hpp"

#include <list>
#include <vector>

template <class datatype> class HierarchicalMatrix;
template <class datatype> class OuterProductBlock;


/// Depicts a non-admissible part
template <class datatype>
class EntrywiseBlock: public Block<datatype>{

protected:
      datatype ** block; ///< mDim * nDim array / Exact part of the original matrix

public:
      /// Copies a matrix to be stored in a block
      ///
      /// \param
      EntrywiseBlock(datatype ** originalBlock, unsigned int mDim, unsigned int nDim, std::vector<unsigned int> iInd, std::vector<unsigned int> jInd);

      ~EntrywiseBlock();

      ///
      ///
      /// \return
      Block<datatype>& coarse() final;

      //---------------------------------------------------------------------------------------

      Block<datatype>& operator+( const Block<datatype>& addedBlock );
      Block<datatype>& operator*( const Block<datatype>& multBlock );
};

#endif // HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
