#ifndef COMPARA
#define COMPARA

#include "registro.h"
#include "trie.h"
#include "datafile_op.h"
#include "menu.h"

using namespace std;

typedef tuple<int, int, double> DIFERENCA;

void compara_regs(vector<Registro> registros);
tuple<int, int> find_ids(vector<Registro> registros, int& id_antigo_i, int& id_antigo_f, int& id_novo_i, int& id_novo_f);
vector<DIFERENCA> find_match(vector<Registro> registros, unordered_map<Categoria, TRIE>&tries, int& id_antigo_i, int& id_antigo_f, int& id_novo_i, int& id_novo_f);
void print_diferenca(DIFERENCA& diferencas, vector<Registro>& registros);
void menu_print_diferencas(vector<DIFERENCA>& diferencas, vector<Registro>& registros, tuple<int, int> anos);

#endif