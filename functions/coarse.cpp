#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

// TODO: Entweder zweite coarse-Fkt mit double accuracy, sonst accuracy in allen übergeben
// TODO: ggf trotzdem/generell SSchnittstellenfkt die auf jeden Fall HM zurück gibt
// TODO: muss ne neue HM erstellt werden oder lässt sich die bestehende nutzen?

// HierarchicalMatrix
template <class datatype>
Block<datatype>* HierarchicalMatrix<datatype>::coarse( double accuracy ){
    for(int a=0; a<2; a++){
          for(int b=0; b<2; b++){
                *matrix[a][b]..coarse(accuracy);
          }
    }
}


// OuterProductBlock
template <class datatype>
Block<datatype>* OuterProductBlock<datatype>::coarse( double accuracy ){

}


// EntrywiseBlock
template <class datatype>
Block<datatype>* EntrywiseBlock<datatype>::coarse( double accuracy ){

}
