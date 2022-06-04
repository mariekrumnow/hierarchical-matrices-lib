#ifndef HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
#define HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H

#include "Block.hpp"
#include <list>
#include <vector>


template <class datatype> class HierarchicalMatrix;
template <class datatype> class EntrywiseBlock;


/// Depicts an admissible part
template <class datatype>
class OuterProductBlock: public Block<datatype> {

protected:
      std::vector<unsigned int> iIndices; //< Menge der Zeilen-Indizes, die in dem Block enthalten sind
      std::vector<unsigned int> jIndices; //< Menge der Spalten-Indizes, die in dem Block enthalten sind

      datatype ** u; ///< mDim * k array
      datatype ** x; ///< k * k array
      datatype ** v; ///< nDim * k array

      unsigned int k; ///< rank of resulting matrix

public:
      /// Transforms an entrywise matrix into it's outer product form
      ///
      /// \param originalBlock
      /// \param I
      /// \param J
      OuterProductBlock(datatype ** originalBlock, unsigned int mDim, unsigned int nDim, std::vector<unsigned int> iInd, std::vector<unsigned int> jInd, unsigned int rank);

      ///
      ///
      /// \return
      Block<datatype>& coarse() final;

      //---------------------------------------------------------------------------------------

      Block<datatype>& operator+( const Block<datatype>& addedBlock );
      Block<datatype>& operator*( const Block<datatype>& multBlock );
};

#endif // HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
