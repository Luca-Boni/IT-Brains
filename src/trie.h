#ifndef TRIE_H
#define TRIE_H

#include "registro.h"
#include <string.h>
#include <sstream>

#define ALPHABET_SIZE 44

enum class Categoria {orgao_superior, orgao_subordinado, acao, grupo_despesa, elemento_despesa};

char* pt_to_ascii(char* str);

class TRIE;

class TRIENode{
  private:
    int id;
    char letra;
    bool folha;
    int arquivo;
    int n_filhos;
    int filhos[ALPHABET_SIZE];
    char filhos_c[ALPHABET_SIZE];
  public:
    TRIENode(char letra='\0', int id=0){
        this->folha = false;
        this->letra=letra;
        this->n_filhos = 0;
        this->arquivo = -1;
        this->id = id;
        for(int i = 0; i < ALPHABET_SIZE; i++){
            this->filhos[i] = -1;
            this->filhos_c[i] = '\0';
        }
    }

    int get_id(){
        return this->id;
    }
    bool is_folha(){
        return this->folha;
    }
    void set_folha(bool folha){
        this->folha = folha;
    }
    int get_arquivo(){
        return this->arquivo;
    }
    void set_arquivo(int arquivo){
        this->arquivo = arquivo;
    }
    int get_n_filhos(){
        return this->n_filhos;
    }
    int get_filho(char c){
        for(int i = 0; i < n_filhos; i++){
            if(this->filhos[i] != -1 && this->filhos_c[i] == c)
                return this->filhos[i];
        }
        return -1;
    }
        
    void create_filho(TRIE& t, char c);
    void save(ofstream& out);
    void load(ifstream& in);
    void print(void);

    friend class TRIE;
};

class TRIE{
  private:
    Categoria categoria;
    int raiz;
    int atual;
    int n_arquivos;
    int n_nodos;
    vector<TRIENode> nodos;
  public:
    TRIE(Categoria categoria){
        this->categoria = categoria;
        this->raiz = 0;
        this->atual = 0;
        this->n_arquivos = 0;
        this->n_nodos = 1;
        TRIENode node('\0', 0);
        this->nodos.push_back(node);
    }
    TRIE(void){ // ! Construtor sem argumentos NÃO DEVE SER UTILIZADO
        this->categoria = Categoria::orgao_superior;
        this->raiz = 0;
        this->atual = 0;
        this->n_arquivos = 0;
        this->n_nodos = 1;
        TRIENode node('\0', 0);
        this->nodos.push_back(node);
    }

    int get_n_nodos(){
        return this->n_nodos;
    }
    void set_n_nodos(int n_nodos){
        this->n_nodos = n_nodos;
    }

    void step(char c);
    void add_Registro(Registro r);
    void save(void);
    void load(void);

    vector<int> find_ids(const char* prefix);
    vector<int> read_ids_all_children(int nodo);
    vector<int> read_ids_from_file(int arquivo);
    void print(void);

    friend class TRIENode;
};


#endif