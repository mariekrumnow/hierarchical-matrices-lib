#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

#include <cmath>
#include <algorithm>
#include <lapacke.h>

#include <iostream>

// TODO: Lapack-Dateien finden + X berechnen? + in Attr speichern
// TODO: Wie clusterParamEta rausfinden? Immer 1? Variabel? S.22/23
// TODO: Was sind Distanz & Durchmesser? Wie berechnen?

// Helper function for HierarchicalMatrix, defined at bottom
template <class datatype> unsigned int diameter(std::vector<unsigned int> cluster, datatype ** originalMatrix);


// HierarchicalMatrix
template <class datatype>
HierarchicalMatrix<datatype>::HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int mDim, unsigned int nDim, unsigned int indices[2][2])
      :Block<datatype>::Block(mDim, nDim)
{
      enum IndiceOrientation {kRangeI=0, kRangeJ=1, kBottom=0, kTop=1};

      if (indices == nullptr) {
            // Beim initialen Aufruf die Anzahl der Vektoren == Blöcke speichern
            indices = new unsigned int[2][2];
            indices[kRangeI][kBottom] = 1;
            indices[kRangeI][kTop] = originalIndices->size();

            indices[kRangeJ][kBottom] = 1;
            indices[kRangeJ][kTop] = originalIndices->size();
      }
      else {
            // Dimension der enthaltenen Blöcke berechnen
            unsigned int vectorIndice = 1;
            auto currentVector = originalIndices->begin();
            while( currentVector != originalIndices->end() ) {
                  if( indices[kRangeI][kBottom] <= vectorIndice || vectorIndice <= indices[kRangeI][kTop] ) {
                        mDim += currentVector->size();
                  }
                  else if( indices[kRangeJ][kBottom] <= vectorIndice || vectorIndice <= indices[kRangeJ][kTop] ) {
                        nDim += currentVector->size();
                  }
                  currentVector++;
                  vectorIndice++;
            }
      }

      // Mitte der Indizes zum Aufteilen in Quadranten berechnen
      unsigned int iMiddle = indices[kRangeI][kBottom] + floor((indices[kRangeI][kTop]-indices[kRangeI][kBottom]) /2);
      unsigned int jMiddle = indices[kRangeJ][kBottom] + floor((indices[kRangeJ][kTop]-indices[kRangeJ][kBottom]) /2);

      // Prüfen, ob nur noch je 1 Indize vorhanden ist --> Blatt erreicht
      bool aufteilbar[2][2] = {
            { !(indices[kRangeI][kBottom] == iMiddle && indices[kRangeJ][kBottom] == jMiddle),
                  !(indices[kRangeI][kBottom] == iMiddle && jMiddle+1 == indices[kRangeJ][kTop]) },
            { !(iMiddle+1 == indices[kRangeI][kTop] && indices[kRangeJ][kBottom] == jMiddle),
                  !(iMiddle+1 == indices[kRangeI][kTop] && jMiddle+1 == indices[kRangeJ][kTop]) }   };

      // Check if not all 4 Blocks can be built and thus invalid ones need to be skipped in for-loop
      unsigned int maxBlockI = 2;
      unsigned int maxBlockJ = 2;
      if( iMiddle+1 > indices[kRangeI][kTop] ) {
            if( jMiddle+1 > indices[kRangeJ][kTop] ) { // Just 1 indice for i and j each --> only 1 Block can be built
                  maxBlockI = 1;
                  maxBlockJ = 1;
            }
            else{ // Just 1 indice for i --> only 1x2 Blocks can be built
                  maxBlockI = 1;
            }
      }
      else if( jMiddle+1 > indices[kRangeJ][kTop] ) { // Just 1 indice for j --> only 2x1 Blocks can be built
            maxBlockJ = 1;
      }
      // Side note: A 1x3 block or similar can't appear since we aren't dividing blocks bigger than 2x2!

      std::cout << maxBlockI << " " << maxBlockJ << " ";

      for (unsigned int a=0; a< maxBlockI; a++) {
            for (unsigned int b=0; b< maxBlockJ; b++) {
                  if (aufteilbar[a][b]) {
                      std::cout << "HM ";
                        // Indizes aufteilen
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

                        matrix[a][b] = new HierarchicalMatrix<datatype>(originalMatrix, originalIndices, 0, 0, newInd); // Dim wird sowieso überschrieben
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
                             std::cout << "OP1 ";
                              matrix[a][b] = new OuterProductBlock<datatype>(cutMatrix, newMdim, newNdim, *iVector, *jVector, k);
                        }
                        else {
                              unsigned int minDistance = 100 /*Erster Wert*/; //!!Ggf datatype wenn Wert aus Matrix
                              std::for_each(iVector->cbegin(), iVector->cend(), [&minDistance, jVector, originalMatrix] (const unsigned int ind1) {
                                    std::for_each(jVector->cbegin(), jVector->cend(), [ind1, &minDistance, originalMatrix] (const unsigned int ind2) {
                                          if(originalMatrix[ind1][ind2] < minDistance) {
                                                minDistance = originalMatrix[ind1][ind2];
                                          }
                                    });
                              });

                              // Between 0 and 1? >0? Depends on Cluster somehow
                              unsigned int clusterParamEta = 1;

                              if( std::min(diameter(*iVector, originalMatrix), diameter(*jVector, originalMatrix)) <= clusterParamEta * minDistance ) {
                                    // Matrix can be approximated by low-rank one --> coarse (admissible)
                                     std::cout << "OP2 ";
                                    matrix[a][b] = new OuterProductBlock<datatype>(cutMatrix/*.coarse()*/, newMdim, newNdim, *iVector, *jVector, k);
                              }
                              else{
                                    // Important info will be lost by approximation, has to be saved with more effort (non-admissible)
                                     std::cout << "EW ";
                                    matrix[a][b] = new EntrywiseBlock<datatype>(cutMatrix, newMdim, newNdim, *iVector, *jVector);
                              }
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

      double* convertedBlock = new double[nDim*mDim];
      double* convertedU = new double[mDim*mDim];
      double* convertedV  = new double[nDim*nDim];

      double* pos = convertedBlock;
      for (unsigned int i=0; i< nDim; i++) {
            for (unsigned int j=0; j< mDim; j++) {
                  *pos++ = originalBlock[j][i];
            }
      }

      int workArrSize = 5*std::max(mDim, nDim);
      double s[std::min(mDim, nDim)], workArr[workArrSize];

      // SVD mit Block aufrufen
      // https://cpp.hotexamples.com/de/examples/-/-/dgesvd_/cpp-dgesvd_-function-examples.html#0xf71dbdc59dc1ab38f7a86d6f008277708cc941285db6708f1275a020eacb3fe9-177,,209,
      // (int), char, char, int, int, double[lda][*], int, double[*], double[ldu][*], int, double[ldvt][*], int, double[*], int
      // Option 'S' für geringere Dim von U/VT?
      int info = LAPACKE_dgesvd_work(LAPACK_COL_MAJOR, 'A', 'A', mDim, nDim, convertedBlock, mDim, s, convertedU, mDim, convertedV, nDim, workArr, workArrSize);
      // http://www.netlib.org/lapack/double/
      // http://www.netlib.org/lapack/explore-html/d1/d7e/group__double_g_esing.html
      // if (info !=0){
             // std::cerr<<"Lapack error occured in dgesdd. error code :"<<info<<std::endl;
      // }
}


// EntrywiseBlock
template <class datatype>
EntrywiseBlock<datatype>::EntrywiseBlock(datatype ** originalBlock, unsigned int nDim, unsigned int mDim, std::vector<unsigned int> iInd, std::vector<unsigned int> jInd)
      :Block<datatype>::Block(mDim, nDim), iIndices(iInd), jIndices(jInd), block(originalBlock)
{
      // Sonst nix mehr nötig zu machen, muss ja nur 1:1 eingespeichert werden
}


// Helper function for HierarchicalMatrix
template <class datatype>
unsigned int diameter(std::vector<unsigned int> cluster, datatype ** originalMatrix){
      unsigned int maxDistance = 0 /*Erster Wert*/;

      std::for_each(cluster.cbegin(), cluster.cend(), [&maxDistance, cluster, originalMatrix] (const unsigned int ind1) {
            std::for_each(cluster.cbegin(), cluster.cend(), [ind1, &maxDistance, originalMatrix] (const unsigned int ind2) {
                  if(originalMatrix[ind1][ind2] > maxDistance) {
                        maxDistance = originalMatrix[ind1][ind2];
                  }
            });
      });
      return maxDistance;
}
