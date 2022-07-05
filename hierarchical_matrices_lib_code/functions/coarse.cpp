#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

// S.69/70, S.72-74
// TODO: Entweder zweite coarse-Fkt mit double accuracy, sonst accuracy in allen übergeben
// TODO: ggf trotzdem/generell Schnittstellenfkt die auf jeden Fall HM zurück gibt
// TODO: ggf EW in Konstruktor den Rang übergeben & auch als Attr speichern !! nur wenn Rang für Storage oder neuen Rang wichtig
// --> allen Block eine unsigned int getRank()-Fkt geben, bei EW/OP einfach return k, bei HM ui rank=0/for/notnull/+=matrix[a][b]->getRank()/return rank;

// FRAGE: Wie soll man den Rang der Matrix wissen, die man ja erst coarsen müsste? S.72 2.14
// FRAGE: Wir können doch nicht assumen, dass da nur lowrank = OP-Blöcke sind, soll an der Stelle der Storage vom EW summiert werden?

// HierarchicalMatrix
template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::coarse( double accuracy ){
      bool coarsable = true;

      for(int a=0; a<2; a++){
            for(int b=0; b<2; b++){
                  if( matrix[a][b] != nullptr ){
                        if( matrix[a][b]->coarse(accuracy) != nullptr ){
                              // nullptr = es handelt sich um ein Blatt, nur wenn alle Blätter sind macht coarsen Sinn
                              coarsable = false;
                        }
                  }
            }
      }


      // coarsability berechnen, ggf rekursiv
      unsigned int newRank = ???;
      unsigned int currStorageCost = 0;
      for(int a=0; a<2; a++){
            for(int b=0; b<2; b++){
                  if( matrix[a][b] != nullptr ){
                        currStorageCost += matrix[a][b]->getRank() * ( matrix[a][b]->mDim + matrix[a][b]->nDim );
                  }
            }
      }
      if( newRank*( mDim + nDim ) <= currStorageCost){
            coarsable = true;
      }

      if( coarsable ){
            // + ersetzen durch individuelle Funktion?
            Block<datatype>* temp1, temp2; // Ist das bei init nullptr oder was anderes?
            if( matrix[1][1] != nullptr ){ // There's either a 2x1 division or a 1x2 division, always including [0][0] but never [1][1]
                  temp1 = matrix[0][0] + matrix[0][1];
                  temp2 = matrix[1][0] + matrix[1][1];
            }
            else if( matrix[0][1] == nullptr ){
                  temp1 = matrix[0][0] + matrix[1][0];
            }
            else if( matrix[1][0] == nullptr ){
                  temp1 = matrix[0][0] + matrix[0][1];
            }

            if( temp2 != nullptr ){
                  temp1 = temp1 + temp2;
            }

            // temp1 reduzieren/coarsen

            return temp1;
      }
      else{
            for(int a=0; a<2; a++){
                  for(int b=0; b<2; b++){
                        if( matrix[a][b] != nullptr ){
                              matrix[a][b]->coarse(accuracy);
                        }
                  }
            }
            return this;
      }
}


// OuterProductBlock
template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::coarse( double accuracy ){
      return nullptr;
}


// EntrywiseBlock
template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::coarse( double accuracy ){
      return nullptr;
}
