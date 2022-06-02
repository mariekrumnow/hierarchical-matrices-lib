#ifndef HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
#define HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H

#include "Block.hpp"


template <class datatype> class HierarchicalMatrix;
template <class datatype> class OuterProductBlock;


/// Depicts a non-admissible part
template <class datatype>
class EntrywiseBlock: public Block<datatype>{

protected:
      datatype ** block; ///< xDim * yDim array / Exact part of the original matrix

public:
      /// Copies a matrix to be stored in a block
      ///
      /// \param originalBlock
      /// \param I
      /// \param J
      EntrywiseBlock(datatype ** originalBlock, unsigned int yDim, unsigned int xDim /*, List-container*/);

      ///
      ///
      /// \return
      Block<datatype>& coarse() final;

      //---------------------------------------------------------------------------------------

      Block<datatype> operator+( const EntrywiseBlock& addedBlock );
      Block<datatype> operator+( const OuterProductBlock<datatype>& addedBlock );
      Block<datatype> operator+( const HierarchicalMatrix<datatype>& addedBlock );

      Block<datatype> operator*( const EntrywiseBlock& multBlock );
      Block<datatype> operator*( const OuterProductBlock<datatype>& multBlock );
      Block<datatype> operator*( const HierarchicalMatrix<datatype>& multBlock );
};

#endif // HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
