# Eingerückter Befehl wird ausgeführt, wenn Datei vor dem Doppelpunkt nicht existent
# ODER sie älter als die genannten Dateien ist

test.exe: test.cc Exceptions.hpp\
	EntrywiseBlock.hpp OuterProductBlock.hpp HierarchicalMatrix.hpp\
	EntrywiseBlock.cpp OuterProductBlock.cpp\
	functions/addition.cpp functions/coarse.cpp functions/constructor.cpp functions/inversion.cpp functions/luDecomposition.cpp functions/matrixMatrixMult.cpp functions/matrixVectorMult.cpp
	g++ -Wall -std=c++17  test.cc  -o test.exe


clean:
	del test.exe *.o
