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

      friend class OuterProductBlock<datatype>;
      friend class EntrywiseBlock<datatype>;

protected:
      // [0][0] = top left, [0][1] = top right / [1][0] = bottom left, [1][1] = bottom right
      Block<datatype>* matrix[2][2]; ///< Hierarchical matrix, recursively divided into quadrants, can be partially nullptr!

public:
      /// Transforms an entrywise matrix into a hierarchical matrix
      ///
      /// \param originalMatrix The entrywise matrix to be transformed and calculated with
      /// \param originalIndices Vektoren sind Blätter wie auf S.31 im Buch
      HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int dim, double clusterParamEta =0.5);


      ///
      ///
      /// \return
      Block<datatype>& coarse() final;

      /// Rounded addition of two Hierarchical Matrices
      ///
      /// \param addedMatrix Second matrix to be added
      /// \return Sum of the two matrices
      HierarchicalMatrix* operator+( const HierarchicalMatrix& addedMatrix );
      HierarchicalMatrix* operator+=( const HierarchicalMatrix& addedMatrix );


      /// Matrix-vector multiplication
      /// Since mDim == nDim for both the input and output vector are of the same size
      ///
      /// \param vector Vector of size nDim == mDim
      /// \return New result vector of size mDim == nDim
      datatype* operator*( const datatype vector[] );

      // Matrix-matrix multiplication
      // Block<datatype>& operator*( const Block<datatype>& multMatrix );
      // Block<datatype>& operator*=( const HierarchicalMatrix& multMatrix );

      // Inversion
      // HierarchicalMatrix& invert();

      // LU-decomposition
      // std::array<HierarchicalMatrix*,2> luDecomposition();

      ~HierarchicalMatrix();

private:
      HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int indices[2][2], double clusterParamEta, unsigned int ** distances);
      void constructHierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int indices[2][2], double clusterParamEta, unsigned int ** distances);

      Block<datatype>* operator+( Block<datatype>* addedBlock );
      Block<datatype>* operator+( HierarchicalMatrix* addedBlock );
      Block<datatype>* operator+( OuterProductBlock<datatype>* addedBlock );
      Block<datatype>* operator+( EntrywiseBlock<datatype>* addedBlock );
};

#endif // HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H
