#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

#include <iostream>
#include <vector>

// Bebendorf p.69/70, p.72-74

// TODO: Statt Rang der Matrix berechnen den Rang aus den Singulärwerten bestimmen?
// --> Kein Rang übergeben/vorher berechnen, erst SVD machen, aus Sigma den Rang bestimmen, dann wie normal im Konstruktor weiter
// Gegeben: convertedX[std::min(mDim, nDim)]
//
// datatype compare = accuracy * convertedX[0];
// unsigned int minIndice = std::min(mDim, nDim);
// for(int a=1; a < std::min(mDim, nDim); a++){
//       if( (convertedX[a] < compare) && (a-1 < minIndice) ){
//             minIndice = a-1;
//       }
// }
// if( minIndice == std::min(mDim, nDim) ){
//       // no rank found
// }


// HierarchicalMatrix
template <class datatype>
void HierarchicalMatrix<datatype>::coarse( double accuracy ){
      if( accuracy <= 0.0 || accuracy >= 1.0 ){
            // Can't be negative, 0 would be the matrix itself & everything above 1 is too far from "approximated"
            std::cerr << "Invalid or non-sensible accuracy chosen" << std::endl;
            return;
      }

      for(int a=0; a<2; a++){
            for(int b=0; b<2; b++){
                  if( matrix[a][b] != nullptr ){
                        matrix[a][b]->coarse(accuracy, true);
                  }
            }
      }
}


template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::coarse( double accuracy, bool checkForLeaf ){
      if(checkForLeaf){
            bool coarsable = true;

            // Existing address returned = leaf, nullptr = HM, nothing changed
            // If not all matrices are leaves, coarsening was already not possible on a deeper level
            for(int a=0; a<2; a++){
                  for(int b=0; b<2; b++){
                        if( matrix[a][b] != nullptr ){
                              Block<datatype>* temp = matrix[a][b]->coarse(accuracy, true);
                              if( temp == nullptr ){
                                    // Returned from an HM, not a leaf
                                    coarsable = false;
                              }
                              else{
                                    // Returned from a EW/OP, leaf
                                    if( matrix[a][b] != temp ){
                                          // If it wasn't a leaf already, save the new coarsened Block
                                          Block<datatype>* tempToDelete = matrix[a][b];
                                          matrix[a][b] = temp;
                                          delete tempToDelete;
                                    }
                              }
                        }
                  }
            }

            if( coarsable ){
                  // Agglomerate (add) all leaves
                  Block<datatype>* agglo1;
                  Block<datatype>* agglo2 = nullptr;
                  // There's either a 2x1 division or a 1x2 division (if not 2x2), always including [0][0] but never [1][1]
                  if( matrix[1][1] != nullptr ){
                        agglo1 = *matrix[0][0] + matrix[0][1];
                        agglo2 = *matrix[1][0] + matrix[1][1];
                  }
                  else if( matrix[0][1] == nullptr ){
                        agglo1 = *matrix[0][0] + matrix[1][0];
                  }
                  else if( matrix[1][0] == nullptr ){
                        agglo1 = *matrix[0][0] + matrix[0][1];
                  }

                  if( agglo2 != nullptr ){
                        agglo1 = *agglo1 + agglo2;
                  }

                  // Blockwise coarse (SVD) the resulting Block if it isn't an OP Block already
                  Block<datatype>* coarsenedBlock = agglo1->coarse(accuracy, false);

                  // Was storage reduced by putting the leaves into one Block?
                  unsigned int currStorageCost = 0;
                  for(int a=0; a<2; a++){
                        for(int b=0; b<2; b++){
                              if( matrix[a][b] != nullptr ){
                                    currStorageCost += matrix[a][b]->getStorageOrRank(true);
                              }
                        }
                  }
                  coarsable = coarsenedBlock->getStorageOrRank(false)*( this->mDim + this->nDim ) <= currStorageCost;

                  if( coarsable ){
                        return coarsenedBlock;
                  }
                  else{
                        return nullptr;
                  }
            }
            else{
                  return nullptr;
            }
      }
      else{
            std::cerr << "Logic Error, this should only be possible to be called with EW/OP Blocks" << std::endl;
            return this;
      }
}


// OuterProductBlock
template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::coarse( double accuracy, bool checkForLeaf ){
      return this;
}


// EntrywiseBlock
template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::coarse( double accuracy, bool checkForLeaf ){
      if(checkForLeaf){
            return this;
      }
      else{
            unsigned int newk = calcRank(this->mDim, this->nDim, this->block);

            std::vector<unsigned int> iInd;
            iInd.push_back(this->indiceRange[kRangeI][kBottom]);
            iInd.push_back(this->indiceRange[kRangeI][kTop]);

            std::vector<unsigned int> jInd;
            jInd.push_back(this->indiceRange[kRangeJ][kBottom]);
            jInd.push_back(this->indiceRange[kRangeJ][kTop]);

            return new OuterProductBlock<datatype>(this->block, this->mDim, this->nDim, iInd, jInd, newk);
      }
}


// Helper function that returns storage used by matrix representation
template <class datatype>
unsigned int HierarchicalMatrix<datatype>::getStorageOrRank( bool getStorage ){
      std::cerr << "Logic Error, Tried to get storage or rank from non-leaf Block" << std::endl;
      return 0;
}

template <class datatype>
unsigned int OuterProductBlock<datatype>::getStorageOrRank( bool getStorage ){
      if(getStorage){
            return k*( this->mDim + this->nDim );
      }
      else{
            return k;
      }
}

template <class datatype>
unsigned int EntrywiseBlock<datatype>::getStorageOrRank( bool getStorage ){
      if(getStorage){
            return this->mDim * this->nDim;
      }
      else{
            std::cerr << "Logic Error, this should be called from OP" << std::endl;
            return 0;
      }
}
