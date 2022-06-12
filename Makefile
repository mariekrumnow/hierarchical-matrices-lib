# Eingerückter Befehl wird ausgeführt, wenn Datei vor dem Doppelpunkt nicht existent
# ODER sie älter als die genannten Dateien ist

test.exe: test.cc Exceptions.hpp Block.hpp\
	EntrywiseBlock.hpp OuterProductBlock.hpp HierarchicalMatrix.hpp\
	functions/addition.cpp functions/coarse.cpp functions/constructor.cpp functions/inversion.cpp functions/luDecomposition.cpp functions/matrixMatrixMult.cpp functions/matrixVectorMult.cpp
# -I = Pfad zu den h-(bzw include-)Dateien von LaPack
# -L = Pfad zu \lib-Ordner von MinGW
# Ansonsten ist der Kompilier-Befehl bei allen Developern gleich!
	g++ -Wall -Wno-return-type -std=c++17 -IC:\MinGW\include\lapack-cmake-build -LC:\MinGW\lib -llapacke -llapack -lblas -lgfortran test.cc  -o test.exe

clean:
	del test.exe *.o
