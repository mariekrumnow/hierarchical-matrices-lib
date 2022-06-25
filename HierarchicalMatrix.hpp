#ifndef HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H
#define HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H

#include "Block.hpp"
#include "EntrywiseBlock.hpp"
#include "OuterProductBlock.hpp"

#include <array>
#include <list>
#include <vector>

/// Transforms an entrywise matrix into a hierarchical matrix to be calculated with
template <class datatype>
class HierarchicalMatrix: public Block<datatype> {

protected:
      // [0][0] = top left, [0][1] = top right / [1][0] = bottom left, [1][1] = bottom right
      Block<datatype>* matrix[2][2]; ///< Hierarchical matrix, recursively divided into quadrants, can be partially nullptr!

private:
      enum IndiceOrientation {kRangeI=0, kRangeJ=1, kBottom=0, kTop=1};

      HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int indices[2][2], double clusterParamEta, unsigned int ** distances);
      void constructHierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int indices[2][2], double clusterParamEta, unsigned int ** distances);

public:
      /// Transforms an entrywise matrix into a hierarchical matrix
      ///
      /// \param originalMatrix The entrywise matrix to be transformed and calculated with
      HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int dim, double clusterParamEta =0.5);

      ///
      ///
      /// \return
      Block<datatype>& coarse() final;

      /// Rounded addition of two Hierarchical Matrices
      ///
      /// \param addedMatrix Second matrix to be added
      /// \return Sum of the two matrices
      Block<datatype>& operator+( const Block<datatype>& addedMatrix );
      Block<datatype>& operator+=( const HierarchicalMatrix& addedMatrix );

      /// Matrix-vector multiplication
      ///
      /// \param vector
      /// \return
      datatype* operator*( const datatype vector[Block<datatype>::mDim] );
      datatype* operator*=( const datatype vector[Block<datatype>::mDim] );

      /// Matrix-matrix multiplication
      ///
      /// \param multMatrix
      /// \return
      Block<datatype>& operator*( const Block<datatype>& multMatrix );
      Block<datatype>& operator*=( const HierarchicalMatrix& multMatrix );

      /// Inversion
      ///
      /// \return The inverted matrix
      HierarchicalMatrix& invert();

      /// LU-decomposition
      ///
      /// \return
      std::array<HierarchicalMatrix*,2> luDecomposition();

      //---------------------------------------------------------------------------------------

      // Block<datatype>& operator+( const EntrywiseBlock<datatype>& addedBlock );
      // Block<datatype>& operator+( const OuterProductBlock<datatype>& addedBlock );
      //
      // Block<datatype>& operator*( const EntrywiseBlock<datatype>& multBlock );
      // Block<datatype>& operator*( const OuterProductBlock<datatype>& multBlock );
};

#endif // HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H
