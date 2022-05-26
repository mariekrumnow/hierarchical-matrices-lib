#ifndef HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
#define HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H

#include "Block.hpp"

/// Depicts a non-admissible part
template <class datatype, unsigned int dim>
class EntrywiseBlock: public Block<datatype, dim>{

protected:
      datatype block[dim][dim]; ///<

public:
      /// Copies a matrix to be stored in a block
      ///
      /// \param originalBlock
      /// \param I
      /// \param J
      EntrywiseBlock(datatype originalBlock[dim][dim], int I, int J);

      /// 
      ///
      /// \return
      Block<datatype, dim>& coarse() final;

      //Verrechnungfkten
};

#endif // HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
