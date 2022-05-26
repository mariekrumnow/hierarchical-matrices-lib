#ifndef HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
#define HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H

#include "Block.hpp"

/// Depicts an admissible part
template <class datatype, unsigned int dim>
class OuterProductBlock: public Block<datatype, dim> {

protected:
      unsigned int k; ///<
      datatype u[][dim]; ///<
      datatype v[][dim]; ///<

public:
      /// Transforms an entrywise matrix into it's outer product form
      ///
      /// \param originalBlock
      /// \param I
      /// \param J
      OuterProductBlock(const datatype originalBlock[dim][dim], int I, int J);

      ///
      /// 
      /// \return
      Block<datatype, dim>& coarse() final;

      //Verrechnungfkten
};

#endif // HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
