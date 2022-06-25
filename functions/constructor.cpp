#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

#include <cmath>
#include <algorithm>
#include <limits>
#include <queue>

#include <lapacke.h>

#include <iostream>

// TODO: (Destruktor, in dem rekursiv die ganzen Attribute gelöscht werden?)

// TODO: Garantie, dass Diagonalen belegt? (sonst selfconnected löschen)
// TODO: Graph auf Block oder Gesamtmatrix bezogen? Von Matrix A und IxI die Rede, hätte man auch b,t,s nehmen können --> Code verlegen/abändern
// TODO: Wie das Problem mit Datentyp bei Rangberechnung lösen? ~Z.225
// TODO: Wie soll das mit dim k bei SVD-Aufruf gehen?

// TODO: Makro für LA-Fkt mit richtigem Datentyp
// TODO: Testen von Konstruktor [public Konstruktor geht]
// --> 2x2, 4x4 richtig, bei 3x3 wir 2x2 Block statt 1x2 gebildet & es läuft ins nix

// Verbesserung: nullptr-Belegung von matrix mit anderem for-loop vereinen

// Helper function for HierarchicalMatrix, defined at bottom
unsigned int diameter(std::vector<unsigned int> cluster, unsigned int ** distances);

// HierarchicalMatrix
template <class datatype>
HierarchicalMatrix<datatype>::HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int dim, double clusterParamEta)
      :Block<datatype>::Block(dim, dim)
{
      // Anzahl der Vektoren (= Anz Blöcke) speichern
      unsigned int indices[2][2] = { {1, originalIndices->size()},
                                    {1, originalIndices->size()} };

      // Matrix-Graph durch existierende Kanten aufstellen
      std::vector< std::vector<unsigned int> > vertices; // [0] begin & [1] end node for each vertice
      vertices.push_back( std::vector<unsigned int>() );
      vertices.push_back( std::vector<unsigned int>() );

      unsigned int node[dim] = {}; // indice +1 in "vertices" with first occurence of input node (node 0 is always on indice 0 --> node[0]=1)
      bool selfconnected[dim] = {}; // True if diagonal != 0

      unsigned int a, b;
      for (a =0; a < dim; a++){
          for (b =0; b < dim; b++){
              if( originalMatrix[a][b] || originalMatrix[b][a] ){
                  vertices[0].push_back(a);
                  vertices[1].push_back(b);
                  if(node[a] == 0){
                        node[a] = vertices[0].size();
                  }
                  if(a==b){
                        selfconnected[a] = true;
                  }
                  // std::cout << vertices[0].back() << " " << vertices[1].back() << std::endl;
              }
          }
      }
      // for(a =0; a < dim; a++){
      //       std::cout << node[a] << std::endl;
      // }

      // Distanzen zwischen allen Knoten berechnen (Moore)
      unsigned int ** distance = new unsigned int*[dim];
      for(a=0; a < dim; a++){
            distance[a] = new unsigned int[dim];
            for(b=0; b < dim; b++){
                  distance[a][b] = ( a==b ? 0 : std::numeric_limits<unsigned int>::max() );
            }
      }

      for(a=0; a < dim; a++){ // Jeder Knoten muss einmal Knoten sein, zu dem Distanz berechnet wird
            std::queue<unsigned int> nextNode;
            nextNode.push(a);
            unsigned int next;
            while( nextNode.size() !=0 ){
                  next = nextNode.front();
                  nextNode.pop();
                  if( node[next] != 0 ){ // Gibt es zu dem Knoten überhaupt eine Kante?
                        unsigned int nextVertice = node[next] -1; // Erste garantierte Kante in Tabelle
                        do{
                              unsigned int endNode = vertices[1][nextVertice];
                              if( distance[a][endNode] == std::numeric_limits<unsigned int>::max() ){
                                    distance[a][endNode] = distance[a][next] +1;
                                    nextNode.push(endNode);
                              }
                              nextVertice++;
                        }while( vertices[0][nextVertice] == next ); // Startknoten muss noch der richtige sein
                  }
            }
            if( !selfconnected[a] ){
                  distance[a][a] = std::numeric_limits<unsigned int>::max(); // node isn't reachable from itself
            }
      }

      // for(a=0; a < dim; a++){
      //       for(b=0; b < dim; b++){
      //             std::cout << "Distanz von " << a << " zu " << b << ": " << distance[a][b] << std::endl;
      //       }
      // }

      constructHierarchicalMatrix(originalMatrix, originalIndices, indices, clusterParamEta, distance);

      for(a=0; a < dim; a++){
            delete[] distance[a];
      }
}


