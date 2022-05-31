#include "../HierarchicalMatrix.hpp"

template <class datatype, unsigned int dim>
HierarchicalMatrix<datatype,dim>::HierarchicalMatrix(datatype (&originalMatrix)[dim][dim], unsigned int I, unsigned int J)
      :Block<datatype, dim, dim>::Block(I, J)
{
      // Clusterbaum = Zeilen- & Spaltenindizes werden willkürlich, nur auf Balance achtend, so lange unterteilt, bis minimal cluster size erreicht??
      // --> Danach müsste entlang dieser aufgeteilten Blöcke geschaut werden, ob Block Admissible
      // Falls nein: Nicht weiter den Clusterbaum entlang gehen, Entrywise speichern
      // Falls ja: Weiter CLusterbaum entlang gehen, auf nächster Ebene admissibility prüfen
      // Wenn minimal cluster size erreicht: OP-Darstellung nutzen?


      // Schauen, ob sich Clusterbaum weiter aufbauen lässt --> minimal cluster size beachten
      // = ob sich I und J weiter aufteilen lassen
      // Falls ja: 4x HierarchicalMatrix-Konstruktor mit neuer Dim/Startspalten usw
      // if( /*Clusterbaum weiter aufteilbar*/ ) {
      //       matrix
      // }


      /*
      Clusterbaum simulieren, um Partitionierung der Blöcke rauszufinden
      --> Algebraisches Clustern + Admissibility condition (reicht immer selbe) + SVD (LA-Pack einbinden?)

      Als Baum in matrix-Attribut speichern

      Konstruktoren der OuterProduct (Admissible) und Entrywise (non-admissible) Blöcke aufrufen
      */

      // matrix[0][1] = new EntrywiseBlock<datatype, 2, 2>(originalMatrix, 1, 1);
      // matrix[0][2] = new OuterProductBlock<datatype, 2, 2>(originalMatrix, 1, 1);
}
