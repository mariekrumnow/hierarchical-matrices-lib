#ifndef HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H
#define HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H

#include "Block.hpp"
#include "EntrywiseBlock.hpp"
#include "OuterProductBlock.hpp"

#include <array>

/// Transforms an entrywise matrix into a hierarchical matrix to be calculated with
template <class datatype, unsigned int dim>
class HierarchicalMatrix: public Block<datatype, dim, dim> {

protected:
      Block<datatype, dim, dim>* matrix[2][2]; ///< Hierarchical matrix, recursively divided into quadrants, [0][0] = top left, [0][1] = bottom left, [1][0] = top right, [1][1] = bottom right

public:
      /// Transforms an entrywise matrix into a hierarchical matrix
      ///
      /// \param originalMatrix The entrywise matrix to be transformed and calculated with
      HierarchicalMatrix(datatype originalMatrix[dim][dim]);

      ///
      ///
      /// \return
      Block<datatype, dim, dim>& coarse() final;

      /// Rounded addition of two Hierarchical Matrices
      ///
      /// \param addedMatrix Second matrix to be added
      /// \return Sum of the two matrices
      HierarchicalMatrix& operator+( const HierarchicalMatrix& addedMatrix );
      HierarchicalMatrix& operator+=( const HierarchicalMatrix& addedMatrix );

      /// Matrix-vector multiplication
      ///
      /// \param vector
      /// \return
      std::array<datatype, dim> operator*( const datatype vector[dim] );
      std::array<datatype, dim> operator*=( const datatype vector[dim] );

      /// Matrix-matrix multiplication
      ///
      /// \param multMatrix
      /// \return
      HierarchicalMatrix& operator*( const HierarchicalMatrix& multMatrix );
      HierarchicalMatrix& operator*=( const HierarchicalMatrix& multMatrix );

      /// Inversion
      ///
      /// \return The inverted matrix
      HierarchicalMatrix& invert();

      /// LU-decomposition
      ///
      /// \return
      std::array<HierarchicalMatrix*,2> luDecomposition();

      //---------------------------------------------------------------------------------------

      HierarchicalMatrix& operator+( const EntrywiseBlock<datatype, dim, dim>& addedBlock );
      HierarchicalMatrix& operator+( const OuterProductBlock<datatype, dim, dim>& addedBlock );
      
      HierarchicalMatrix& operator*( const EntrywiseBlock<datatype, dim, dim>& multBlock );
      HierarchicalMatrix& operator*( const OuterProductBlock<datatype, dim, dim>& multBlock );
};

#endif // HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H
