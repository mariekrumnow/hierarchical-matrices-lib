#ifndef HIERARCHICAL_MATRICES_CALCRANK_H
#define HIERARCHICAL_MATRICES_CALCRANK_H

// Berechnung des Rangs (k) des Blocks via
// https://www.geeksforgeeks.org/program-for-rank-of-matrix/

template <class datatype>
unsigned int calcRank(unsigned int newMdim, unsigned int newNdim, datatype** cutMatrix){  

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
                            datatype mult = copy[col][row] / copy[row][row];
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
    } 
    
}

#endif //HIERARCHICAL_MATRICES_CALCRANK_H