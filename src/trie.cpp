#include "trie.h"

char* pt_to_ascii(char* str){

    int str_size = strlen(str);
    char ascii[200];

    for(int i=0; i < str_size; i++){
        if((int)str[i] ==  0xFFFFFFE1 || (int)str[i] ==  0xFFFFFFE0 || (int)str[i] ==  0xFFFFFFE2 || (int)str[i] ==  0xFFFFFFE3){
            ascii[i] = 'a';
        }
        else if((int)str[i] ==  0xFFFFFFE9 || (int)str[i] ==  0xFFFFFFEA){
            ascii[i] = 'e';
        }
        else if((int)str[i] ==  0xFFFFFFED){
            ascii[i] = 'i';
        }
        else if((int)str[i] ==  0xFFFFFFF3 || (int)str[i] ==  0xFFFFFFF4 || (int)str[i] ==  0xFFFFFFF5){
            ascii[i] = 'o';
        }
        else if((int)str[i] ==  0xFFFFFFFA || (int)str[i] ==  0xFFFFFFFC){
            ascii[i] = 'u';
        }
        else if((int)str[i] ==  0xFFFFFFE7){
            ascii[i] = 'c';
        }
        else if((int)str[i] ==  0xFFFFFFC1 || (int)str[i] ==  0xFFFFFFC0 || (int)str[i] ==  0xFFFFFFC2 || (int)str[i] ==  0xFFFFFFC3){
            ascii[i] = 'A';
        }
        else if((int)str[i] ==  0xFFFFFFC9 || (int)str[i] ==  0xFFFFFFCA){
            ascii[i] = 'E';
        }
        else if((int)str[i] ==  0xFFFFFFCD){
            ascii[i] = 'I';
        }
        else if((int)str[i] ==  0xFFFFFFD3 || (int)str[i] ==  0xFFFFFFD5 || (int)str[i] ==  0xFFFFFFD4){
            ascii[i] = 'O';
        }
        else if((int)str[i] ==  0xFFFFFFDA || (int)str[i] ==  0xFFFFFFDC){
            ascii[i] = 'U';
        }
        else if((int)str[i] ==  0xFFFFFFC7){
            ascii[i] = 'C';
        }
        else if((int)str[i] == 0xFFFFFFB1) // caractere ±
            ascii[i] = '-';
        else if((str[i] >= 'a' && str[i] <= 'z') || (str[i] >='A' && str[i] <= 'Z') || str[i] == ' ' || str[i] == '(' || str[i] == ')' || str[i] == '-' || str[i] == ',' || str[i] == ':' || str[i] == '/' || str[i] == '.' || (str[i] >= '0' && str[i] <= '9')){
            ascii[i] = str[i];
        }
        else{
            ascii[i] = '-';
        }

        ascii[i] = toupper(ascii[i]);
    }
    ascii[str_size] = '\0';
    strcpy(str, ascii);
    return str;
}

//* Métodos da classe TRIENode

void TRIENode::create_filho(TRIE& t, char c){

    TRIENode filho(c, t.n_nodos);
    
    this->filhos[n_filhos] = t.n_nodos;
    this->filhos_c[n_filhos] = c;

    //ordena os filhos com base no caractere
    for(int i=0; i<this->n_filhos; i++){
        for(int j=i+1; j<this->n_filhos; j++){
            if(this->filhos_c[i] > this->filhos_c[j]){
                int aux = this->filhos[i];
                char aux_c = this->filhos_c[i];
                this->filhos[i] = this->filhos[j];
                this->filhos_c[i] = this->filhos_c[j];
                this->filhos[j] = aux;
                this->filhos_c[j] = aux_c;
            }
        }
    }

    this->n_filhos++;
    t.n_nodos++;
    t.nodos.push_back(filho);
}

void TRIENode::save(ofstream& out){

    out.write((char*)&this->id, sizeof(int));
    out.write((char*)&this->letra, sizeof(char));
    out.write((char*)&this->folha, sizeof(bool));
    out.write((char*)&this->arquivo, sizeof(int));
    out.write((char*)&this->n_filhos, sizeof(int));
    for(int i=0; i<this->n_filhos; i++){
        out.write((char*)&this->filhos[i], sizeof(int));
        out.write((char*)&this->filhos_c[i], sizeof(char));
    }
}

void TRIENode::load(ifstream& in){

    in.read((char*)&this->id, sizeof(int));
    in.read((char*)&this->letra, sizeof(char));
    in.read((char*)&this->folha, sizeof(bool));
    in.read((char*)&this->arquivo, sizeof(int));
    in.read((char*)&this->n_filhos, sizeof(int));
    for(int i=0; i<this->n_filhos; i++){
        in.read((char*)&this->filhos[i], sizeof(int));
        in.read((char*)&this->filhos_c[i], sizeof(char));
    }
}

