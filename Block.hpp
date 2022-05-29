#ifndef HIERARCHICAL_MATRICES_BLOCK_H
#define HIERARCHICAL_MATRICES_BLOCK_H

/// Abstract class to depict all parts of the matrix
template <class datatype, unsigned int xDim, unsigned int yDim>
class Block {

protected:
      unsigned int indiceI; ///< At which column the Block belongs in the matrix (range 1 to Dim)
      unsigned int indiceJ; ///< At which row the Block belongs in the matrix (range 1 to Dim)

public:
      /// Initialization of attributes from subclasses
      Block(unsigned int I, unsigned int J): indiceI(I), indiceJ(J) {}

      /// Abstract function for coarsening
      virtual Block& coarse() =0;
};

#endif // HIERARCHICAL_MATRICES_BLOCK_H