template <class datatype>
HierarchicalMatrix<datatype>::HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int indices[2][2], double clusterParamEta, unsigned int ** distances)
      :Block<datatype>::Block(0, 0)
{
      // Dimension der enthaltenen Blöcke berechnen
      unsigned int vectorIndice = 1;
      auto currentVector = originalIndices->begin();
      while( currentVector != originalIndices->end() ) {
            if( indices[kRangeI][kBottom] <= vectorIndice && vectorIndice <= indices[kRangeI][kTop] ) { // Wenn Indize des Vektors im Indize-Bereich liegt
                  Block<datatype>::mDim += currentVector->size();
                  std::cout << currentVector->size() << std::endl;
            }
            else if( indices[kRangeJ][kBottom] <= vectorIndice && vectorIndice <= indices[kRangeJ][kTop] ) {
                  Block<datatype>::nDim += currentVector->size();
                  std::cout << currentVector->size() << std::endl;
            }
            currentVector++;
            vectorIndice++;
      }
      std::cout << std::endl << "m: " << Block<datatype>::mDim << "  n: " << Block<datatype>::nDim << std::endl;

      constructHierarchicalMatrix(originalMatrix, originalIndices, indices, clusterParamEta, distances);
}


template <class datatype>
void HierarchicalMatrix<datatype>::constructHierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int indices[2][2], double clusterParamEta, unsigned int ** distances)
{
      // Mitte der Indizes zum Aufteilen in Quadranten berechnen
      unsigned int iMiddle = indices[kRangeI][kBottom] + floor((indices[kRangeI][kTop]-indices[kRangeI][kBottom]) /2);
      unsigned int jMiddle = indices[kRangeJ][kBottom] + floor((indices[kRangeJ][kTop]-indices[kRangeJ][kBottom]) /2);
      // std::cout  << std::endl << "i: " << indices[kRangeI][kBottom] << " - " << indices[kRangeI][kTop] << " Mitte: " << iMiddle << std::endl;
      // std::cout << "j: " << indices[kRangeJ][kBottom] << " - " << indices[kRangeJ][kTop] << " Mitte: " << jMiddle << std::endl;

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

      // std::cout << maxBlockI << " " << maxBlockJ << " ";

      unsigned int a, b;
      for (a=0; a<2; a++) {
            for (b=0; b<2; b++) {
                  matrix[a][b] = nullptr;
            }
      }

      for (a=0; a< maxBlockI; a++) {
            for (b=0; b< maxBlockJ; b++) {
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

                        matrix[a][b] = new HierarchicalMatrix<datatype>(originalMatrix, originalIndices, newInd, clusterParamEta, distances);
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
                        for(unsigned int c=0; c < newMdim; c++){
                              cutMatrix[c] = new datatype[newNdim];

                              for(unsigned int d=0; d < newNdim; d++){
                                    cutMatrix[c][d] = originalMatrix[ (*iVector)[c] ][ (*iVector)[d] ];
                              }
                        }

                        // Berechnung des Rangs (k) des Blocks via
                        // https://www.geeksforgeeks.org/program-for-rank-of-matrix/
                        unsigned int k = newNdim;

                        datatype copy[newMdim][newNdim]; // Kopie wird verändert!
                        for(unsigned int c=0; c< newMdim; c++){
                              for(unsigned int d=0; d < newNdim; d++){
                                    copy[c][d] = cutMatrix[c][d];
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

                                    for (unsigned int i = row + 1; i < newMdim; i++){
                                          if (copy[i][row]) {
                                                for (unsigned int j=0; j < k; j++){
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
                              unsigned int minDistance = std::numeric_limits<unsigned int>::max();
                              std::for_each(iVector->cbegin(), iVector->cend(), [&minDistance, jVector, distances] (const unsigned int ind1) {
                                    std::for_each(jVector->cbegin(), jVector->cend(), [ind1, &minDistance, distances] (const unsigned int ind2) {
                                          if(distances[ind1][ind2] < minDistance && distances[ind1][ind2] != std::numeric_limits<unsigned int>::max()) {
                                                minDistance = distances[ind1][ind2];
                                          }
                                    });
                              });

                              if( std::min(diameter(*iVector, distances), diameter(*jVector, distances)) <= clusterParamEta * minDistance ) {
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

                        for(unsigned int c=0; c < newMdim; c++){
                              delete[] cutMatrix[c];
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
      datatype* convertedBlock = new datatype[nDim*mDim];
      // Größen mit k?
      datatype* convertedU = new datatype[mDim*mDim];
      datatype* convertedV  = new datatype[nDim*nDim];
      // std::min(mDim, nDim) statt k?
      x = new datatype[k];

      datatype* pos = convertedBlock;
      unsigned int a, b;
      for (a=0; a< nDim; a++) {
            for (b=0; b< mDim; b++) {
                  *pos++ = originalBlock[b][a];
            }
      }

      unsigned int workArrSize = 5*std::max(mDim, nDim);
      datatype* workArr = new datatype[workArrSize];

      // SVD mit Block aufrufen
      // https://cpp.hotexamples.com/de/examples/-/-/dgesvd_/cpp-dgesvd_-function-examples.html#0xf71dbdc59dc1ab38f7a86d6f008277708cc941285db6708f1275a020eacb3fe9-177,,209,
      // Option 'S' für geringere Dim von U/VT?
      // int info = LAPACKE_dgesvd_work(LAPACK_COL_MAJOR, 'S', 'S', mDim, nDim, convertedBlock, mDim, x, convertedU, mDim, convertedV, nDim, workArr, workArrSize);
      // http://www.netlib.org/lapack/double/
      // http://www.netlib.org/lapack/explore-html/d1/d7e/group__double_g_esing.html
      // if (info){
      //        std::cerr<<"Lapack error occured in dgesdd. error code :" << info << std::endl;
      // }

      // Attribute aus Format von Lapack rausholen
      pos = convertedU;
      u = new datatype*[mDim];
      for(a=0; a < mDim; a++){
            u[a] = new datatype[k]; // stattdessen mDim?
            for (b=0; b < k; b++){ // stattdessen mDim?
                  u[b][a] = *pos++;
            }
      }

      pos = convertedV;
      v = new datatype*[nDim];
      for(a=0; a < nDim; a++){
            v[a] = new datatype[k]; // stattdessen nDim?
            for (b=0; b < k; b++){ // stattdessen nDim?
                  v[a][b] = *pos++;
            }
      }

      delete[] convertedBlock;
      delete[] convertedU;
      delete[] convertedV;
      delete[] workArr;
}


// EntrywiseBlock
template <class datatype>
EntrywiseBlock<datatype>::EntrywiseBlock(datatype ** originalBlock, unsigned int nDim, unsigned int mDim, std::vector<unsigned int> iInd, std::vector<unsigned int> jInd)
      :Block<datatype>::Block(mDim, nDim), iIndices(iInd), jIndices(jInd), block(originalBlock)
{
      // Sonst nix mehr nötig zu machen, muss ja nur 1:1 eingespeichert werden
}


// Helper function for HierarchicalMatrix
unsigned int diameter(std::vector<unsigned int> cluster, unsigned int ** distances){
      unsigned int maxDistance = 0;

      std::for_each(cluster.cbegin(), cluster.cend(), [&maxDistance, cluster, distances] (const unsigned int ind1) {
            std::for_each(cluster.cbegin(), cluster.cend(), [ind1, &maxDistance, distances] (const unsigned int ind2) {
                  if(distances[ind1][ind2] > maxDistance && distances[ind1][ind2] != std::numeric_limits<unsigned int>::max()) {
                        maxDistance = distances[ind1][ind2];
                  }
            });
      });
      return maxDistance;
}
