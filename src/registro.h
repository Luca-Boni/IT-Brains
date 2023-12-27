#ifndef REGISTRO
#define REGISTRO

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <vector>
#include <iomanip>

using namespace std;

vector<string> splitString(string str, char delimiter);
void print_cabecalho();

class Registro
{
  private:
    int id;
    int ano_exercicio;
    char orgao_superior[200];
    char orgao_subordinado[200];
    char acao[200];
    char grupo_despesa[200];
    char elemento_despesa[200];
    double orcamento_inicial;
    double orcamento_atualizado;
    double orcamento_empenhado;
    double orcamento_realizado;

  public:
    Registro(){
        this->id = 0;
        for(int i=0; i<200; i++){
            this->orgao_superior[i] = '\0';
            this->orgao_subordinado[i] = '\0';
            this->acao[i] = '\0';
            this->grupo_despesa[i] = '\0';
            this->elemento_despesa[i] = '\0';
        }
    }
    Registro(string linha, int id){
        //"EXERCÍCIO";"CÓDIGO ÓRGÃO SUPERIOR";"NOME ÓRGÃO SUPERIOR";"CÓDIGO ÓRGÃO SUBORDINADO";"NOME ÓRGÃO SUBORDINADO";"CÓDIGO UNIDADE ORÇAMENTÁRIA";"NOME UNIDADE ORÇAMENT�RIA";"CÓDIGO FUNÇÃO";"NOME FUNÇÃO";"CÓDIGO SUBFUNÇÃO";"NOME SUBFUNÇÃO";"CÓDIGO PROGRAMA ORÇAMENTÁRIO";"NOME PROGRAMA ORÇAMENTÁRIO";"CÓDIGO AÇÃO";"NOME AÇÃO";"CÓDIGO CATEGORIA ECONÔMICA";"NOME CATEGORIA ECONÔMICA";"CÓDIGO GRUPO DE DESPESA";"NOME GRUPO DE DESPESA";"CÓDIGO ELEMENTO DE DESPESA";"NOME ELEMENTO DE DESPESA";"ORÇAMENTO INICIAL (R$)";"ORÇAMENTO ATUALIZADO (R$)";"ORÇAMENTO EMPENHADO (R$)";"ORÇAMENTO REALIZADO (R$)"
        this->id = id;
        while(linha.find("\"") != string::npos)
        {
            linha.erase(linha.find("\""), 1);
        }
        vector<string> campos = splitString(linha, ';');

        this->ano_exercicio = stoi(campos[0]);
        if(campos[2].size() >= 200){
            memcpy(this->orgao_superior, campos[2].c_str(), 199);
            this->orgao_superior[199] = '\0';
        }else
            strcpy(this->orgao_superior, campos[2].c_str());

        if(campos[4].size() >= 200){
            memcpy(this->orgao_subordinado, campos[4].c_str(), 199);
            this->orgao_subordinado[199] = '\0';
        }else
            strcpy(this->orgao_subordinado, campos[4].c_str());
        if(campos[14].size() >= 200){
            memcpy(this->acao, campos[14].c_str(), 199);
            this->acao[199] = '\0';
        }else
            strcpy(this->acao, campos[14].c_str());
        if(campos[18].size() >= 200){
            memcpy(this->grupo_despesa, campos[18].c_str(), 199);
            this->grupo_despesa[199] = '\0';
        }else
            strcpy(this->grupo_despesa, campos[18].c_str());
        if(campos[20].size() >= 200){
            memcpy(this->elemento_despesa, campos[20].c_str(), 199);
            this->elemento_despesa[199] = '\0';
        }else
            strcpy(this->elemento_despesa, campos[20].c_str());

        // "Limpa" o resto das strings com '\0'

        for(int i = strlen(this->orgao_superior); i < 200; i++)
            this->orgao_superior[i] = '\0';
        for(int i = strlen(this->orgao_subordinado); i < 200; i++)
            this->orgao_subordinado[i] = '\0';
        for(int i = strlen(this->acao); i < 200; i++)
            this->acao[i] = '\0';
        for(int i = strlen(this->grupo_despesa); i < 200; i++)
            this->grupo_despesa[i] = '\0';
        for(int i = strlen(this->elemento_despesa); i < 200; i++)
            this->elemento_despesa[i] = '\0';
        
        // Troca vírgula por ponto
        campos[21].replace(campos[21].find(","), 1, ".");
        this->orcamento_inicial = stod(campos[21]);
        campos[22].replace(campos[22].find(","), 1, ".");
        this->orcamento_atualizado = stod(campos[22]);
        campos[23].replace(campos[23].find(","), 1, ".");
        this->orcamento_empenhado = stod(campos[23]);
        campos[24].replace(campos[24].find(","), 1, ".");
        this->orcamento_realizado = stod(campos[24]);
    }
    Registro(char binario[]){
        memcpy(&(this->id), binario, sizeof(int));
        memcpy(&(this->ano_exercicio), binario + sizeof(int), sizeof(int));
        memcpy(&this->orgao_superior, binario + 2*sizeof(int), 200*sizeof(char));
        memcpy(&this->orgao_subordinado, binario + 2*sizeof(int) + 200*sizeof(char), 200*sizeof(char));
        memcpy(&this->acao, binario + 2*sizeof(int) + 400*sizeof(char), 200*sizeof(char));
        memcpy(&this->grupo_despesa, binario + 2*sizeof(int) + 600*sizeof(char), 200*sizeof(char));
        memcpy(&this->elemento_despesa, binario + 2*sizeof(int) + 800*sizeof(char), 200*sizeof(char));
        memcpy(&(this->orcamento_inicial), binario + 2*sizeof(int) + 1000*sizeof(char), sizeof(double));
        memcpy(&(this->orcamento_atualizado), binario + 2*sizeof(int) + 1000*sizeof(char) + sizeof(double), sizeof(double));
        memcpy(&(this->orcamento_empenhado), binario + 2*sizeof(int) + 1000*sizeof(char) + 2*sizeof(double), sizeof(double));
        memcpy(&(this->orcamento_realizado), binario + 2*sizeof(int) + 1000*sizeof(char) + 3*sizeof(double), sizeof(double));
    }

