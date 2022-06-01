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


      // Für ersten Aufruf schauen, ob sich algebraischen Clusterbaum überhaupt aufbauen lässt --> minimal cluster size beachten
      // = ob sich I und J weiter aufteilen lassen
      // (Attr dass dafür true gesetzt, sonst default =false)
      // Falls nein: Fehler, müsste eigentlich als OP/EW gespeichert werden

      // Nächstes Level im algebraischen CLusterbaum ermitteln (aktuelle range von Zeilen/Spalten je einmal aufteilen)

      unsigned int splitI;
      unsigned int splitJ;

      // https://stackoverflow.com/questions/2178909/how-to-initialize-3d-array-in-c
      unsigned int newIndices[2][2][2];

    for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++) {
            bool aufteilbar = true; //Lieber false init?
            
            // Für jede entstehende neue Aufteilungen prüfen, ob sich diese weiter aufteilen lassen --> minimal cluster size beachten
            // Falls ja: matrix für den Quadranten = HierarchicalMatrix-Konstruktor mit neuer Dim/Startspalten usw
            // Falls nein: matrix für den Quadranten = OP(ad) oder EW-Konstruktor(non) je nach Admissibilty?

            if (aufteilbar) {
                matrix[i][j] = new HierarchicalMatrix<datatype, /*neue dim*/>(/*abgekappte Daten*/);
            }
            else {
                bool admissible = true;

                // k = Rang der Matrix? Weil Rang = Maximale Anzahl linear unabhängiger Zeilen/Spalten = k (S.10)
                // Wenn k(xDim+yDim) < xDim*yDim: OuterProduct-Form benutzen

                if (admissible) {
                    matrix[i][j] = new OuterProductBlock<datatype, /*neue dim x2*/>(/*abgekappte Daten*/);
                }
                else {
                    matrix[i][j] = new EntrywiseBlock<datatype, /*neue dim x2*/>(/*abgekappte Daten*/);
                }
            }
        }
    }

}
