#include "menu.h"

void print_pagina(vector<Registro>& registros, int pagina, Categoria categoria, string substring, bool reverse){
    
    system("cls");

    if(substring != "TodosRegistros"){

        cout << "Busca pela categoria \'";
        switch(categoria){
            case Categoria::orgao_superior:
                wcout << L"Órgão Superior";
                break;
            case Categoria::orgao_subordinado:
                wcout << L"Órgão Subordinado";
                break;
            case Categoria::acao:
                wcout << L"Ação";
                break;
            case Categoria::grupo_despesa:
                wcout << L"Grupo de Despesa";
                break;
            case Categoria::elemento_despesa:
                wcout << L"Elemento de Despesa";
                break;
        }

        cout << "\' com o termo \'" << substring << "\'" << endl << endl;
    }else{
        cout << "Exibindo todos os registros." << endl << endl;
    }

    print_cabecalho();

    int total_paginas = registros.size()/20 + 1;

    if(reverse){
        for(int i = (total_paginas-pagina+1)*20-1; i >= (total_paginas-pagina)*20; i--){
            if(i >= 0 && i < registros.size())
                registros[i].print();
        }
    }else{
        for(int i = (pagina-1)*20; i < pagina*20; i++){
            if(i < registros.size())
                registros[i].print();
        }
    }
    
    wcout << endl << L"Página " << pagina << " de " << total_paginas << endl;
}

void menu_print_pagina(vector<Registro>& resultado, int cat, string substring){
    string navegacao = {'\0'};
        int pagina=1;
        bool reverse = false;
        while(navegacao!="q"){
            print_pagina(resultado, pagina, static_cast<Categoria>(cat-1), substring, reverse);
            wcout << L"Digite 'n' para a próxima página, 'p' para a página anterior, 'r' para reverter a ordem, um número de página, ou 'q' para sair da consulta: ";
            fflush(stdin);
            cin >> navegacao;
            while(navegacao!="n" && navegacao!="p" && navegacao!="q" && navegacao!="r" && stoi(navegacao) < 1 && stoi(navegacao) > resultado.size()/20 + 1){
                wcout << L"Opção inválida. Digite novamente: ";
                fflush(stdin);
                cin >> navegacao;
            }
            if(navegacao == "n"){
                if(pagina < resultado.size()/20 + 1){
                    pagina++;
                }
            }else if(navegacao == "p"){
                if(pagina > 1)
                    pagina--;
            }else if(navegacao == "r"){
                reverse = !reverse;
                pagina = 1;
            }else if(navegacao != "q"){
                pagina = stoi(navegacao);
            }
        }
}

void busca_categoria(void){

    ifstream data;
    data.open("./data/data.bin", ios::in | ios::binary);

    system("cls");
    wcout << L"Opções:" << endl
         << L"1 - Órgão Superior" << endl
         << L"2 - Órgão Subordinado" << endl
         << L"3 - Ação" << endl
         << L"4 - Grupo de Despesa" << endl
         << L"5 - Elemento de Despesa" << endl
         << L"Digite a categoria que gostaria de buscar: ";
    int cat=0;
    fflush(stdin);
    cin >> cat;

    while(cat < 1 || cat > 5){
        wcout << L"Categoria inválida. Digite novamente: ";
        cin.clear();
        fflush(stdin);
        cin >> cat;
    }

    TRIE trie(static_cast<Categoria>(cat-1));
    trie.load();

    cout << "Digite a substring: ";
    string substring;
    fflush(stdin);
    getline(cin, substring);
    char substringc[200]={0};
    strncpy(substringc, substring.c_str(), 199);

    vector<int> ids = trie.find_ids(substringc);

    if(ids.size() == 0)
        cout << "Nenhum registro encontrado." << endl;

    else{
        vector<Registro> resultado;

        for(int i = 0; i < ids.size(); i++){
            Registro registro = directsearch_data_id("./data/data.bin", ids[i]);
            resultado.push_back(registro);
        }

        menu_print_pagina(resultado, cat, substring);
    }
}

