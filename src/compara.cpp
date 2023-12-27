#include "compara.h"

void compara_regs(vector<Registro> registros){

    ifstream file;
    file.open("./data/comp.bin", ios::binary | ios::in);

    if(!file.is_open()){
        
        std::wcout << L"Calculando diferenças entre os registros... (Pode levar um tempo)" << endl; 

        int id_antigo_i = 0;
        int id_antigo_f = 0;
        int id_novo_i = 0;
        int id_novo_f = 0;
        tuple<int, int> anos = find_ids(registros, id_antigo_i, id_antigo_f, id_novo_i, id_novo_f);
        unordered_map<Categoria, TRIE> tries{
            {Categoria::orgao_superior, TRIE(Categoria::orgao_superior)},
            {Categoria::orgao_subordinado, TRIE(Categoria::orgao_subordinado)},
            {Categoria::acao, TRIE(Categoria::acao)},
            {Categoria::grupo_despesa, TRIE(Categoria::grupo_despesa)},
            {Categoria::elemento_despesa, TRIE(Categoria::elemento_despesa)}
        };

        load_trie_trees(tries);

        vector<DIFERENCA> diferencas = find_match(registros, tries, id_antigo_i, id_antigo_f, id_novo_i, id_novo_f);

        if(diferencas.size() > 0){
            sort(diferencas.begin(), diferencas.end(), [](DIFERENCA a, DIFERENCA b){
                                                        return get<2>(a) > get<2>(b);
                                                    });

            ofstream(ofile);
            ofile.open("./data/comp.bin", ios::binary | ios::out);
            ofile.write((char*)&anos, sizeof(tuple<int, int>));
            for(int i=0; i<diferencas.size(); i++){
                ofile.write((char*)&diferencas[i], sizeof(DIFERENCA));
            }

            menu_print_diferencas(diferencas, registros, anos);
        }
        else
            wcout << L"Nenhum orçamento possui ocorrências em mais de um ano!" << endl;        

    }else{

        vector<DIFERENCA> diferencas;
        tuple<int, int> anos;
        DIFERENCA dif;
        file.read((char*)&anos, sizeof(tuple<int, int>));

        while(file.read((char*)&dif, sizeof(DIFERENCA))){
            diferencas.push_back(dif);
        }

        menu_print_diferencas(diferencas, registros, anos);

    }

}

tuple<int,int> find_ids(vector<Registro> registros, int &id_antigo_i, int& id_antigo_f, int &id_novo_i, int& id_novo_f){

    int id_aux = 1;
    int ano_antigo = registros[0].get_ano_exercicio();
    int ano_novo;

    while(registros[id_aux-1].get_ano_exercicio() == ano_antigo && id_aux <= registros.size()){
        id_aux++;
    }
    
    if(registros[id_aux-1].get_ano_exercicio() < ano_antigo){
        ano_novo = ano_antigo;
        ano_antigo = registros[id_aux-1].get_ano_exercicio();

        id_antigo_i = id_aux;
        id_antigo_f = registros.size();

        id_novo_i = 1;
        id_novo_f = id_aux;

    }else{
        ano_novo = registros[id_aux-1].get_ano_exercicio();

        id_novo_i = id_aux;
        id_novo_f = registros.size();

        id_antigo_i = 1;
        id_antigo_f = id_aux;
    }
    return make_tuple(ano_antigo, ano_novo);
}

