#ifndef HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
#define HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H

#include "Block.hpp"


template <class datatype> class HierarchicalMatrix;
template <class datatype> class OuterProductBlock;


/// Depicts a non-admissible part
template <class datatype>
class EntrywiseBlock: public Block<datatype>{

protected:
      datatype (&block)[xDim][yDim]; ///< Exact part of the original matrix

public:
      /// Copies a matrix to be stored in a block
      ///
      /// \param originalBlock
      /// \param I
      /// \param J
      EntrywiseBlock(datatype ( &originalBlock)[xDim][yDim]);

      ///
      ///
      /// \return
      Block<datatype>& coarse() final;

      //---------------------------------------------------------------------------------------

      EntrywiseBlock& operator+( const EntrywiseBlock& addedBlock );
      EntrywiseBlock& operator+( const OuterProductBlock<datatype>& addedBlock );
      EntrywiseBlock& operator+( const HierarchicalMatrix<datatype>& addedBlock );

      EntrywiseBlock& operator*( const EntrywiseBlock& multBlock );
      EntrywiseBlock& operator*( const OuterProductBlock<datatype>& multBlock );
      EntrywiseBlock& operator*( const HierarchicalMatrix<datatype>& multBlock );
};

#endif // HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
