#ifndef HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
#define HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H

#include "Block.hpp"

/// Depicts an admissible part
template <class datatype, unsigned int dim, unsigned int k>
class OuterProductBlock: public Block<datatype, dim> {

protected:
      unsigned int rank =k; ///<
      datatype u[dim][k]; ///<
      datatype v[dim][k]; ///<

public:
      /// Transforms an entrywise matrix into it's outer product form
      OuterProductBlock(const datatype originalBlock[dim][dim], int I, int J);

      ///
      Block<datatype, dim>& coarse() final;

      //Verrechnungfkten
};

#endif // HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
