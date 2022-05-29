#ifndef HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
#define HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H

#include "Block.hpp"


template <class datatype, unsigned int dim> class HierarchicalMatrix;
template <class datatype, unsigned int xDim, unsigned int yDim> class OuterProductBlock;


/// Depicts a non-admissible part
template <class datatype, unsigned int xDim, unsigned int yDim>
class EntrywiseBlock: public Block<datatype, xDim, yDim>{

protected:
      datatype (&block)[xDim][yDim]; ///< Exact part of the original matrix

public:
      /// Copies a matrix to be stored in a block
      ///
      /// \param originalBlock
      /// \param I
      /// \param J
      EntrywiseBlock(datatype ( &originalBlock)[xDim][yDim], unsigned int I, unsigned int J);

      ///
      ///
      /// \return
      Block<datatype, xDim, yDim>& coarse() final;

      //---------------------------------------------------------------------------------------

      EntrywiseBlock& operator+( const EntrywiseBlock& addedBlock );
      EntrywiseBlock& operator+( const OuterProductBlock<datatype, xDim, yDim>& addedBlock );
      EntrywiseBlock& operator+( const HierarchicalMatrix<datatype, xDim>& addedBlock );

      EntrywiseBlock& operator*( const EntrywiseBlock& multBlock );
      EntrywiseBlock& operator*( const OuterProductBlock<datatype, xDim, yDim>& multBlock );
      EntrywiseBlock& operator*( const HierarchicalMatrix<datatype, xDim>& multBlock );
};

#endif // HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
