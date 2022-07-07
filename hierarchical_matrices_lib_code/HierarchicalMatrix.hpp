#ifndef HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H
#define HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H

#include "Block.hpp"
#include "EntrywiseBlock.hpp"
#include "OuterProductBlock.hpp"

#include <array>
#include <list>
#include <vector>


/// Transforms a square entrywise matrix into a hierarchical matrix to be calculated with
template <class datatype>
class HierarchicalMatrix: public Block<datatype> {

friend class OuterProductBlock<datatype>;
friend class EntrywiseBlock<datatype>;

protected:
      // [0][0] = top left, [0][1] = top right / [1][0] = bottom left, [1][1] = bottom right
      Block<datatype>* matrix[2][2]; ///< Hierarchical matrix, recursively divided into quadrants until one of the other Block types is reached, can also hold nullptr if a 2x1 or 1x2 division is reached

public:
      /// Transforms an entrywise matrix with float, double or complex float entries into a hierarchical matrix
      /// Example: HierarchicalMatrix<double> exampleMatrix(data, &indices, 100)
      ///
      /// \param originalMatrix A two-dimensional array containing the square entrywise matrix to be transformed and calculated with
      /// \param originalIndices List containing the admissible partition into blocks for column and row indices of the matrix,
      ///  each vector contains all the indices in ascending order, vectors are listed in ascending order
      ///  so the first vector always s withh 0 and the last ends with dim-1, see page 31: Fig 1.5
      /// \param dim Number of columns/rows of the input matrix
      /// \param clusterParamEta Optional cluster parameter ∈ (0.0, 1.0), will be defaulted to 0.5 if no value is given, see page 24: (1.13)
      HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int dim, double clusterParamEta =0.5);
      HierarchicalMatrix(){}

      /// Coarsens the given hierarchical matrix until the given accuracy can no longer be held,
      /// usually keeping the same storage size while reducing the number of branches/blocks
      ///
      /// \param accuracy Accuracy ∈ (0.0, 1.0) to be satisfied in each coarsening step, see page 72: (2.13)
      void coarse( double accuracy );
      Block<datatype>* coarse( double accuracy, bool checkForLeaf ) final;
      unsigned int getStorageOrRank( bool getStorage ) final;

      /// Rounded addition of two Hierarchical Matrices of the same size
      ///
      /// \param addedMatrix Second matrix to be added with the same number of rows and columns as the first matrix
      /// \return Sum of the two matrices with the same number of rows and columns as the input matrices, nullptr if matrices couldn't be added
      HierarchicalMatrix* operator+( const HierarchicalMatrix& addedMatrix );
      /// Rounded addition of two Hierarchical Matrices of the same size
      ///
      /// \param addedMatrix Second matrix to be added with the same number of rows and columns as the first matrix
      /// \return Sum of the two matrices with the same number of rows and columns as the input matrices, nullptr if matrices couldn't be added
      HierarchicalMatrix* operator+=( const HierarchicalMatrix& addedMatrix );


      /// Matrix-vector multiplication
      ///
      /// \param vector One-dimensional array with size equaling the number of columns of the input matrix
      /// \return Resulting vector with size equaling the number of rows of the input matrix, nullptr if vector couldn't be calculated
      datatype* operator*( const datatype vector[] );

      // Matrix-matrix multiplication
      // HierarchicalMatrix* operator*( const HierarchicalMatrix& multMatrix );
      // HierarchicalMatrix* operator*=( const HierarchicalMatrix& multMatrix );

      // Inversion
      // HierarchicalMatrix& invert();

      // LU-decomposition
      // std::array<HierarchicalMatrix*,2> luDecomposition();

      /// Recursively frees all Blocks saved in the matrix-attribute
      ~HierarchicalMatrix();

private:
      /// Internal connstructor for all other layers except the outside constructor call
      HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int indices[2][2], double clusterParamEta, unsigned int ** distances);
      /// Outsourced part of the constructor that holds code to be executed in both the public and private one
      void constructHierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int indices[2][2], double clusterParamEta, unsigned int ** distances);

      Block<datatype>* operator+( Block<datatype>* addedBlock );
      Block<datatype>* operator+( HierarchicalMatrix* addedBlock );
      Block<datatype>* operator+( OuterProductBlock<datatype>* addedBlock );
      Block<datatype>* operator+( EntrywiseBlock<datatype>* addedBlock );
};

#endif // HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H
