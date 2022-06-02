#include "../HierarchicalMatrix.hpp"

template <class datatype>
HierarchicalMatrix<datatype>::HierarchicalMatrix(datatype ** originalMatrix, unsigned int yDim, unsigned int xDim)
      :Block<datatype>::Block(xDim, yDim)
{
      // https://stackoverflow.com/questions/2178909/how-to-initialize-3d-array-in-c
      // unsigned int newIndice[2][2][2] = { { {000, 001}, {010, 011} },
      //                                   { {100, 101}, {110, 111} } };

      // for (int i=0; i<2; i++) {
      //       for (int j=0; j<2; j++) {
      //             // Falls ja: matrix für den Quadranten = HierarchicalMatrix-Konstruktor mit neuer Dim/Startspalten usw
      //             // Falls nein: matrix für den Quadranten = OP(ad) oder EW-Konstruktor(non) je nach Admissibilty?
      //
      //             if (aufteilbar) {
      //                   matrix[i][j] = new HierarchicalMatrix<datatype>(/*abgekappte Daten*/);
      //             }
      //             else {
      //                   // Daten schon hier kappen oder erst in Konstruktor? In letzterem müsste es 2x geschrieben werden
      //
      //                   if ( k*(dim + dim) < dim*dim ) { // == Low-rank matrix
      //                         matrix[i][j] = new OuterProductBlock<datatype>(/*abgekappte Daten*/);
      //                   }
      //                   else {
      //                         matrix[i][j] = new EntrywiseBlock<datatype>(/*abgekappte Daten*/);
      //                   }
      //             }
      //       }
      // }

}