    int get_id();
    int* pget_id();
    int get_ano_exercicio();
    int* pget_ano_exercicio();
    char* get_orgao_superior();
    char* get_orgao_subordinado();
    char* get_acao();
    char* get_grupo_despesa();
    char* get_elemento_despesa();
    double get_orcamento_inicial();
    double* pget_orcamento_inicial();
    double get_orcamento_atualizado();
    double *pget_orcamento_atualizado();
    double get_orcamento_empenhado();
    double* pget_orcamento_empenhado();
    double get_orcamento_realizado();
    double* pget_orcamento_realizado();
    void print();
    void write_to_file(ofstream &arquivo);

    Registro operator=(Registro r){
        this->id = r.get_id();
        this->ano_exercicio = r.get_ano_exercicio();
        memcpy(this->orgao_superior, r.get_orgao_superior(), 199);
        memcpy(this->orgao_subordinado, r.get_orgao_subordinado(), 199);
        memcpy(this->acao, r.get_acao(),199);
        memcpy(this->grupo_despesa, r.get_grupo_despesa(),199);
        memcpy(this->elemento_despesa, r.get_elemento_despesa(),199);
        this->orcamento_inicial = r.get_orcamento_inicial();
        this->orcamento_atualizado = r.get_orcamento_atualizado();
        this->orcamento_empenhado = r.get_orcamento_empenhado();
        this->orcamento_realizado = r.get_orcamento_realizado();
        return *this;
    }

    Registro operator+(Registro r){
        Registro novo;
        novo.id = this->id;
        novo.ano_exercicio = this->ano_exercicio;
        memcpy(novo.orgao_superior, this->orgao_superior,199);
        memcpy(novo.orgao_subordinado, this->orgao_subordinado,199);
        memcpy(novo.acao, this->acao,199);
        memcpy(novo.grupo_despesa, this->grupo_despesa,199);
        memcpy(novo.elemento_despesa, this->elemento_despesa,199);
        novo.orcamento_inicial = this->orcamento_inicial + r.get_orcamento_inicial();
        novo.orcamento_atualizado = this->orcamento_atualizado + r.get_orcamento_atualizado();
        novo.orcamento_empenhado = this->orcamento_empenhado + r.get_orcamento_empenhado();
        novo.orcamento_realizado = this->orcamento_realizado + r.get_orcamento_realizado();
        return novo;
    }

    bool operator==(Registro r){
        if(this->ano_exercicio == r.get_ano_exercicio() && strcmp(this->orgao_superior, r.get_orgao_superior()) == 0 && strcmp(this->orgao_subordinado, r.get_orgao_subordinado()) == 0 && strcmp(this->acao, r.get_acao()) == 0 && strcmp(this->grupo_despesa, r.get_grupo_despesa()) == 0 && strcmp(this->elemento_despesa, r.get_elemento_despesa()) == 0){
            return true;
        }
        return false;
    }
};

#endif
