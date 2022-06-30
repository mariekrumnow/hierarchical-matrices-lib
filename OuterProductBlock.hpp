#ifndef HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
#define HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H

#include "Block.hpp"
#include <list>
#include <vector>


/// Depicts an admissible part
template <class datatype>
class OuterProductBlock: public Block<datatype> {

protected:
      datatype ** u; ///< mDim * k array
      datatype ** x; ///< k * k array
      datatype ** v; ///< nDim * k array

      unsigned int k; ///< rank of resulting matrix

public:
      /// Transforms an entrywise matrix into it's outer product form
      ///
      /// \param
      OuterProductBlock(datatype ** originalBlock, unsigned int mDim, unsigned int nDim, std::vector<unsigned int> iInd, std::vector<unsigned int> jInd, unsigned int rank);

      ~OuterProductBlock();

      ///
      ///
      /// \return
      Block<datatype>& coarse() final;

      //---------------------------------------------------------------------------------------

      // Block<datatype>& operator*( const Block<datatype>& multBlock );

      Block<datatype>* operator+( Block<datatype>* addedBlock );
      Block<datatype>* operator+( HierarchicalMatrix<datatype>* addedBlock );
      Block<datatype>* operator+( OuterProductBlock* addedBlock );
      Block<datatype>* operator+( EntrywiseBlock<datatype>* addedBlock );
};

#endif // HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
