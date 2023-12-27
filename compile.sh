cd src
g++ -c datafile_op.cpp
g++ -c registro.cpp
g++ -c trie.cpp
g++ -c menu.cpp
g++ -c main.cpp
g++ -c compara.cpp
g++ -o ../Trabalho.exe main.o datafile_op.o registro.o trie.o menu.o compara.o
cd ..