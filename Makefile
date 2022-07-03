# Eingerückter Befehl wird ausgeführt, wenn Datei vor dem Doppelpunkt nicht existent
# ODER sie älter als die genannten Dateien ist

example.exe: example.cc Exceptions.hpp Block.hpp\
	EntrywiseBlock.hpp OuterProductBlock.hpp HierarchicalMatrix.hpp\
	functions/addition.cpp functions/coarse.cpp functions/constructor.cpp functions/inversion.cpp\
	functions/luDecomposition.cpp functions/matrixMatrixMult.cpp functions/matrixVectorMult.cpp
# -I = Pfad zu den h-(bzw include-)Dateien von LaPack
# -L = Pfad zu \lib-Ordner von MinGW
# Ansonsten ist der Kompilier-Befehl bei allen Developern gleich!
	g++ -IC:\MinGW\include\lapack-cmake-build -LC:\MinGW\lib -std=c++17 -Wall -Wno-return-type -Wno-unused-variable \
	example.cc -o example.exe -llapacke -llapack -lblas

hierarchical_matrices_lib.o: example.cc Exceptions.hpp Block.hpp\
	EntrywiseBlock.hpp OuterProductBlock.hpp HierarchicalMatrix.hpp\
	functions/addition.cpp functions/coarse.cpp functions/constructor.cpp functions/inversion.cpp\
	functions/luDecomposition.cpp functions/matrixMatrixMult.cpp functions/matrixVectorMult.cpp

	g++ -IC:\MinGW\include\lapack-cmake-build -LC:\MinGW\lib -std=c++17 -Wall \
	library.c -c -o hierarchical_matrices_lib.o -llapacke -llapack -lblas

clean:
	del test.exe *.o