vector<DIFERENCA> find_match(vector<Registro> registros, unordered_map<Categoria, TRIE>&tries, int& id_antigo_i, int& id_antigo_f, int& id_novo_i, int& id_novo_f){

    vector<DIFERENCA> diferencas;

    for(int i = id_antigo_i; i < id_antigo_f; i++){

        vector<int> ids_org_sup = tries[Categoria::orgao_superior].find_ids(registros[i-1].get_orgao_superior());
        vector<int> ids_org_sub = tries[Categoria::orgao_subordinado].find_ids(registros[i-1].get_orgao_subordinado());
        vector<int> ids_acao = tries[Categoria::acao].find_ids(registros[i-1].get_acao());
        vector<int> ids_grupo_desp = tries[Categoria::grupo_despesa].find_ids(registros[i-1].get_grupo_despesa());
        vector<int> ids_elem_desp = tries[Categoria::elemento_despesa].find_ids(registros[i-1].get_elemento_despesa());

        sort(ids_org_sup.begin(), ids_org_sup.end());
        sort(ids_org_sub.begin(), ids_org_sub.end());
        sort(ids_acao.begin(), ids_acao.end());
        sort(ids_grupo_desp.begin(), ids_grupo_desp.end());
        sort(ids_elem_desp.begin(), ids_elem_desp.end());

        // faz a intersecção entre os vectors
        vector<int> ids_aux1, ids_aux2, ids_aux3, ids_aux4;
        set_intersection(ids_org_sup.begin(), ids_org_sup.end(), ids_org_sub.begin(), ids_org_sub.end(), back_inserter(ids_aux1));
        set_intersection(ids_acao.begin(), ids_acao.end(), ids_grupo_desp.begin(), ids_grupo_desp.end(), back_inserter(ids_aux2));
        set_intersection(ids_aux1.begin(), ids_aux1.end(), ids_aux2.begin(), ids_aux2.end(), back_inserter(ids_aux3));
        set_intersection(ids_aux3.begin(), ids_aux3.end(), ids_elem_desp.begin(), ids_elem_desp.end(), back_inserter(ids_aux4));

        // encontra o registro com o ano diferente
        for(int j = 0; j<ids_aux4.size(); j++){
            if(registros[ids_aux4[j]-1].get_ano_exercicio() != registros[i-1].get_ano_exercicio()){
                // compara os valores
                DIFERENCA diferenca;
                get<0>(diferenca) = i;
                get<1>(diferenca) = ids_aux4[j];
                get<2>(diferenca) = registros[ids_aux4[j]-1].get_orcamento_realizado() - registros[i-1].get_orcamento_realizado(); 
                diferencas.push_back(diferenca);
                break;
            }
        }
    }
    return diferencas;
}

