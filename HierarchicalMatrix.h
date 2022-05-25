#ifndef HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H
#define HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H

template <class datatype, int dim>
class HierarchicalMatrix: public Block {

protected:
    Block matrix[2][2]*;

public:
    /// Constructor
    HierarchicalMatrix(datatype originalBlock[dim][dim]);
    //Verrechnungfkten

    /// Addition of two Hierarchical Matrices
    HierarchicalMatrix& operator+( const HierarchicalMatrix& addedMatrix );
    HierarchicalMatrix& operator+=( const HierarchicalMatrix& addedMatrix );

    /// matrix-veector multiplication
    datatype[dim] operator*( const datatype vector[dim] );
    datatype[dim] operator*=( const datatype vector[dim] );

    HierarchicalMatrix& operator*( const HierarchicalMatrix& multMatrix );
    HierarchicalMatrix& operator*=( const HierarchicalMatrix& multMatrix );

    HierarchicalMatrix& invert();
    HierarchicalMatrix[2]& luDecomposition();
};

#endif // HIERARCHICAL_MATRICES_HIERARCHICALMATRIX_H
