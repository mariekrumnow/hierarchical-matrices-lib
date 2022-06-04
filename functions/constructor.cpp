#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

// https://www.cplusplus.com/reference/stl/
// splice & size (anz elemente in liste) wichtig

// HierarchicalMatrix
template <class datatype>
HierarchicalMatrix<datatype>::HierarchicalMatrix(datatype ** originalMatrix, unsigned int mDim, unsigned int nDim, std::list<std::vector<unsigned int>> indices)
      :Block<datatype>::Block(mDim, nDim)
{
    std::vector<unsigned int> indice1 (0,0);
    matrix[0][0] = new OuterProductBlock<datatype>(originalMatrix, mDim, nDim, indice1, indice1, 2);
    matrix[0][1] = new EntrywiseBlock<datatype>(originalMatrix, mDim, nDim, indice1, indice1);

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
OuterProductBlock<datatype>::OuterProductBlock(datatype ** originalBlock, unsigned int mDim, unsigned int nDim, std::vector<unsigned int> iInd, std::vector<unsigned int> jInd, unsigned int rank)
      :Block<datatype>::Block(mDim, nDim), iIndices(iInd), jIndices(jInd), k(rank)
{
      // Block raussuchen und als Matrix abspeichern?
      // SVD mit Block aufrufen
      // Zurückgegebene Matrizen in die Attribute setzen

      // for(unsigned int i=0; i< mDim; i++){
      //       u[i] = new datatype[k];
      // }
      //
      for(unsigned int i=0; i< nDim; i++){
            v[i] = new datatype[k];
      }

      // k = Rang der Matrix? Weil Rang = Maximale Anzahl linear unabhängiger Zeilen/Spalten = k (S.10)
      // Berechnung des Rangs (k) des Blocks via
      // https://www.geeksforgeeks.org/program-for-rank-of-matrix/
      k = nDim;

      datatype copy[mDim][nDim]; // Kopie wird verändert!
      for(unsigned int i=0; i< mDim; i++){
            for(unsigned int j=0; j < nDim; j++){
                  copy[i][j] = originalBlock[i][j];
            }
      }


      for (unsigned int row = 0; row < k; row++){
            if (copy[row][row]){
                  for (unsigned int col = 0; col < mDim; col++){
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

                  for (unsigned int i = row + 1; i < mDim; i++) {
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
                        for (unsigned int i=0; i < mDim; i++){
                              copy[i][row] = copy[i][k];
                        }
                  }

                  row--;
            }
      } // Rang k FERTIG
}


// EntrywiseBlock
template <class datatype>
EntrywiseBlock<datatype>::EntrywiseBlock(datatype ** originalBlock, unsigned int nDim, unsigned int mDim, std::vector<unsigned int> iInd, std::vector<unsigned int> jInd)
      :Block<datatype>::Block(mDim, nDim), iIndices(iInd), jIndices(jInd), block(originalBlock)
{
      // Sonst nix mehr nötig zu machen, muss ja nur 1:1 eingespeichert werden
}
