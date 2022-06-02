#ifndef HIERARCHICAL_MATRICES_BLOCK_H
#define HIERARCHICAL_MATRICES_BLOCK_H

/// Abstract class to depict all parts of the matrix
template <class datatype>
class Block {

protected:
      unsigned int xDim;
      unsigned int yDim;
      // Liste mit abgedekten Indizes?

public:
      /// Initialization of attributes from subclasses
      Block(unsigned int x, unsigned int y): xDim(x), yDim(y) {}

      /// Abstract functions
      virtual Block& coarse() =0;
      
      virtual Block& operator+(const Block& addedBlock) =0;
      virtual Block& operator*(const Block& multBlock) =0;
};

#endif // HIERARCHICAL_MATRICES_BLOCK_H
