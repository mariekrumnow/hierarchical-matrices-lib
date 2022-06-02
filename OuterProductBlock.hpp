#ifndef HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
#define HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H

#include "Block.hpp"


template <class datatype> class HierarchicalMatrix;
template <class datatype> class EntrywiseBlock;


/// Depicts an admissible part
template <class datatype>
class OuterProductBlock: public Block<datatype> {

protected:
      unsigned int k; ///< rank of resulting matrix
      datatype * u[Block<datatype>::xDim]; ///< k * yDim array
      datatype ** x; ///< k * k array
      datatype * v[Block<datatype>::yDim]; ///< k * xDim array

public:
      /// Transforms an entrywise matrix into it's outer product form
      ///
      /// \param originalBlock
      /// \param I
      /// \param J
      OuterProductBlock(const datatype ** originalBlock, unsigned int xDim, unsigned int yDim /*, List-container*/, unsigned int rank);

      ///
      ///
      /// \return
      Block<datatype>& coarse() final;

      //---------------------------------------------------------------------------------------

      Block<datatype>& operator+( const Block<datatype>& addedBlock );
      Block<datatype>& operator*( const Block<datatype>& multBlock );
};

#endif // HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
