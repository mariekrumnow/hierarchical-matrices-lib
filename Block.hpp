#ifndef HIERARCHICAL_MATRICES_BLOCK_H
#define HIERARCHICAL_MATRICES_BLOCK_H

template <class datatype> class HierarchicalMatrix;
template <class datatype> class OuterProductBlock;
template <class datatype> class EntrywiseBlock;


enum IndiceOrientation {kRangeI=0, kRangeJ=1, kBottom=0, kTop=1};


/// Abstract class to depict all parts of the matrix
template <class datatype>
class Block {

protected:

      unsigned int mDim; //< Zeilen der ursprünglichen Matrix / Rows
      unsigned int nDim; //< Spalten der ursprünglichen Matrix / Columns
      unsigned int indiceRange[2][2]; //< Indices that can be found within the HM

public:
      /// Initialization of attributes from subclasses
      Block(unsigned int m, unsigned int n): mDim(m), nDim(n) {}

      /// Abstract functions
      virtual Block& coarse() =0;

      virtual datatype* operator*( const datatype vector[] ) =0;

      // virtual Block& operator*(const Block& multBlock) =0;

      virtual Block* operator+( Block* addedBlock ) =0;
      virtual Block* operator+( HierarchicalMatrix<datatype>* addedBlock ) =0;
      virtual Block* operator+( OuterProductBlock<datatype>* addedBlock ) =0;
      virtual Block* operator+( EntrywiseBlock<datatype>* addedBlock ) =0;

      virtual ~Block(){};
};

#endif // HIERARCHICAL_MATRICES_BLOCK_H
