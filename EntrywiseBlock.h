#ifndef HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
#define HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H

template <class datatype, int dim>
class EntrywiseBlock: public Block {

protected:
    datatype block[dim][dim];

public:
    EntrywiseBlock(const datatype originalBlock[dim][dim], int I, int J);
    //Verrechnungfkten
};

#endif // HIERARCHICAL_MATRICES_ENTRYWISEBLOCK_H
