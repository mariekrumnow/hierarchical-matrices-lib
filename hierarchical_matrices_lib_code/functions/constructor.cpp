#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"
#include "calcRank.hpp"

#include "../../user_settings.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <lapacke.h>
#include <limits>
#include <queue>


// Bugfix: LaPack function prohibits executable from being able to run, possibly 32 vs 64 bit error
// Testing: constructHierarchicalMatrix, OuterProductBlock constructor


// Helper function for constructor, defined at bottom
unsigned int diameter(std::vector<unsigned int> cluster, unsigned int ** distances);

// HierarchicalMatrix
template <class datatype>
HierarchicalMatrix<datatype>::HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int dim, double clusterParamEta)
      :Block<datatype>::Block(dim, dim)
{
      if( clusterParamEta <= 0.0 || clusterParamEta >= 1.0 ){
            std::cerr << "Invalid or non-sensible cluster parameter chosen" << std::endl;
            return;
      }

      // Save number of vectors (= num of Blocks)
      unsigned int indices[2][2] = { {1, originalIndices->size()},
                                    {1, originalIndices->size()} };

      Block<datatype>::indiceRange[kRangeI][kBottom] = 0;
      Block<datatype>::indiceRange[kRangeI][kTop] = dim-1;
      Block<datatype>::indiceRange[kRangeJ][kBottom] = 0;
      Block<datatype>::indiceRange[kRangeJ][kTop] = dim-1;

      // Construct matrix graph from existing vertices
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

      // Calculate distance between all nodes (Moore algorithm)
      unsigned int ** distance = new unsigned int*[dim];
      for(a=0; a < dim; a++){
            distance[a] = new unsigned int[dim];
            for(b=0; b < dim; b++){
                  distance[a][b] = ( a==b ? 0 : std::numeric_limits<unsigned int>::max() );
            }
      }

      for(a=0; a < dim; a++){ // Every node has to be the one to calcute the distance from once
            std::queue<unsigned int> nextNode;
            nextNode.push(a);
            unsigned int next;
            while( nextNode.size() !=0 ){
                  next = nextNode.front();
                  nextNode.pop();
                  if( node[next] != 0 ){ // Is there even a vertice to this node?
                        unsigned int nextVertice = node[next] -1; // First guaranteed vertice in table
                        do{
                              unsigned int endNode = vertices[1][nextVertice];
                              if( distance[a][endNode] == std::numeric_limits<unsigned int>::max() ){
                                    distance[a][endNode] = distance[a][next] +1;
                                    nextNode.push(endNode);
                              }
                              nextVertice++;
                        }while( vertices[0][nextVertice] == next ); // starting node still needs to be the correct one
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
      delete[] distance;
}


template <class datatype>
HierarchicalMatrix<datatype>::HierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int indices[2][2], double clusterParamEta, unsigned int ** distances)
      :Block<datatype>::Block(0, 0)
{
      // std::cout << std::endl << indices[kRangeI][kBottom] << " < " << indices[kRangeI][kTop];
      // std::cout << std::endl << indices[kRangeJ][kBottom] << " < " << indices[kRangeJ][kTop] << std::endl;

      // Calculate dimension of contained Blocks
      unsigned int vectorIndice = 1;
      std::list<std::vector<unsigned int>>::iterator currentVector = originalIndices->begin();
      while( currentVector != originalIndices->end() ) {
            if( indices[kRangeI][kBottom] <= vectorIndice && vectorIndice <= indices[kRangeI][kTop] ) { // Wenn Indize des Vektors im Indize-Bereich liegt
                  Block<datatype>::mDim += currentVector->size();
                  // std::cout << std::endl << currentVector->size();
            }
            if( indices[kRangeJ][kBottom] <= vectorIndice && vectorIndice <= indices[kRangeJ][kTop] ) {
                  Block<datatype>::nDim += currentVector->size();
                  // std::cout << std::endl << currentVector->size();
            }
            if( vectorIndice == indices[kRangeI][kBottom] ){
                  Block<datatype>::indiceRange[kRangeI][kBottom] = currentVector->front();
            }
            if( vectorIndice == indices[kRangeI][kTop] ){
                  Block<datatype>::indiceRange[kRangeI][kTop] = currentVector->back();
            }
            if( vectorIndice == indices[kRangeJ][kBottom] ){
                  Block<datatype>::indiceRange[kRangeJ][kBottom] = currentVector->front();
            }
            if( vectorIndice == indices[kRangeJ][kTop] ){
                  Block<datatype>::indiceRange[kRangeJ][kTop] = currentVector->back();
            }
            currentVector++;
            vectorIndice++;
      }
      // std::cout << std::endl << "m: " << Block<datatype>::mDim << "  n: " << Block<datatype>::nDim << std::endl;

      constructHierarchicalMatrix(originalMatrix, originalIndices, indices, clusterParamEta, distances);
}


template <class datatype>
void HierarchicalMatrix<datatype>::constructHierarchicalMatrix(datatype ** originalMatrix, std::list<std::vector<unsigned int>>* originalIndices, unsigned int indices[2][2], double clusterParamEta, unsigned int ** distances)
{
      // Middle of the indices to calculate division into quadrants
      unsigned int iMiddle = indices[kRangeI][kBottom] + floor((indices[kRangeI][kTop]-indices[kRangeI][kBottom]) /2);
      unsigned int jMiddle = indices[kRangeJ][kBottom] + floor((indices[kRangeJ][kTop]-indices[kRangeJ][kBottom]) /2);
      // std::cout  << std::endl << "i: " << indices[kRangeI][kBottom] << " - " << indices[kRangeI][kTop] << " Mitte: " << iMiddle << std::endl;
      // std::cout << "j: " << indices[kRangeJ][kBottom] << " - " << indices[kRangeJ][kTop] << " Mitte: " << jMiddle << std::endl;

      // Making sure a 3x2 block won't unnecessarily be split across the x2
      if( indices[kRangeI][kBottom] == iMiddle && iMiddle+1 == indices[kRangeI][kTop]
                  && indices[kRangeJ][kBottom]+1 == jMiddle && jMiddle+1 == indices[kRangeJ][kTop] ){
            iMiddle = indices[kRangeI][kTop];
      }
      else if( indices[kRangeJ][kBottom] == jMiddle && jMiddle+1 == indices[kRangeJ][kTop]
                  && indices[kRangeI][kBottom]+1 == iMiddle && iMiddle+1 == indices[kRangeI][kTop] ){
            jMiddle = indices[kRangeJ][kTop];
      }

      // Check if there's only one indice left in vectors --> Leaf reached
      bool splittable[2][2] = {
            { !(indices[kRangeI][kBottom] == iMiddle && indices[kRangeJ][kBottom] == jMiddle),
                  !(indices[kRangeI][kBottom] == iMiddle && jMiddle+1 == indices[kRangeJ][kTop]) },
            { !(iMiddle+1 == indices[kRangeI][kTop] && indices[kRangeJ][kBottom] == jMiddle),
                  !(iMiddle+1 == indices[kRangeI][kTop] && jMiddle+1 == indices[kRangeJ][kTop]) }   };

      // Check if not all 4 Blocks can be built and thus invalid ones need to be skipped in for-loop --> If 1x1 it wouldn't be split!
      unsigned int maxBlockI = 2;
      unsigned int maxBlockJ = 2;
      if( iMiddle+1 > indices[kRangeI][kTop] ) {
            maxBlockI = 1;
      }
      else if( jMiddle+1 > indices[kRangeJ][kTop] ) { // Just 1 indice for j --> only 2x1 Blocks can be built
            maxBlockJ = 1;
      }
      // std::cout << maxBlockI << " " << maxBlockJ << " ";

      unsigned int a, b;
      for (a=0; a<2; a++) {
            for (b=0; b<2; b++) {
                  matrix[a][b] = nullptr;
            }
      }

      for (a=0; a< maxBlockI; a++) {
            for (b=0; b< maxBlockJ; b++) {
                  if (splittable[a][b]) {
                        // std::cout << "HM ";
                        // Split indices
                        unsigned int newInd[2][2];
                        if( a==0 ) {
                              newInd[kRangeI][kBottom] = indices[kRangeI][kBottom];
                              newInd[kRangeI][kTop] = iMiddle;
                        }
                        else {
                              newInd[kRangeI][kBottom] = iMiddle+1;
                              newInd[kRangeI][kTop] = indices[kRangeI][kTop];
                        }

                        if( b==0 ) {
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
                        // Find vector by its indice
                        unsigned int iVectorNum = a==0 ? iMiddle : iMiddle+1;
                        std::list<std::vector<unsigned int>>::iterator iVector = originalIndices->begin();
                        while( iVectorNum != 1 ) {
                              iVector++;
                              iVectorNum--;
                        }

                        unsigned int jVectorNum = b==0 ? jMiddle : jMiddle+1;
                        std::list<std::vector<unsigned int>>::iterator jVector = originalIndices->begin();
                        while( jVectorNum != 1 ) {
                              jVector++;
                              jVectorNum--;
                        }

                        unsigned int newMdim = iVector->size();
                        unsigned int newNdim = jVector->size();
                        // std::cout << " " << iVector->size() << " " << jVector->size() << " ";

                        // Copy corresponding indices from the original matrix
                        datatype ** cutMatrix = new datatype*[newMdim];
                        for(unsigned int c=0; c < newMdim; c++){
                              cutMatrix[c] = new datatype[newNdim];

                              for(unsigned int d=0; d < newNdim; d++){
                                    cutMatrix[c][d] = originalMatrix[ (*iVector)[c] ][ (*iVector)[d] ];
                              }
                        }

                        unsigned int k = calcRank<datatype>(newMdim, newMdim, cutMatrix);

                        if ( k*(newMdim + newNdim) < newMdim*newNdim ) { // == Low-rank matrix
                              // std::cout << "OP1 ";
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
                                    // Matrix can be approximated by low-rank one
                                    // std::cout << "OP2 ";
                                    matrix[a][b] = new OuterProductBlock<datatype>(cutMatrix, newMdim, newNdim, *iVector, *jVector, k);
                              }
                              else{
                                    // Important info will be lost by approximation, has to be saved with more effort (non-admissible)
                                    // std::cout << "EW ";
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
      :Block<datatype>::Block(mDim, nDim), k(rank)
{
      Block<datatype>::indiceRange[kRangeI][kBottom] = iInd.front();
      Block<datatype>::indiceRange[kRangeI][kTop] = iInd.back();
      Block<datatype>::indiceRange[kRangeJ][kBottom] = jInd.front();
      Block<datatype>::indiceRange[kRangeJ][kTop] = jInd.back();

      datatype* convertedBlock = new datatype[nDim*mDim];
      datatype* convertedU = new datatype[mDim*mDim];
      datatype* convertedV  = new datatype[nDim*nDim];
      datatype* convertedX = new datatype[std::min(mDim, nDim)];

      datatype* pos = convertedBlock;
      unsigned int a, b;
      for (a=0; a< nDim; a++) {
            for (b=0; b< mDim; b++) {
                  *pos++ = originalBlock[b][a];
            }
      }

      unsigned int workArrSize = 5*std::max(mDim, nDim);
      datatype* workArr = new datatype[workArrSize];

      // Call SVD with the Block
      // example: https://cpp.hotexamples.com/de/examples/-/-/dgesvd_/cpp-dgesvd_-function-examples.html#0xf71dbdc59dc1ab38f7a86d6f008277708cc941285db6708f1275a020eacb3fe9-177,,209,
      // documentation: http://www.netlib.org/lapack/explore-html/d1/d7e/group__double_g_esing.html
      // int info = CALC_SVD(LAPACK_COL_MAJOR, 'A', 'A', mDim, nDim, convertedBlock, mDim, convertedX, convertedU, mDim, convertedV, nDim, workArr, workArrSize);
      // if(info){
      //        std::cerr << "Lapack error occured in ?gesvd. Error code: " << info << std::endl;
      // }

      // Transform attributes back from LaPack format
      // --> Just throw out all rows over rank k
      pos = convertedU;
      u = new datatype*[mDim];
      for(a=0; a < mDim; a++){
            u[a] = new datatype[k];
            for (b=0; b < k; b++){
                  u[b][a] = *pos++;
            }
      }

      pos = convertedX;
      x = new datatype*[k];
      for(a=0; a < k; a++){
            x[a] = new datatype[k];
            for (b=0; b < k; b++){
                  x[a][b] = 0;
            }
            x[a][a] = *pos++;
      }

      pos = convertedV;
      v = new datatype*[nDim];
      for(a=0; a < nDim; a++){
            v[a] = new datatype[k];
            for (b=0; b < k; b++){
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
      :Block<datatype>::Block(mDim, nDim), block(originalBlock)
{
      Block<datatype>::indiceRange[kRangeI][kBottom] = iInd.front();
      Block<datatype>::indiceRange[kRangeI][kTop] = iInd.back();
      Block<datatype>::indiceRange[kRangeJ][kBottom] = jInd.front();
      Block<datatype>::indiceRange[kRangeJ][kTop] = jInd.back();
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
