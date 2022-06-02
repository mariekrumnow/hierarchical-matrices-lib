#include "../HierarchicalMatrix.hpp"

template <class datatype, unsigned int dim>
HierarchicalMatrix<datatype,dim>& HierarchicalMatrix<datatype,dim>::operator*( const HierarchicalMatrix<datatype,dim>& multMatrix ){

}

template <class datatype, unsigned int dim>
HierarchicalMatrix<datatype,dim>& HierarchicalMatrix<datatype,dim>::operator*=( const HierarchicalMatrix<datatype,dim>& multMatrix ){

}

OuterProductBlock& opopmult(OuterProductBlock& A, OuterProductBlock& B){

    OuterProductBlock C;

    if( (2 * A.k * B.k * (A.xDim + A.yDim)) - (B.k * (A.xDim + A.k)) < (2 * A.k * B.k * (B.xDim + B.yDim)) - (A.k * (B.yDim + B.k)) ){
        C.U = A.U * ( trans(A.V) * B.U);
        C.V = B.V;
    }

    else{
        C.U = A.U;
        C.V = B.V * ( trans(B.U) * A.V);
    }

    return C;
}

OuterProductBlock& opewmult(OuterProductBlock& A, EntrywiseBlock& B){

    OuterProductBlock Bop; // Bop = B (in OuterProduct Form)
    OuterProductBlock C;

  return C;
}

EntrywiseBlock& ewewmult(EntrywiseBlock& A, EntrywiseBlock& B){

    EntrywiseBlock C;

    //Evtl umwandeln in "gewöhnliche" Matrizen nötig

    C = A*B;

    //Evtl Rück umwandlung nötig

    return C;
}
