#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

#include <iostream>

// S.69/70, S.72-74

// Erst coarsen, dann neuen Rang prüfen und gucken ob das Sinn macht
// wo kommt da jetzt die accuracy rein?

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
                        matrix[a][b]->tryCoarse(accuracy);
                  }
            }
      }
}


template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::tryCoarse( double accuracy ){
      bool coarsable = true;

      // Existing address returned = leaf, nullptr = HM, nothing changed
      // If not all matrices are leaves, coarsening was already not possible on a deeper level
      for(int a=0; a<2; a++){
            for(int b=0; b<2; b++){
                  if( matrix[a][b] != nullptr ){
                        Block<datatype>* temp = matrix[a][b]->tryCoarse(accuracy);
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
            Block<datatype>* temp1;
            Block<datatype>* temp2 = nullptr;
            // There's either a 2x1 division or a 1x2 division (if not 2x2), always including [0][0] but never [1][1]
            if( matrix[1][1] != nullptr ){
                  temp1 = *matrix[0][0] + matrix[0][1];
                  temp2 = *matrix[1][0] + matrix[1][1];
            }
            else if( matrix[0][1] == nullptr ){
                  temp1 = *matrix[0][0] + matrix[1][0];
            }
            else if( matrix[1][0] == nullptr ){
                  temp1 = *matrix[0][0] + matrix[0][1];
            }

            if( temp2 != nullptr ){
                  temp1 = *temp1 + temp2;
            }

            // Blockwise coarse (SVD) the resulting Block if it's not a OP already
            temp1->actualCoarse(); // Bei OP nix machen, bei HM Fehler, bei EW SVD benutzen (aus Konstruktor auslagern?)

            // Was storage reduced by putting the leaves into one Block?
            unsigned int newRank = 1; //Estimate??
            unsigned int currStorageCost = 0;
            for(int a=0; a<2; a++){
                  for(int b=0; b<2; b++){
                        if( matrix[a][b] != nullptr ){
                              currStorageCost += matrix[a][b]->getStorage();
                        }
                  }
            }
            coarsable = newRank*( this->mDim + this->nDim ) <= currStorageCost;

            if( coarsable ){
                  return temp1;
            }
            else{
                  return nullptr;
            }
      }
      else{
            return nullptr;
      }
}


// OuterProductBlock
template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::tryCoarse( double accuracy ){
      return this;
}


// EntrywiseBlock
template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::tryCoarse( double accuracy ){
      return this;
}


// Helper function that returns storage used by matrix representation
template <class datatype>
unsigned int HierarchicalMatrix<datatype>::getStorage(){
      std::cerr << "Tried to get Storage from non-leaf Block" << std::endl;
      return 0;
}

template <class datatype>
unsigned int OuterProductBlock<datatype>::getStorage(){
      return k*( mDim + nDim );
}

template <class datatype>
unsigned int EntrywiseBlock<datatype>::getStorage(){
      return mDim * nDim;
}
