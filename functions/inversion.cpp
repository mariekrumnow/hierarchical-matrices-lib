#include "../HierarchicalMatrix.hpp"
#include "../OuterProductBlock.hpp"
#include "../EntrywiseBlock.hpp"

template <class datatype>
HierarchicalMatrix<datatype>& HierarchicalMatrix<datatype>::invert(){

      // Nur in erstem Aufruf: Tiefe Kopie mit der selben Block-Struktur anlegen
      // --> Copy-Konstruktor + darin polymorpher clone-"Konstruktor"(Funktion), siehe https://stackoverflow.com/questions/4122789/copy-object-keep-polymorphism
      // Einfach invert()-Funktion von den matrix-Inhalten aufrufen wo nötig
      // (Keine Prüfung ob "klein genug", da dann auftomatisch normale Invertierung aufgerufen, da dann von EW/OP aufgerufen)
      // --> ruft per Polymorphie direkt die richtige invert-Funktion auf


      // HierarchicalMatrix<double> * invertMatrix = new HierarchicalMatrix<double>(this); // Copy-Konstruktor nötig
      //
      // A11inv = invertH(t,A11,C); //A11inv = A11 invertiert
      // S = A22 - A21 *  A11inv * A12 ; // S = A22 -A21*A11inv *A12 von A11 in A
      //
      // // Die Inverse immer nur einmal berechnen lassen und nicht immer wieder neu, damit Rechenzeit gespart wird (Wie Inv von A und S)
      // // Alle Berechnungen sollen nach Möglichkeit nur einmal durchgeführt werden, deshalb dei Variablen
      // Sinv = invertH(t,S, C); // Sinv = S invertiert
      // A11invA12Sinv = A11inv *A12 *Sinv;
      // A21A11inv = A21 * A11inv;
      //
      // Ainv[1][1] = AA11invA12Sinv * A21A11inv ;  //A11 = A11inv + A11inv * A12 * Sinv * A21 * A11inv
      // Ainv[1][2] = -A11invA12Sinv; // A12 = -A11inv * A12 * Sinv
      // Ainv[2][1] = -Sinv * A21A11inv; // A21 = -Sinv * A21 * A11inv
      // Ainv[2][2] = Sinv // A22 = Sinv
      //
      //
      //
      // HierachlMatrix invertH(t, A, C) // Rekusiver Algorithmus zum Berechnen der Inverse C{H} von A
      // {
      //     if () // Wenn die Matrix klein genug ist dann wird die Normale Berechnung der Inversen Durchgeführt
      //     {
      //         //normaler Algorithmus zum Berechnen einer Inversen (Gaußscher Algorithmus)
      //     }
      //     else
      //     {
      //         invertH(t1,A,C);
      //         T12 = T12 - C11 * A12; //Berechnung wird mit unseren neuen Operatoren durchgeführt
      //         T21 = T21 - A21 * C11; //Matrix T an der Stelle 21 bei einer 2x2 Matrix T[2][1]??
      //         A22 = A22 + A21 * T12;
      //         invertH(t2,A,C);
      //         C12 = C12 + T12 * C22;
      //         C21 = C21 + C22 * T21;
      //         C11 = C11 + T12 * C21;
      //     }
      //
      // }

      //Alles noch nicht kompiliert und nicht funktionsfähig, da mir noch folgende Dinge fehlen:
      // - Operator Multiplikation
      // - Operator Addition + Operator Subtraktion


}
