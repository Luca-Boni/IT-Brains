#ifndef MENU
#define MENU

#include "registro.h"
#include "trie.h"
#include "datafile_op.h"
#include "compara.h"

using namespace std;

void print_pagina(vector<Registro>& registros, int pagina, Categoria categoria, string substring, bool reverse);
void menu_print_pagina(vector<Registro>& resultado, int cat, string substring);
void busca_categoria(void);
void inicializacao(void);
void menu_principal(void);

#endif