#ifndef HIERARCHICAL_MATRICES_BLOCK_H
#define HIERARCHICAL_MATRICES_BLOCK_H

/// Abstract class to depict all parts of the matrix
template <class datatype>
class Block {

protected:
      unsigned int mDim; //< Zeilen der ursprünglichen Matrix / Rows
      unsigned int nDim; //< Spalten der ursprünglichen Matrix / Columns

public:
      /// Initialization of attributes from subclasses
      Block(unsigned int m, unsigned int n): mDim(m), nDim(n) {}

      /// Abstract functions
      virtual Block& coarse() =0;

      virtual Block& operator+(const Block& addedBlock) =0;
      virtual Block& operator*(const Block& multBlock) =0;
};

#endif // HIERARCHICAL_MATRICES_BLOCK_H