void TRIENode::print(void){
    cout << "nodo: " << this->id << endl;
    cout << "letra: " << this->letra << endl;
    cout << "eh folha:" << this->folha << endl;
    cout << "arquivo: " << this->arquivo << endl;
    cout << "n_filhos: " << this->n_filhos << endl;
    for(int i=0; i<ALPHABET_SIZE; i++){
        cout << "filho " << i << ": " << this->filhos[i] << endl;
    }
    for(int i=0; i<ALPHABET_SIZE; i++){
        cout << "filho_c " << i << ": " << this->filhos_c[i] << endl;
    }
}

//* Métodos da classe TRIE

void TRIE::step(char c){
    int i=0;
    while(this->nodos[this->atual].filhos_c[i] != c && this->nodos[this->atual].filhos[i] !=-1 && i<ALPHABET_SIZE) // procura o filho com o caractere c
        i++;

    if(this->nodos[this->atual].filhos[i] ==-1){ // se não existe filho com o caractere c
        this->nodos[this->atual].create_filho(*this, c); // cria um filho com o caractere c
    }

    if(i==ALPHABET_SIZE){
        this->atual = this->raiz;
        wcout << L"ERRO: Não foi possível avançar na TRIE!" << endl;
        exit(2);
    }
    this->atual = this->nodos[this->atual].filhos[i]; // atualiza o indice do nodo atual
}

void TRIE::add_Registro(Registro r){
    stringstream file_name;
    file_name.clear();
    file_name << "./data/";

    char str[200];
    switch(this->categoria){ //orgao_superior, orgao_subordinado, acao, grupo_despesa, elemento_despesa
        case Categoria::orgao_superior:
            strcpy(str, r.get_orgao_superior());
            file_name << "orgao_superior/";
            break;
        case Categoria::orgao_subordinado:
            strcpy(str, r.get_orgao_subordinado());
            file_name << "orgao_subordinado/";
            break;
        case Categoria::acao:
            strcpy(str, r.get_acao());
            file_name << "acao/";
            break;
        case Categoria::grupo_despesa:
            strcpy(str, r.get_grupo_despesa());
            file_name << "grupo_despesa/";
            break;
        case Categoria::elemento_despesa:
            strcpy(str, r.get_elemento_despesa());
            file_name << "elemento_despesa/";
            break;
    }

    pt_to_ascii(str);
    this->atual=this->raiz; // sempre começa na raiz
    for(int i=0; i < strlen(str); i++){
        this->step(str[i]);
    }
    //TRIENode *folha = &this->nodos[atual];
    this->nodos[atual].set_folha(true);

    ofstream arquivo;

    if(this->nodos[atual].get_arquivo()!=-1){
        //arquivo já existe
        file_name << setw(5) << setfill('0') << this->nodos[atual].get_arquivo() << ".bin";
        arquivo.open(file_name.str(), ios::app | ios::binary);
        arquivo.write((char*)r.pget_id(), sizeof(int));
    }else{
        //cria arquivo
        this->nodos[atual].set_arquivo(++(this->n_arquivos));
        file_name << setw(5) << setfill('0') << this->nodos[atual].get_arquivo() << ".bin";
        arquivo.open(file_name.str(), ios::out | ios::binary);
        arquivo.write((char*)r.pget_id(), sizeof(int));
    }
    arquivo.close();
}

void TRIE::save(void){

    ofstream arquivo;
    stringstream file_name;
    file_name.clear();
    file_name << "./data/";
    switch(this->categoria){ //orgao_superior, orgao_subordinado, acao, grupo_despesa, elemento_despesa
        case Categoria::orgao_superior:
            file_name << "orgao_superior/";
            break;
        case Categoria::orgao_subordinado:
            file_name << "orgao_subordinado/";
            break;
        case Categoria::acao:
            file_name << "acao/";
            break;
        case Categoria::grupo_despesa:
            file_name << "grupo_despesa/";
            break;
        case Categoria::elemento_despesa:
            file_name << "elemento_despesa/";
            break;
    }
    file_name << "trie.bin";

    arquivo.open(file_name.str(), ios::out | ios::binary);

    arquivo.write((char*)&this->raiz, sizeof(int));
    arquivo.write((char*)&this->n_arquivos, sizeof(int));
    arquivo.write((char*)&this->n_nodos, sizeof(int));
    for(int i=0; i<n_nodos; i++)
        this->nodos[i].save(arquivo);

}

