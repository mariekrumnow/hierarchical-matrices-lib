#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

#include <cmath>
#include <lapacke.h>

// TODO: Szenarios mit <4 Aufteilungen durchdenken --> if um alles in nested for-loop mit Prüfung, ob Indizes < bzw > falls das reicht?
// TODO: Um Dim bei Hierarchischer Matrix kümmern
// TODO: Entscheidung ob EW/OP nach admissibility
// TODO: SVD mit LaPack machen

// HierarchicalMatrix
template <class datatype>
HierarchicalMatrix<datatype>::HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int mDim, unsigned int nDim, unsigned int indices[2][2])
      :Block<datatype>::Block(mDim, nDim)
{
      enum IndiceOrientation {kRangeI=0, kRangeJ=1, kBottom=0, kTop=1};

      // Beim initialen Aufruf die Anzahl der Vektoren == Blöcke speichern
      if (indices == nullptr) {
            indices[kRangeI][kBottom] = 1;
            indices[kRangeI][kTop] = originalIndices->size();

            indices[kRangeJ][kBottom] = 1;
            indices[kRangeJ][kTop] = originalIndices->size();
      }

      // Mitte der Indizes zum Aufteilen in Quadranten berechnen
      unsigned int iMiddle = indices[kRangeI][kBottom] + floor(indices[kRangeI][kTop]-indices[kRangeI][kBottom] /2);
      unsigned int jMiddle = indices[kRangeJ][kBottom] + floor(indices[kRangeJ][kTop]-indices[kRangeJ][kBottom] /2);

      // Prüfen, ob nur noch je 1 Indize vorhanden ist --> Blatt erreicht
      bool aufteilbar[2][2] = {
            {indices[kRangeI][kBottom] == iMiddle && indices[kRangeJ][kBottom] == jMiddle,
                  indices[kRangeI][kBottom] == iMiddle && jMiddle+1 == indices[kRangeJ][kTop]},
            {iMiddle+1 == indices[kRangeI][kTop] && indices[kRangeJ][kBottom] == jMiddle,
                  iMiddle+1 == indices[kRangeI][kTop] && jMiddle+1 == indices[kRangeJ][kTop]}   };

      for (int a=0; a<2; a++) {
            for (int b=0; b<2; b++) {
                  if (aufteilbar[a][b]) {
                        // Neue Dim berechnen? (Brauch man die für die Hierarchischen?)
                        // (for each vector in list, get size, sum size)

                        unsigned int newInd[2][2];
                        if(a < 2) {
                              newInd[kRangeI][kBottom] = indices[kRangeI][kBottom];
                              newInd[kRangeI][kTop] = iMiddle;
                        }
                        else {
                              newInd[kRangeI][kBottom] = iMiddle+1;
                              newInd[kRangeI][kTop] = indices[kRangeI][kTop];
                        }

                        if( !(b % 2) ) {
                              newInd[kRangeJ][kBottom] = indices[kRangeJ][kBottom];
                              newInd[kRangeJ][kTop] = jMiddle;
                        }
                        else {
                              newInd[kRangeJ][kBottom] = jMiddle+1;
                              newInd[kRangeJ][kTop] = indices[kRangeJ][kTop];
                        }

                        matrix[a][b] = new HierarchicalMatrix<datatype>(originalMatrix, originalIndices, mDim, nDim, newInd); //DIM NOCH NET RICHTIG
                  }
                  else {
                        // Vektoren anhand der Indize raussuchen
                        unsigned int iVectorNum = a<2 ? iMiddle : iMiddle+1;
                        auto iVector = originalIndices->begin();
                        while( iVectorNum != 1 ) {
                              iVector++;
                              iVectorNum--;
                        }

                        unsigned int jVectorNum = !(b % 2) ? jMiddle : jMiddle+1;
                        auto jVector = originalIndices->begin();
                        while( jVectorNum != 1 ) {
                              jVector++;
                              jVectorNum--;
                        }

                        unsigned int newMdim = iVector->size();
                        unsigned int newNdim = jVector->size();

                        // Indizes entsprechend der Vektoren in die gekappte Matrix kopieren
                        datatype ** cutMatrix = new datatype*[newMdim];
                        for(unsigned int a=0; a < newMdim; a++){
                              cutMatrix[a] = new datatype[newNdim];

                              for(unsigned int b=0; b < newNdim; b++){
                                    cutMatrix[a][b] = originalMatrix[ (*iVector)[a] ][ (*iVector)[b] ];
                              }
                        }

                        // Berechnung des Rangs (k) des Blocks via
                        // https://www.geeksforgeeks.org/program-for-rank-of-matrix/
                        unsigned int k = newNdim;

                        datatype copy[newMdim][newNdim]; // Kopie wird verändert!
                        for(unsigned int i=0; i< newMdim; i++){
                              for(unsigned int j=0; j < newNdim; j++){
                                    copy[i][j] = cutMatrix[i][j];
                              }
                        }


                        for (unsigned int row = 0; row < k; row++){
                              if (copy[row][row]){
                                    for (unsigned int col = 0; col < newMdim; col++){
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

                                    for (unsigned int i = row + 1; i < newMdim; i++) {
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
                                          for (unsigned int i=0; i < newMdim; i++){
                                                copy[i][row] = copy[i][k];
                                          }
                                    }

                                    row--;
                              }
                        } // Rang k FERTIG

                        if ( k*(newMdim + newNdim) < newMdim*newNdim ) { // == Low-rank matrix
                              matrix[a][b] = new OuterProductBlock<datatype>(cutMatrix, newMdim, newNdim, *iVector, *jVector, k);
                        }
                        else {
                              matrix[a][b] = new EntrywiseBlock<datatype>(cutMatrix, newMdim, newNdim, *iVector, *jVector);
                        }
                  }
            }
      }

}


// OuterProductBlock
template <class datatype>
OuterProductBlock<datatype>::OuterProductBlock(datatype ** originalBlock, unsigned int mDim, unsigned int nDim, std::vector<unsigned int> iInd, std::vector<unsigned int> jInd, unsigned int rank)
      :Block<datatype>::Block(mDim, nDim), iIndices(iInd), jIndices(jInd), k(rank)
{
      // Übergabe- & Rückgabeparam. vorbereiten
      // datatype ** u; ///< mDim * k array
      // datatype ** x; ///< k * k array
      // datatype ** v; ///< nDim * k array
      // Wie mit anderen Datentypen umgehen? Kann abhängig davon die Funktion aufgerufen werden?

      u = new datatype*[mDim];
      for(unsigned int a=0; a < mDim; a++){
            u[a] = new datatype[k];
      }

      x = new datatype*[k];
      for(unsigned int a=0; a < k; a++){
            x[a] = new datatype[k];
      }

      v = new datatype*[nDim];
      for(unsigned int a=0; a < nDim; a++){
            v[a] = new datatype[k];
      }

      // SVD mit Block aufrufen
      // LAPACK_COL_MAJOR als erstes Arg?, originalBlock zu double Array?
      // http://www.netlib.org/lapack/explore-html/d1/d7e/group__double_g_esing_ga84fdf22a62b12ff364621e4713ce02f2.html#ga84fdf22a62b12ff364621e4713ce02f2
      // int info = LAPACKE_dgesvd('A', 'A', mDim, nDim, originalBlock, mDim, s, u, ldu, vt, ldvt);+
      // if (info !=0){
      //       // std::cerr<<"Lapack error occured in dgesdd. error code :"<<info<<std::endl;
      // }
}


// EntrywiseBlock
template <class datatype>
EntrywiseBlock<datatype>::EntrywiseBlock(datatype ** originalBlock, unsigned int nDim, unsigned int mDim, std::vector<unsigned int> iInd, std::vector<unsigned int> jInd)
      :Block<datatype>::Block(mDim, nDim), iIndices(iInd), jIndices(jInd), block(originalBlock)
{
      // Sonst nix mehr nötig zu machen, muss ja nur 1:1 eingespeichert werden
}
