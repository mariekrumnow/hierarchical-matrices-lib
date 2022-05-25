#ifndef HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
#define HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H

template <class datatype, int dim>
class OuterProductBlock: public Block {

protected:
    datatype u[dim][k];
    datatype v[dim][k];
    unsigned int k;

public:
    OuterProductBlock(const datatype originalBlock[dim][dim], int I, int J, int k);
    //Verrechnungfkten
};

#endif // HIERARCHICAL_MATRICES_OUTERPRODUCTBLOCK_H
