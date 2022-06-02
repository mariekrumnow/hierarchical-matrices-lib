#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

// HierarchicalMatrix
template <class datatype>
HierarchicalMatrix<datatype>::HierarchicalMatrix(datatype ** originalMatrix, unsigned int yDim, unsigned int xDim)
      :Block<datatype>::Block(xDim, yDim)
{
      // Wenn Liste nur einen Vektor enthält:
            // Fehler
      // Sonst:
            // Teile die Liste in der "Mitte"
            // Wenn Liste nur noch einen Vektor enthält:
                  // Schaue ob EW oder OP
            // Sonst:
                  // Rufe HM-Konstruktor auf


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


// OuterProductBlock
template <class datatype>
OuterProductBlock<datatype>::OuterProductBlock(const datatype ** originalBlock, unsigned int xDim, unsigned int yDim /*, List-container*/, unsigned int rank)
      :Block<datatype>::Block(xDim, yDim), k(rank)
{
      // Block raussuchen und als Matrix abspeichern?
      // SVD mit Block aufrufen
      // Zurückgegebene Matrizen in die Attribute setzen

      // for(unsigned int i=0; i< xDim; i++){
      //       u[i] = new datatype[k];
      // }
      //
      for(unsigned int i=0; i< yDim; i++){
            v[i] = new datatype[k];
      }

      // k = Rang der Matrix? Weil Rang = Maximale Anzahl linear unabhängiger Zeilen/Spalten = k (S.10)
      // Berechnung des Rangs (k) des Blocks via
      // https://www.geeksforgeeks.org/program-for-rank-of-matrix/
      k = yDim;

      datatype copy[xDim][yDim]; // Kopie wird verändert!
      for(unsigned int i=0; i< xDim; i++){
            for(unsigned int j=0; j < yDim; j++){
                  copy[i][j] = originalBlock[i][j];
            }
      }


      for (unsigned int row = 0; row < k; row++){
            if (copy[row][row]){
                  for (unsigned int col = 0; col < xDim; col++){
                        if (col != row){
                              datatype mult = copy[col][row] / copy[row][row]; // Kann failen wenn int erlaubt
                              for (unsigned int i = 0; i < k; i++){
                                    copy[col][i] -= mult * copy[row][i];
                              }
                        }
                  }
            }
            else {
                  bool reduce = true;

                  for (unsigned int i = row + 1; i < xDim; i++) {
                        if (copy[i][row]) {
                              for (unsigned int j=0; j < k; j++) {
                                    datatype temp = copy[row][j];
                                    copy[row][j] = copy[i][j];
                                    copy[i][j] = temp;
                              }
                              reduce = false;
                              break;
                        }
                  }

                  if (reduce) {
                        k--;
                        for (unsigned int i=0; i < xDim; i++){
                              copy[i][row] = copy[i][k];
                        }
                  }

                  row--;
            }
      } // Rang k FERTIG
}


// EntrywiseBlock
template <class datatype>
EntrywiseBlock<datatype>::EntrywiseBlock(datatype ** originalBlock, unsigned int yDim, unsigned int xDim /*, List-container*/)
      :Block<datatype>::Block(xDim, yDim), block(originalBlock)
{
      // Sonst nix mehr nötig zu machen, muss ja nur 1:1 eingespeichert werden
}
