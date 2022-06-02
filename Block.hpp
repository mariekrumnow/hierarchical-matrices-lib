#ifndef HIERARCHICAL_MATRICES_BLOCK_H
#define HIERARCHICAL_MATRICES_BLOCK_H

/// Abstract class to depict all parts of the matrix
template <class datatype>
class Block {

protected:
      unsigned int xDim;
      unsigned int yDim;

public:
      /// Initialization of attributes from subclasses
      Block(unsigned int x, unsigned int y): xDim(x), yDim(y) {}

      /// Abstract function for coarsening
      virtual Block& coarse() =0;
};

#endif // HIERARCHICAL_MATRICES_BLOCK_H