void print_diferenca(DIFERENCA& diferenca, vector<Registro>& registros){

    char tmp_superiorl[200] = {0};
    char tmp_subordinadol[200] = {0};
    char tmp_acaol[200] = {0};
    char tmp_grupo_despesal[200] = {0};
    char tmp_elemento_despesal[200] = {0};

    strcpy(tmp_superiorl, registros[get<0>(diferenca)-1].get_orgao_superior());
    strcpy(tmp_subordinadol, registros[get<0>(diferenca)-1].get_orgao_subordinado());
    strcpy(tmp_acaol, registros[get<0>(diferenca)-1].get_acao());
    strcpy(tmp_grupo_despesal, registros[get<0>(diferenca)-1].get_grupo_despesa());
    strcpy(tmp_elemento_despesal, registros[get<0>(diferenca)-1].get_elemento_despesa());

    if(strlen(tmp_superiorl) <= 73 && strlen(tmp_subordinadol) <= 73 && strlen(tmp_acaol) <= 74){

        cout << setw(73) << left << tmp_superiorl
             << " | " << setw(73) << tmp_subordinadol
             << " | " << setw(74) << tmp_acaol << endl
             << setw(235) << setfill('-') << "-" << setfill(' ') << endl;

    }else if(strlen(tmp_superiorl) <= 2*73 && strlen(tmp_subordinadol) <= 2*73 && strlen(tmp_acaol) <= 2*74){

        char tmp_superior[74] = {0};
        char tmp_subordinado[74] = {0};
        char tmp_acao[75] = {0};

        memcpy(tmp_superior, tmp_superiorl, 73);
        memcpy(tmp_subordinado, tmp_subordinadol, 73);
        memcpy(tmp_acao, tmp_acaol, 74);

        cout << setw(73) << left << tmp_superior
             << " | " << setw(73) << tmp_subordinado
             << " | " << setw(74) << tmp_acao << endl
 
             << "       | " << setw(73) << left << tmp_superiorl + 73
             << " | " << setw(73) << tmp_subordinadol + 73
             << " | " << setw(74) << tmp_acaol + 74 << endl
             << setw(235) << setfill('-') << "-" << setfill(' ') << endl;       
    }else{
        char tmp_superior[74] = {0};
        char tmp_subordinado[74] = {0};
        char tmp_acao[75] = {0};

        char tmp_superior2[74] = {0};
        char tmp_subordinado2[74] = {0};
        char tmp_acao2[75] = {0};

        memcpy(tmp_superior, tmp_superiorl, 73);
        memcpy(tmp_subordinado, tmp_subordinadol, 73);
        memcpy(tmp_acao, tmp_acao, 74);

        memcpy(tmp_superior2, tmp_superior + 73, 73);
        memcpy(tmp_subordinado2, tmp_subordinado + 73, 73);
        memcpy(tmp_acao2, tmp_acao + 74, 74);

        cout << setw(73) << left << tmp_superior
             << " | " << setw(73) << tmp_subordinado
             << " | " << setw(74) << tmp_acao << endl

             << "       | " << setw(73) << left << tmp_superior2
             << " | " << setw(73) << tmp_subordinado2
             << " | " << setw(74) << tmp_acao2 << endl
 
             << "       | " << setw(73) << left << tmp_superiorl + 146
             << " | " << setw(73) << tmp_subordinadol + 146
             << " | " << setw(74) << tmp_acaol + 146 << endl
             << setw(235) << setfill('-') << "-" << setfill(' ') << endl;
    }

    if(strlen(tmp_grupo_despesal) <= 73 && strlen(tmp_elemento_despesal) <= 96){

        cout << "       | " << setw(73) << left << tmp_grupo_despesal
             << " | " << setw(96) << left << tmp_elemento_despesal
             << " | " << setprecision(2) << right << fixed << setw(15) << registros[get<0>(diferenca)-1].get_orcamento_realizado()
             << " | " << setprecision(2) << right << fixed << setw(15) << registros[get<1>(diferenca)-1].get_orcamento_realizado()
             << " | " << setprecision(2) << right << fixed << setw(15) << get<2>(diferenca) << endl
             << left << setw(235) << setfill('=') << "=" << setfill(' ') << endl;
    }else if(strlen(tmp_grupo_despesal) <= 2*73 && strlen(tmp_elemento_despesal) <= 2*96){

        char tmp_grupo[74] = {'\0'};
        char tmp_elemento[97] = {'\0'};

        memcpy(tmp_grupo, tmp_grupo_despesal, 73);
        memcpy(tmp_elemento, tmp_elemento_despesal, 96);

        cout << "       | " << setw(73) << left << tmp_grupo
             << " | " << setw(96) << left << tmp_elemento
             << " | " << setprecision(2) << right << fixed << setw(15) << registros[get<0>(diferenca)-1].get_orcamento_realizado()
             << " | " << setprecision(2) << right << fixed << setw(15) << registros[get<1>(diferenca)-1].get_orcamento_realizado()
             << " | " << setprecision(2) << right << fixed << setw(15) << get<2>(diferenca) << endl
 
             << "       | " << setw(73) << left << tmp_grupo_despesal + 73
             << " | " << setw(96) << left << tmp_elemento_despesal + 96
             << " | " << setw(15) << ""
             << " | " << setw(15) << ""
             << " | " << setw(15) << "" << endl
             << left << setw(235) << setfill('=') << "=" << setfill(' ') << endl;

    }else{

        char tmp_grupo[74] = {'\0'};
        char tmp_elemento[97] = {'\0'};

        char tmp_grupo2[74] = {'\0'};
        char tmp_elemento2[97] = {'\0'};

        memcpy(tmp_grupo, tmp_grupo_despesal, 73);
        memcpy(tmp_elemento, tmp_elemento_despesal, 96);

        memcpy(tmp_grupo2, tmp_grupo_despesal + 73, 73);
        memcpy(tmp_elemento2, tmp_elemento_despesal + 96, 96);

        cout << "       | " << setw(73) << left << tmp_grupo
             << " | " << setw(96) << left << tmp_elemento
             << " | " << setw(15) << ""
             << " | " << setw(15) << ""
             << " | " << setw(15) << "" << endl
 
             << "       | " << setw(73) << left << tmp_grupo2
             << " | " << setw(96) << left << tmp_elemento2
             << " | " << setprecision(2) << right << fixed << setw(15) << registros[get<0>(diferenca)-1].get_orcamento_realizado()
             << " | " << setprecision(2) << right << fixed << setw(15) << registros[get<1>(diferenca)-1].get_orcamento_realizado()
             << " | " << setprecision(2) << right << fixed << setw(15) << get<2>(diferenca) << endl
 
             << "       | " << setw(73) << left << tmp_grupo_despesal + 170
             << " | " << setw(96) << left << tmp_elemento_despesal + 168
             << " | " << setw(15) << ""
             << " | " << setw(15) << ""
             << " | " << setw(15) << "" << endl
             << left << setw(235) << setfill('=') << "=" << setfill(' ') << endl;
    }
}

