# Eingerückter Befehl wird ausgeführt, wenn Datei vor dem Doppelpunkt nicht existent
# ODER sie älter als die genannten Dateien ist

example.exe: example.cc\
	hierarchical_matrices_lib_code/Exceptions.hpp\
	hierarchical_matrices_lib_code/Block.hpp\
	hierarchical_matrices_lib_code/EntrywiseBlock.hpp\
	hierarchical_matrices_lib_code/OuterProductBlock.hpp\
	hierarchical_matrices_lib_code/HierarchicalMatrix.hpp\
	hierarchical_matrices_lib_code/functions/addition.cpp\
	hierarchical_matrices_lib_code/functions/coarse.cpp\
	hierarchical_matrices_lib_code/functions/constructor.cpp\
	hierarchical_matrices_lib_code/functions/inversion.cpp\
	hierarchical_matrices_lib_code/functions/luDecomposition.cpp\
	hierarchical_matrices_lib_code/functions/matrixMatrixMult.cpp\
	hierarchical_matrices_lib_code/functions/matrixVectorMult.cpp
# -I = Pfad zu den h-(bzw include-)Dateien von LaPack
# -L = Pfad zu \lib-Ordner von MinGW
# Ansonsten ist der Kompilier-Befehl bei allen Developern gleich!
	g++ -IC:\MinGW\include\lapack-cmake-build -LC:\MinGW\lib -std=c++17 -Wall -Wno-return-type -Wno-unused-variable \
	example.cc -o example.exe -llapacke -llapack -lblas

clean:
	del test.exe *.o
