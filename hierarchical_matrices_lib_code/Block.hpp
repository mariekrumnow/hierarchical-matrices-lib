#ifndef HIERARCHICAL_MATRICES_BLOCK_H
#define HIERARCHICAL_MATRICES_BLOCK_H

template <class datatype> class HierarchicalMatrix;
template <class datatype> class OuterProductBlock;
template <class datatype> class EntrywiseBlock;

/// Helper enum to navigate the indiceRang-array and similar
enum IndiceOrientation {kRangeI=0, kRangeJ=1, kBottom=0, kTop=1};


/// Abstract class to depict all parts of the matrix
template <class datatype>
class Block {

protected:

      unsigned int mDim; ///< Number of rows from the original matrix depicted within this block
      unsigned int nDim; ///< Number of columns from the original matrix depicted within this block
      unsigned int indiceRange[2][2]; ///< Lower and upper bound of row and column indices from the original matrix depicted within this block, see enum InndiceOrientation

public:
      /// Automatic initialization of attributes from subclasses
      Block(unsigned int m, unsigned int n): mDim(m), nDim(n) {}
      Block(){}

      virtual Block* tryCoarse( double accuracy ) =0;
      virtual unsigned int getStorage() =0;

      virtual datatype* operator*( const datatype vector[] ) =0;

      // virtual Block& operator*(const Block& multBlock) =0;

      /// First layer of polymorphic addition, detects the first operand and calls swapped addition to detect the second operand through the second layer
      virtual Block* operator+( Block* addedBlock ) =0;

      /// Second layer of polymorphic addition, detects the second operand and actually calculates the correxponding sum
      virtual Block* operator+( HierarchicalMatrix<datatype>* addedBlock ) =0;
      /// Second layer of polymorphic addition, detects the second operand and actually calculates the correxponding sum
      virtual Block* operator+( OuterProductBlock<datatype>* addedBlock ) =0;
      /// Second layer of polymorphic addition, detects the second operand and actually calculates the correxponding sum
      virtual Block* operator+( EntrywiseBlock<datatype>* addedBlock ) =0;

      virtual ~Block(){};
};

#endif // HIERARCHICAL_MATRICES_BLOCK_H
