#ifndef HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
#define HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H

#include "Block.hpp"


template <class datatype, unsigned int dim> class HierarchicalMatrix;
template <class datatype, unsigned int xDim, unsigned int yDim> class EntrywiseBlock;


/// Depicts an admissible part
template <class datatype, unsigned int xDim, unsigned int yDim>
class OuterProductBlock: public Block<datatype, xDim, yDim> {

protected:
      unsigned int k; ///<
      datatype u[][xDim]; ///< Erste Dimension muss k groß sein!
      datatype v[][yDim]; ///< Erste Dimension muss k groß sein!

public:
      /// Transforms an entrywise matrix into it's outer product form
      ///
      /// \param originalBlock
      /// \param I
      /// \param J
      OuterProductBlock(const datatype originalBlock[xDim][yDim], unsigned int I, unsigned int J);

      ///
      ///
      /// \return
      Block<datatype, xDim, yDim>& coarse() final;

      //---------------------------------------------------------------------------------------

      OuterProductBlock& operator+( const OuterProductBlock& addedBlock );
      OuterProductBlock& operator+( const EntrywiseBlock<datatype, xDim, yDim>& addedBlock );
      OuterProductBlock& operator+( const HierarchicalMatrix<datatype, xDim>& addedBlock );

      OuterProductBlock& operator*( const OuterProductBlock& multBlock );
      OuterProductBlock& operator*( const EntrywiseBlock<datatype, xDim, yDim>& multBlock );
      OuterProductBlock& operator*( const HierarchicalMatrix<datatype, xDim>& multBlock );
};

#endif // HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
