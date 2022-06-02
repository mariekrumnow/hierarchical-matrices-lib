#include "../HierarchicalMatrix.hpp"

template <class datatype, unsigned int dim>
HierarchicalMatrix<datatype,dim>::HierarchicalMatrix(datatype (&originalMatrix)[dim][dim], unsigned int I, unsigned int J)
      :Block<datatype, dim, dim>::Block(I, J)
{
      // https://stackoverflow.com/questions/2178909/how-to-initialize-3d-array-in-c
      // unsigned int newIndice[2][2][2] = { { {000, 001}, {010, 011} },
      //                                   { {100, 101}, {110, 111} } };

      for (int i=0; i<2; i++) {
            for (int j=0; j<2; j++) {
                  // Falls ja: matrix für den Quadranten = HierarchicalMatrix-Konstruktor mit neuer Dim/Startspalten usw
                  // Falls nein: matrix für den Quadranten = OP(ad) oder EW-Konstruktor(non) je nach Admissibilty?

                  if (aufteilbar) {
                        matrix[i][j] = new HierarchicalMatrix<datatype, dim - newIndice[i][j][0]>(/*abgekappte Daten*/, newIndice[i][j][0], newIndice[i][j][1]);
                  }
                  else {
                        // Daten schon hier kappen oder erst in Konstruktor? In letzterem müsste es 2x geschrieben werden
                        
                        if ( k*(dim + dim) < dim*dim ) { // == Low-rank matrix
                              matrix[i][j] = new OuterProductBlock<datatype, dim - newIndice[i][j][0], dim - newIndice[i][j][1]>(/*abgekappte Daten*/, newIndice[i][j][0], newIndice[i][j][1]);
                        }
                        else {
                              matrix[i][j] = new EntrywiseBlock<datatype, dim - newIndice[i][j][0], dim - newIndice[i][j][1]>(/*abgekappte Daten*/, newIndice[i][j][0], newIndice[i][j][1]);
                        }
                  }
            }
      }

}
