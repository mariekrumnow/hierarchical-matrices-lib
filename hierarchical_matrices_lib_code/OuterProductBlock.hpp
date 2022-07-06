#ifndef HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
#define HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H

#include "Block.hpp"
#include <list>
#include <vector>


/// Depicts an admissible or low-rank part of the original matrix
template <class datatype>
class OuterProductBlock: public Block<datatype> {

protected:
      datatype ** u; ///< mDim * k array / U
      datatype ** x; ///< k * k array / X, similar to S
      datatype ** v; ///< nDim * k array / V^H with complex entries, V^T with real entries

      unsigned int k; ///< Rank of original matrix part

public:
      /// Transforms an entrywise part of the matrix into its outer product form
      OuterProductBlock(datatype ** originalBlock, unsigned int mDim, unsigned int nDim, std::vector<unsigned int> iInd, std::vector<unsigned int> jInd, unsigned int rank);

      Block<datatype>* tryCoarse( double accuracy ) final;

      datatype* operator*( const datatype vector[] );

      // Block<datatype>& operator*( const Block<datatype>& multBlock );

      Block<datatype>* operator+( Block<datatype>* addedBlock );
      Block<datatype>* operator+( HierarchicalMatrix<datatype>* addedBlock );
      Block<datatype>* operator+( OuterProductBlock* addedBlock );
      Block<datatype>* operator+( EntrywiseBlock<datatype>* addedBlock );

      /// Frees all memory allocated for the u, x and v arrays
      ~OuterProductBlock();
};

#endif // HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
