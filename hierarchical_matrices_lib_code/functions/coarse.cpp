#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

// S.69/70, S.72-74

// FRAGE: Lieber gecoarsened Matrix als Rückgabetyp oder nur in der Fkt geändert mit der Annahme, dass es nicht in einem einzigen Block endet?

// TODO: ggf EW in Konstruktor den Rang übergeben & auch als Attr speichern !! nur wenn Rang für Storage oder neuen Rang wichtig
// --> allen Block eine unsigned int getRank()-Fkt geben, bei EW/OP einfach return k, bei HM ui rank=0/for/notnull/+=matrix[a][b]->getRank()/return rank;
// --> alternativ einfach storage-Fkt, die für HM Fehler, in anderen beiden m*n bzw k*(m+n) zurückgibt

// Schnittstellenfkt nötig, da sonst nullptr zurückgegeben

// Man schaut sich die Blätter an, wenn diese weniger Speicher verbrauchen als wenn man sie zsmfasst (coarse),
// agglomeriert man die Blätter bis auf die Genauigkeit, die angegeben ist
// und geht in die nächste Ebene darüber

// FRAGE: Wie soll man den Rang der Matrix wissen, die man ja erst coarsen müsste? S.72 2.14
// S.69 der resulting rank k' wird estimated? Warum steht in der Formel dann k? S.14?

// FRAGE: Wir können doch nicht assumen, dass da nur lowrank = OP-Blöcke sind, soll an der Stelle der Storage vom EW summiert werden?

// FRAGE: Blockwise coarsening 1.1.3 S.13 vs 1.1.6 S.18 Agglomeration, wo ist der Unterschied? Oder agglo vor blockwise c?
// einfach wieder zu einner Matrix zsmfügen = Addition, SVD machen und das wars?

// HierarchicalMatrix
template <class datatype>
void HierarchicalMatrix<datatype>::coarse( double accuracy ){
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
                              coarsable = false;
                        }
                        else{
                            if( matrix[a][b] != temp ){
                                Block<datatype>* tempToDelete = matrix[a][b];
                                matrix[a][b] = temp;
                                delete tempToDelete;
                            }
                        }
                  }
            }
      }

      if( coarsable ){
          // Would storage be reduced if we were to put the leaves into one Block?
          unsigned int newRank = 1; //Estimate??
          unsigned int currStorageCost = 0;
          for(int a=0; a<2; a++){
                for(int b=0; b<2; b++){
                      if( matrix[a][b] != nullptr ){
                            //currStorageCost += matrix[a][b]->getStorage();
                      }
                }
          }
        coarsable = newRank*( this->mDim + this->nDim ) <= currStorageCost;

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
                //temp1->actualCoarse(); // Bei OP nix machen, bei HM Fehler, bei EW SVD benutzen (aus Konstruktorr auslagern?)
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
