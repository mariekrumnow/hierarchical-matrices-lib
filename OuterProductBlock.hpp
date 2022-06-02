#ifndef HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
#define HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H

#include "Block.hpp"


template <class datatype> class HierarchicalMatrix;
template <class datatype> class EntrywiseBlock;


/// Depicts an admissible part
template <class datatype>
class OuterProductBlock: public Block<datatype> {

protected:
      unsigned int k; ///<
      datatype * u[xDim]; ///< Erste Dimension muss k groß sein!
      datatype * v[yDim]; ///< Erste Dimension muss k groß sein!

public:
      /// Transforms an entrywise matrix into it's outer product form
      ///
      /// \param originalBlock
      /// \param I
      /// \param J
      OuterProductBlock(const datatype (&originalBlock)[xDim][yDim], unsigned int rank);

      ///
      ///
      /// \return
      Block<datatype>& coarse() final;

      //---------------------------------------------------------------------------------------

      OuterProductBlock& operator+( const OuterProductBlock& addedBlock );
      OuterProductBlock& operator+( const EntrywiseBlock<datatype>& addedBlock );
      OuterProductBlock& operator+( const HierarchicalMatrix<datatype>& addedBlock );

      OuterProductBlock& operator*( const OuterProductBlock& multBlock );
      OuterProductBlock& operator*( const EntrywiseBlock<datatype>& multBlock );
      OuterProductBlock& operator*( const HierarchicalMatrix<datatype>& multBlock );
};

#endif // HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
