#include "datafile_op.h"

void deleteDirectoryContents(const string& dir_path){
    filesystem::remove_all(dir_path);
    filesystem::create_directory(dir_path);
}

void deleteTRIEs(void){
    deleteDirectoryContents("./data/orgao_superior/");
    deleteDirectoryContents("./data/orgao_subordinado/");
    deleteDirectoryContents("./data/acao/");
    deleteDirectoryContents("./data/grupo_despesa/");
    deleteDirectoryContents("./data/elemento_despesa/");
}

string meta_filename(string filename){
    
    string meta_filename = filename.substr(0, filename.find_last_of("/\\")) + "/meta" + filename.substr(filename.find_last_of("/\\")+1, filename.length());
    return meta_filename;
}

Registro binarysearch_data_id(string file_name, int id){

    ifstream file, metadata;
    file.open(file_name, ios::in | ios::binary);
    metadata.open(meta_filename(file_name), ios::in | ios::binary);

    Registro registro;
    char buffer[sizeof(Registro)];
    int n_registros;

    if (file.is_open())
    {
        metadata.read((char*)&n_registros, sizeof(int));
        file.read(buffer, sizeof(Registro));
        registro = Registro(buffer);
        int offset = 0;

        int e=0, f=n_registros;
        offset = (e+f)/2;

        while(registro.get_id()!=id && e!=f){
            file.seekg(offset*sizeof(Registro), ios::beg);
            file.read(buffer, sizeof(Registro));
            registro = Registro(buffer);
            if(registro.get_id()>id){
                f = offset;
                offset = (e+f)/2;
            }
            else if(registro.get_id()<id){
                e = offset;
                offset = (e+f)/2;
            }
        }
        if(registro.get_id()!=id)
            registro = Registro();
    }

    file.close();

    return registro;
}

Registro directsearch_data_id(string file_name, int id){

    ifstream file, metadata;
    file.open(file_name, ios::in | ios::binary);
    metadata.open(meta_filename(file_name), ios::in | ios::binary);

    Registro registro;
    char buffer[sizeof(Registro)];
    int n_registros;

    if (file.is_open() && metadata.is_open())
    {
        metadata.read((char*)&n_registros, sizeof(int));
        if(id>n_registros)
            return Registro();

        file.seekg((id-1)*sizeof(Registro), ios::beg);
        file.read(buffer, sizeof(Registro));
        registro = Registro(buffer);

    }

    file.close();

    return registro;
}

void add_Registro_to_vector(vector<Registro>& registros, ifstream& file, int& n_registros, unordered_map<Categoria, TRIE>& tries){

    string line;
    Registro registro;
    char opcao='b';

    getline(file, line);
    int id=n_registros+1; // atribui IDs nova
    cout << "Adicionando registros..." << endl;
    int duplicate = -1;
    while (getline(file, line)){
        registro = Registro(line, id);
        if(opcao!='a' && id>1)
            duplicate = find_duplicate(registros, registro/*, tries*/);
        if(duplicate==-1){
            registros.push_back(registro);

            tries[Categoria::orgao_superior].add_Registro(registro);
            tries[Categoria::orgao_subordinado].add_Registro(registro);
            tries[Categoria::acao].add_Registro(registro);
            tries[Categoria::grupo_despesa].add_Registro(registro);
            tries[Categoria::elemento_despesa].add_Registro(registro);
            id++;
        }else{
            while(opcao!='a' && opcao!='s' && opcao!='+'){
                wcout << L"Alguns registros só diferem por valor do orçamento. Deseja adicionar o registro duplicado (a), substituir (s), ou somar os valores (+)? ";
                cin.clear();
                cin >> opcao;
                switch(opcao){
                    case 'a':
                        cout << "Adicionando registros..." << endl;
                        break;
                    case 's':
                        cout << "Substituindo registros... (pode levar algum tempo)" << endl;
                        break;
                    case '+':
                        cout << "Somando valores... (pode levar algum tempo)" << endl;
                        break;
                    default:
                        cout << "Opção inválida. Tente novamente." << endl;
                }
            }
            if(opcao=='a'){
                registros.push_back(registro);

                tries[Categoria::orgao_superior].add_Registro(registro);
                tries[Categoria::orgao_subordinado].add_Registro(registro);
                tries[Categoria::acao].add_Registro(registro);
                tries[Categoria::grupo_despesa].add_Registro(registro);
                tries[Categoria::elemento_despesa].add_Registro(registro);
                id++;
                duplicate = -1;
            }else if(opcao=='s'){
                registros[duplicate] = registro;
            }else if(opcao=='+'){
                registros[duplicate] = registros[duplicate] + registro;
            }
        }
    }
    n_registros = id-1;
}

