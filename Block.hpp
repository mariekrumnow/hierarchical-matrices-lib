#ifndef HIERARCHICAL_MATRICES_BLOCK_H
#define HIERARCHICAL_MATRICES_BLOCK_H

/// Abstract class to depict all parts of the matrix
template <class datatype, unsigned int dim>
class Block {

protected:
      unsigned int indiceI; ///<
      unsigned int indiceJ; ///<

public:
      ///
      virtual Block& coarse() =0;

};

#endif // HIERARCHICAL_MATRICES_BLOCK_H
