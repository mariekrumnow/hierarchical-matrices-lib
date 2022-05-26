# Eingerückter Befehl wird ausgeführt, wenn Datei vor dem Doppelpunkt nicht existent
# ODER sie älter als die genannten Dateien ist

test.exe: test.cc
	g++ -Wall -std=c++17  test.cc  -o test.exe


clean:
	del test.exe *.o
