#include "datafile_op.h"
#include "trie.h"
#include "registro.h"
#include "menu.h"
#include "compara.h"
#include <filesystem>
#include <unistd.h>

int main(){

    setlocale(LC_ALL, "portuguese");

    wcout << L"Bem-vindo ao comparador de orçamentos!" << endl 
          << L"Feito por: Luca Sartori Boni e Thiago Simões Carpinter" << endl << endl;

    inicializacao();

    menu_principal();

    wcout << endl << endl << L"Obrigado por usar o comparador de orçamentos!" << endl;

    sleep(2);

    return 0;
}