void TRIE::load(void){

    ifstream arquivo;
    stringstream file_name;
    file_name.clear();
    file_name << "./data/";
    switch(this->categoria){ //orgao_superior, orgao_subordinado, acao, grupo_despesa, elemento_despesa
        case Categoria::orgao_superior:
            file_name << "orgao_superior/";
            break;
        case Categoria::orgao_subordinado:
            file_name << "orgao_subordinado/";
            break;
        case Categoria::acao:
            file_name << "acao/";
            break;
        case Categoria::grupo_despesa:
            file_name << "grupo_despesa/";
            break;
        case Categoria::elemento_despesa:
            file_name << "elemento_despesa/";
            break;
    }
    file_name << "trie.bin";

    arquivo.open(file_name.str(), ios::in | ios::binary);

    arquivo.read((char*)&this->raiz, sizeof(int));
    arquivo.read((char*)&this->n_arquivos, sizeof(int));
    arquivo.read((char*)&this->n_nodos, sizeof(int));
    for(int i=0; i<n_nodos; i++){
        //adiciona os nodos de volta na árvore
        TRIENode n;
        n.load(arquivo);
        if(i==0)
            this->nodos[0] = n;
        else
            this->nodos.push_back(n);
    }
    this->atual = this->raiz;
}

vector<int> TRIE::read_ids_from_file(int arquivo){

    vector<int> ids;
    ifstream arquivo_bin;
    stringstream file_name;
    file_name.clear();
    file_name << "./data/";
    switch(this->categoria){ //orgao_superior, orgao_subordinado, acao, grupo_despesa, elemento_despesa
        case Categoria::orgao_superior:
            file_name << "orgao_superior/";
            break;
        case Categoria::orgao_subordinado:
            file_name << "orgao_subordinado/";
            break;
        case Categoria::acao:
            file_name << "acao/";
            break;
        case Categoria::grupo_despesa:
            file_name << "grupo_despesa/";
            break;
        case Categoria::elemento_despesa:
            file_name << "elemento_despesa/";
            break;
    }
    file_name << setw(5) << setfill('0') << arquivo << ".bin";

    arquivo_bin.open(file_name.str(), ios::in | ios::binary);

    int id;
    while(arquivo_bin.read((char*)&id, sizeof(int))){
        ids.push_back(id);
    }

    return ids;
}

vector<int> TRIE::read_ids_all_children(int nodo){

    vector<int> ids;
    if(this->nodos[nodo].is_folha()){
        vector<int> ids_arquivo = this->read_ids_from_file(this->nodos[nodo].get_arquivo());
        ids.insert(ids.end(), ids_arquivo.begin(), ids_arquivo.end());
        for(int i=0; i< this->nodos[nodo].n_filhos; i++){
            if(this->nodos[nodo].filhos[i] != -1){
                vector<int> ids_filho = this->read_ids_all_children(this->nodos[nodo].filhos[i]);
                ids.insert(ids.end(), ids_filho.begin(), ids_filho.end());
            }
        }
    }else{
        for(int i=0; i< this->nodos[nodo].n_filhos; i++){
            if(this->nodos[nodo].filhos[i] != -1){
                vector<int> ids_filho = this->read_ids_all_children(this->nodos[nodo].filhos[i]);
                ids.insert(ids.end(), ids_filho.begin(), ids_filho.end());
            }
        }
    }
    return ids;
}

vector<int> TRIE::find_ids(const char* str){

    char str_new[200];
    strcpy(str_new, str);
    pt_to_ascii(str_new);

    this->atual = this->raiz;
    for(int j=0; j<strlen(str); j++){ // percorre a string
        int i=0;
        while(this->nodos[this->atual].filhos_c[i] != str_new[j] && this->nodos[this->atual].filhos[i] !=-1 && i<ALPHABET_SIZE) // procura o filho com o caractere str_new[j]
            i++;
        if(i==ALPHABET_SIZE) // procurou um nodo inválido (subtrsing não existe na árvore)
            return vector<int>();
        
        this->atual = this->nodos[this->atual].filhos[i]; // atualiza o indice do nodo atual
    }
    return read_ids_all_children(this->atual);
}

void TRIE::print(void){
    cout << "raiz: " << this->raiz << endl;
    cout << "n_arquivos: " << this->n_arquivos << endl;
    cout << "n_nodos: " << this->n_nodos << endl;
    for(int i=0; i<20; i++){
        this->nodos[i].print();
    }
}