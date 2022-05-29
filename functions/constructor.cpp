#include "../HierarchicalMatrix.hpp"

template <class datatype, unsigned int dim>
HierarchicalMatrix<datatype,dim>::HierarchicalMatrix(datatype (&originalMatrix)[dim][dim], unsigned int I, unsigned int J)
      :Block<datatype, dim, dim>::Block(I, J)
{
      for(int i=0; i<2; i++){
            for(int j=0; j<2; j++){
                  // matrix[i][j] = new EntrywiseBlock<datatype, 2, 2>(originalMatrix, 1, 1);
                  matrix[i][j] = new OuterProductBlock<datatype, 2, 2>(originalMatrix, 1, 1);

                  // matrix[i][1] = matrix[i][1] + matrix[i][2];
            }
      }

      /*
      Clusterbaum simulieren, um Partitionierung der Blöcke rauszufinden
      --> Algebraisches Clustern + Admissibility condition (reicht immer selbe) + SVD (LA-Pack einbinden?)

      Als Baum in matrix-Attribut speichern

      Konstruktoren der OuterProduct (Admissible) und Entrywise (non-admissible) Blöcke aufrufen
      */
}
