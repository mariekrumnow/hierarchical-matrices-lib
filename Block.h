#ifndef HIERARCHICAL_MATRICES_BLOCK_H
#define HIERARCHICAL_MATRICES_BLOCK_H

template <class datatype, int dim>
class Block {

protected:
    unsigned int indiceI;
    unsigned int indiceJ;

public:
    Block& coarse();

};

#endif // HIERARCHICAL_MATRICES_BLOCK_H