void inicializacao(void){

    ifstream file;
    file.open("./data/data.bin", ios::in | ios::binary);
    if(!file.good()){
        cout << "Nenhum arquivo de dados encontrado. Digite o nome do arquivo gostaria de usar: ";
        deleteTRIEs();
        string file_name;
        cin >> file_name;
        while(convert_csv_bin(file_name) == 1){
            cout << "Erro ao abrir o arquivo." << endl;
            cout << "Digite o nome do arquivo que gostaria de usar: ";
            fflush(stdin);
            cin >> file_name;
        }
        cout << "Arquivo convertido com sucesso." << endl;
    }else{
        wcout << L"Um arquivo de dados foi encontrado. Antes de começar, gostaria de excluir os dados anteriores? (s/n): ";
        char answer;
        fflush(stdin);
        cin >> answer;
        if(answer == 's'){
            file.close();
            if(remove("./data/data.bin"))
                cout << "Erro ao excluir o arquivo." << endl;
            else
                wcout << L"Arquivo excluído com sucesso." << endl << endl;
            deleteTRIEs();
            remove("./data/metadata.bin");
            remove("./data/comp.bin");
            remove("./data/metacomp.bin");
            cout << "Digite o nome do arquivo que gostaria de usar: ";
            string file_name;
            fflush(stdin);
            cin >> file_name;
            while(convert_csv_bin(file_name)){
                cout << "Erro ao abrir o arquivo." << endl;
                cout << "Digite o nome do arquivo que gostaria de usar: ";
                fflush(stdin);
                cin >> file_name;
            }
            system("cls");
            cout << "Arquivo convertido com sucesso." << endl;
        }
        do{
            cout << "Gostaria de adicionar mais dados? (s/n): ";
            fflush(stdin);
            cin >> answer;
            if(answer == 's'){
                cout << "Digite o nome do arquivo que gostaria de usar: ";
                string file_name;
                cin >> file_name;
                if(convert_csv_bin(file_name))
                    cout << "Erro ao abrir o arquivo." << endl;
                else{
                    system("cls");
                    cout << "Arquivo convertido com sucesso." << endl;
                }
            }
        }while(answer != 'n');
    }
    file.close();
}

void menu_principal(void){

    char comando='\0';

    do{
        wcout << "Comandos: " << endl
              << "(a) - Adicionar mais dados" << endl
              << "(b) - Buscar por categoria" << endl
              << L"(c) - Comparar orçamentos" << endl
              << "(d) - Deletar todos os dados" << endl
              << "(e) - Exibir todos os registros" << endl
              << "(q) - sair" << endl;

        cout << "Digite um comando: ";
        fflush(stdin);
        cin >> comando;
        while(comando != 'a' && comando != 'b' && comando != 'c' && comando != 'd' && comando != 'e' && comando != 'q'){
            wcout << L"Comando inválido. Digite novamente: ";
            fflush(stdin);
            cin >> comando;
        }
        if(comando == 'a'){
            cout << "Digite o nome do arquivo que gostaria de usar: ";
            string file_name;
            cin >> file_name;
            while(convert_csv_bin(file_name)){
                cout << "Erro ao abrir o arquivo." << endl;
                cout << "Digite o nome do arquivo que gostaria de usar: ";
                fflush(stdin);
                cin >> file_name;
            }
            system("cls");
            cout << "Arquivo convertido com sucesso." << endl;
        }else if(comando == 'b'){
            char continua_leitura = 's';
            do{
                if(continua_leitura == 's')
                    busca_categoria();
                cout << "Gostaria de fazer outra busca? (s/n): ";
                fflush(stdin);
                cin >> continua_leitura;
                system("cls");
            }while(continua_leitura != 'n');
        }else if(comando == 'c'){
            
            ifstream data;
            data.open("./data/data.bin", ios::in | ios::binary);
            if(data.is_open()){

                Registro r;
                vector<Registro> registros;
                char buffer[sizeof(Registro)];
                while(data.read((char*)buffer, sizeof(Registro))){
                    r = Registro(buffer);
                    registros.push_back(r);
                }

                compara_regs(registros);
                data.close();
            }else
                cout << "Erro ao abrir o arquivo de dados." << endl;
            
            system("cls");
        }else if(comando == 'd'){
            char answer;
            cout << "Tem certeza que deseja excluir todos os dados? (s/n): ";
            fflush(stdin);
            cin >> answer;
            if(answer == 's'){
                if(remove("./data/data.bin"))
                    cout << "Erro ao excluir o arquivo." << endl;
                else
                    wcout << L"Arquivo excluído com sucesso." << endl << endl;
                deleteTRIEs();
                remove("./data/metadata.bin");
            }
            system("cls");
        }else if(comando == 'e'){
            
            ifstream data;
            data.open("./data/data.bin", ios::in | ios::binary);
            if(data.is_open()){

                Registro r;
                vector<Registro> registros;
                char buffer[sizeof(Registro)];
                while(data.read((char*)buffer, sizeof(Registro))){
                    r = Registro(buffer);
                    registros.push_back(r);
                }

                menu_print_pagina(registros, 1, "TodosRegistros");
                data.close();
                system("cls");
            }else{
                cout << "Erro ao abrir o arquivo de dados." << endl;
            }
        }

    }while(comando != 'q');
}