int convert_csv_bin(string file_name) // adicina dados do arquivo .csv no arquivo binário
{
    ifstream file;
    file.open(file_name);

    int n_registros=0;
    vector<Registro> registros;
    unordered_map<Categoria, TRIE> tries{
        {Categoria::orgao_superior, TRIE(Categoria::orgao_superior)},
        {Categoria::orgao_subordinado, TRIE(Categoria::orgao_subordinado)},
        {Categoria::acao, TRIE(Categoria::acao)},
        {Categoria::grupo_despesa, TRIE(Categoria::grupo_despesa)},
        {Categoria::elemento_despesa, TRIE(Categoria::elemento_despesa)}
    };

    load_trie_trees(tries);

    ifstream readfile("./data/data.bin", ios::in | ios::binary); // testa se já existe uma base de dados
    if(readfile.is_open()){
        ifstream metadata("./data/metadata.bin", ios::in | ios::binary);
        if(metadata.is_open()) // se existir, pega o número de registros (4 primeiros bytes do arquivo)
            metadata.read((char*)&n_registros, sizeof(int)); // se existir, pega o número de registros (4 primeiros bytes do arquivo)
        metadata.close();

        Registro tmp;
        char buffer[sizeof(Registro)];

        while(readfile.read(buffer, sizeof(Registro))){ // lê todos os registros do arquivo e copia para a memória
            tmp = Registro(buffer);
            registros.push_back(tmp);
        }
    }

    ofstream file_out, metadata_out;
    file_out.open("./data/data.bin", ios::trunc | ios::binary);
    metadata_out.open("./data/metadata.bin", ios::trunc | ios::binary);


    if (file.is_open())
    {
        if (file_out.is_open())
        {
            add_Registro_to_vector(registros, file, n_registros, tries);
            
            for(int i=0; i<registros.size(); i++){
                registros[i].write_to_file(file_out);
                
            }
            tries[Categoria::orgao_superior].save();
            tries[Categoria::orgao_subordinado].save();
            tries[Categoria::acao].save();
            tries[Categoria::grupo_despesa].save();
            tries[Categoria::elemento_despesa].save();
            
            metadata_out.write((char*)&n_registros, sizeof(int));
        }
        else
            return 1;
    }
    else
        return 1;

    //system("cls");
    metadata_out.close();
    file.close();
    file_out.close();

    return 0;
}

void load_trie_trees(unordered_map<Categoria, TRIE>& tries){
    
    ifstream check;
    
    check.open("./data/orgao_superior/trie.bin", ios::in | ios::binary);
    if(check.is_open())
        tries[Categoria::orgao_superior].load();
    check.close();
    check.open("./data/orgao_subordinado/trie.bin", ios::in | ios::binary);
    if(check.is_open())
        tries[Categoria::orgao_subordinado].load();
    check.close();
    check.open("./data/acao/trie.bin", ios::in | ios::binary);
    if(check.is_open())
        tries[Categoria::acao].load();
    check.close();
    check.open("./data/grupo_despesa/trie.bin", ios::in | ios::binary);
    if(check.is_open())
        tries[Categoria::grupo_despesa].load();
    check.close();
    check.open("./data/elemento_despesa/trie.bin", ios::in | ios::binary);
    if(check.is_open())
        tries[Categoria::elemento_despesa].load();
    check.close();
}

int find_duplicate(vector<Registro>& registros, Registro& registro){ // TODO: buscar pelas árvores TRIE
    for(int i=0; i<registros.size(); i++){
        if(registros[i] == registro)
            return i;
    }
    return -1;
}

// * Tentativa de implementar busca de duplicatas por árvore TRIE, não funcionou, não sei por quê
/*int find_duplicate(vector<Registro>& registros, Registro& registro, unordered_map<Categoria, TRIE>& tries){

    vector<int> id_org_sup = tries[Categoria::orgao_superior].find_ids(registro.get_orgao_superior());
    vector<int> id_org_sub = tries[Categoria::orgao_subordinado].find_ids(registro.get_orgao_subordinado());
    vector<int> id_acao = tries[Categoria::acao].find_ids(registro.get_acao());
    vector<int> id_grupo = tries[Categoria::grupo_despesa].find_ids(registro.get_grupo_despesa());
    vector<int> id_elemento = tries[Categoria::elemento_despesa].find_ids(registro.get_elemento_despesa());

    // faz a intersecção entre os vectors
    sort(id_org_sup.begin(), id_org_sup.end());
    sort(id_org_sub.begin(), id_org_sub.end());
    sort(id_acao.begin(), id_acao.end());
    sort(id_grupo.begin(), id_grupo.end());
    sort(id_elemento.begin(), id_elemento.end());
    
    vector<int> ids_aux1, ids_aux2, ids_aux3, ids_aux4;
    set_intersection(id_org_sup.begin(), id_org_sup.end(), id_org_sub.begin(), id_org_sub.end(), back_inserter(ids_aux1));
    set_intersection(id_acao.begin(), id_acao.end(), id_grupo.begin(), id_grupo.end(), back_inserter(ids_aux2));
    set_intersection(ids_aux1.begin(), ids_aux1.end(), ids_aux2.begin(), ids_aux2.end(), back_inserter(ids_aux3));
    set_intersection(ids_aux3.begin(), ids_aux3.end(), id_elemento.begin(), id_elemento.end(), back_inserter(ids_aux4));

    if(ids_aux4.size() > 0){
        for(int i=0; i<ids_aux4.size(); i++){
            if(registros[ids_aux4[i]-1] == registro)
                return ids_aux4[i]-1;
        }
    }

    return -1;
}*/