void menu_print_diferencas(vector<DIFERENCA>& diferencas, vector<Registro>& registros, tuple<int, int> anos){
    string navegacao = {'\0'};
    int pagina=1;
    bool reverse = false;
    int total_paginas = diferencas.size()/20 + 1;
    while(navegacao!="q"){

        system("cls");

        wcout << L"Mostrando as diferenças entre os orçamentos de " << get<0>(anos) << " e " << get<1>(anos) << endl << endl;

        wcout << setw(5) << right << "No" << left
              << ". | " 
              << setw(73) << left << L"ORGÃO SUPERIOR"
              << " | " << setw(73) << L"ORGÃO SUBORDINADO"
              << " | " << setw(74) << L"AÇÃO" << endl;
        cout  << setw(235) << setfill('-') << "-" << setfill(' ') << endl;
        wcout << "       | " << setw(73) << left << "GRUPO DE DESPESA"
              << " | " << setw(96) << left << "ELEMENTO DE DESPESA"
              << " | " << setw(11) << right << L"ORÇAM. " << get<0>(anos)
              << " | " << setw(11) << right << L"ORÇAM. " << get<1>(anos)
              << " | " << setw(15) << right << L"DIFERENÇA";
        cout  << left << setw(235) << setfill('=') << "=" << setfill(' ') << endl;
        cout  << left << setw(235) << setfill('=') << "=" << setfill(' ') << endl;

        if(reverse){
            for(int i = (total_paginas-pagina+1)*20-1; i >= (total_paginas-pagina)*20; i--){
                if(i >= 0 && i < diferencas.size()){
                    cout << right << setw(5) << i+1 << left << ". | ";
                    print_diferenca(diferencas[i], registros);
                }
            }
        }else{
            for(int i = (pagina-1)*20; i < pagina*20; i++){
                if(i < diferencas.size()){
                    cout << right << setw(5) << i+1 << left << ". | ";
                    print_diferenca(diferencas[i], registros);
                }
            }
        }

        wcout << endl << L"Página " << pagina << " de " << total_paginas << endl;
        
        wcout << L"Digite 'n' para a próxima página, 'p' para a página anterior, 'r' para reverter a ordem, um número de página, ou 'q' para sair da consulta: ";
        fflush(stdin);
        cin >> navegacao;
        while(navegacao!="n" && navegacao!="p" && navegacao!="q" && navegacao!="r" && stoi(navegacao) < 1 && stoi(navegacao) > total_paginas){
            wcout << L"Opção inválida. Digite novamente: ";
            fflush(stdin);
            cin >> navegacao;
        }
        if(navegacao == "n"){
            if(pagina < total_paginas){
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
    system("cls");
}