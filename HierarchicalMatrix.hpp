#ifndef HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H
#define HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H

#include "Block.hpp"
#include <array>

/// Transforms an entrywise matrix into a hierarchical matrix
template <class datatype, unsigned int dim>
class HierarchicalMatrix: public Block<datatype, dim> {

protected:
      Block<datatype, dim>* matrix[2][2]; ///< Hierarchical matrix, recursively divided into quadrants, [0][0] = top left, [0][1] = bottom left, [1][0] = top right, [1][1] = bottom right

public:
      /// Constructor
      /// \param originalMatrix The entrywise matrix to be transformed and calculated with
      HierarchicalMatrix(datatype originalMatrix[dim][dim]);

      ///
      Block<datatype, dim>& coarse() final;

      //Verrechnungfkten

      /// Addition of two Hierarchical Matrices
      HierarchicalMatrix& operator+( const HierarchicalMatrix& addedMatrix );
      HierarchicalMatrix& operator+=( const HierarchicalMatrix& addedMatrix );

      /// Matrix-vector multiplication
      std::array<datatype, dim> operator*( const datatype vector[dim] );
      std::array<datatype, dim> operator*=( const datatype vector[dim] );

      /// Matrix-matrix multiplication
      HierarchicalMatrix& operator*( const HierarchicalMatrix& multMatrix );
      HierarchicalMatrix& operator*=( const HierarchicalMatrix& multMatrix );

      /// Inversion
      HierarchicalMatrix& invert();

      /// LU-decomposition
      std::array<HierarchicalMatrix*,2> luDecomposition();
};

// template class HierarchicalMatrix <float, dim>;
// template class HierarchicalMatrix <double, dim>;

#endif // HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H
