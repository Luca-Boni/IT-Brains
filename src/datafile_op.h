#ifndef DATAFILEOP
#define DATAFILEOP

#include "registro.h"
#include "trie.h"
#include <filesystem>
#include <cwchar>
#include <unordered_map>

void deleteDirectoryContents(const string& dir_path);
void deleteTRIEs(void);
void add_Registro_to_vector(vector<Registro>& registros, ifstream& file, int& n_registros, unordered_map<Categoria, TRIE>& tries);
int convert_csv_bin(string file_name);
Registro binarysearch_data_id(string file_name, int id);
Registro directsearch_data_id(string file_name, int id);
//int find_duplicate(vector<Registro>& registros, Registro& registro, unordered_map<Categoria, TRIE>& tries);
int find_duplicate(vector<Registro>& registros, Registro& registro);
void load_trie_trees(unordered_map<Categoria, TRIE>& tries);

#endif
