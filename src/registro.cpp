#include "registro.h"

vector<string> splitString(string str, char delimiter) {
    
    vector<string> internal;
    stringstream ss(str);
    string tok;
   
    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

int Registro::get_id(){
    return this->id;
}
int* Registro::pget_id(){
    return &this->id;
}
int Registro::get_ano_exercicio(){
    return this->ano_exercicio;
}
int* Registro::pget_ano_exercicio(){
    return &this->ano_exercicio;
}
char* Registro::get_orgao_superior(){
    return this->orgao_superior;
}
char* Registro::get_orgao_subordinado(){
    return this->orgao_subordinado;
}
char* Registro::get_acao(){
    return this->acao;
}
char* Registro::get_grupo_despesa(){
    return this->grupo_despesa;
}
char* Registro::get_elemento_despesa(){
    return this->elemento_despesa;
}
double Registro::get_orcamento_inicial(){
    return this->orcamento_inicial;
}
double* Registro::pget_orcamento_inicial(){
    return &this->orcamento_inicial;
}
double Registro::get_orcamento_atualizado(){
    return this->orcamento_atualizado;
}
double* Registro::pget_orcamento_atualizado(){
    return &this->orcamento_atualizado;
}
double Registro::get_orcamento_empenhado(){
    return this->orcamento_empenhado;
}
double* Registro::pget_orcamento_empenhado(){
    return &this->orcamento_empenhado;
}
double Registro::get_orcamento_realizado(){
    return this->orcamento_realizado;
}
double* Registro::pget_orcamento_realizado(){
    return &this->orcamento_realizado;
}

void print_cabecalho(){
    wcout << setw(5) << right << "ID" << left
          << ". | " << "ANO "
          << " | " << setw(71) << left << L"ORGÃO SUPERIOR"
          << " | " << setw(71) << L"ORGÃO SUBORDINADO"
          << " | " << setw(71) << L"AÇÃO" << endl;
    cout  << setw(235) << setfill('-') << "-" << setfill(' ') << endl;
    wcout << "       | " << setw(78) << left << "GRUPO DE DESPESA"
          << " | " << setw(71) << left << "ELEMENTO DE DESPESA"
          << " | " << setw(15) << right << L"ORÇAM. INICIAL"
          << " | " << setw(15) << right << L"ORÇAM. ATUALIZ."
          << " | " << setw(15) << right << L"ORÇAM. EMPENH."
          << " | " << setw(15) << right << L"ORÇAM. REALIZ." << endl;
    cout  << left << setw(235) << setfill('=') << "=" << setfill(' ') << endl;
    cout  << left << setw(235) << setfill('=') << "=" << setfill(' ') << endl;
}

void Registro::print(){

    if(strlen(this->orgao_superior) <= 71 && strlen(this->orgao_subordinado) <= 71 && strlen(this->acao) <= 71){

        cout << setw(5) << right << this->id << left
             << ". | " << this->ano_exercicio
             << " | " << setw(71) << left << this->orgao_superior
             << " | " << setw(71) << this->orgao_subordinado
             << " | " << setw(71) << this->acao << endl
             << setw(235) << setfill('-') << "-" << setfill(' ') << endl;

    }else if(strlen(this->orgao_superior) <= 2*71 && strlen(this->orgao_subordinado) <= 2*71 && strlen(this->acao) <= 2*71){

        char tmp_superior[72] = {0};
        char tmp_subordinado[72] = {0};
        char tmp_acao[72] = {0};

        memcpy(tmp_superior, this->orgao_superior, 71);
        memcpy(tmp_subordinado, this->orgao_subordinado, 71);
        memcpy(tmp_acao, this->acao, 71);

        cout << setw(5) << right << this->id << left
             << ". | " << this->ano_exercicio
             << " | " << setw(71) << left << tmp_superior
             << " | " << setw(71) << tmp_subordinado
             << " | " << setw(71) << tmp_acao << endl
 
             << "       |      | " << setw(71) << left << this->orgao_superior + 71
             << " | " << setw(71) << this->orgao_subordinado + 71
             << " | " << setw(71) << this->acao + 71 << endl
             << setw(235) << setfill('-') << "-" << setfill(' ') << endl;       
    }else{
        char tmp_superior[72] = {0};
        char tmp_subordinado[72] = {0};
        char tmp_acao[72] = {0};

        char tmp_superior2[72] = {0};
        char tmp_subordinado2[72] = {0};
        char tmp_acao2[72] = {0};

        memcpy(tmp_superior, this->orgao_superior, 71);
        memcpy(tmp_subordinado, this->orgao_subordinado, 71);
        memcpy(tmp_acao, this->acao, 71);

        memcpy(tmp_superior2, this->orgao_superior + 71, 71);
        memcpy(tmp_subordinado2, this->orgao_subordinado + 71, 71);
        memcpy(tmp_acao2, this->acao + 71, 71);

        cout << "       |      | " << setw(71) << left << tmp_superior
             << " | " << setw(71) << tmp_subordinado
             << " | " << setw(71) << tmp_acao << endl

             << setw(5) << right << this->id << left
             << ". | " << this->ano_exercicio << " | "
             << setw(71) << left << tmp_superior2
             << " | " << setw(71) << tmp_subordinado2
             << " | " << setw(71) << tmp_acao2 << endl
 
             << "       |      | " << setw(71) << left << this->orgao_superior + 142
             << " | " << setw(71) << this->orgao_subordinado + 142
             << " | " << setw(71) << this->acao + 142 << endl
             << setw(235) << setfill('-') << "-" << setfill(' ') << endl;
    }

    if(strlen(this->grupo_despesa) <= 78 && strlen(this->elemento_despesa) <= 71){

        cout << "       | " << setw(78) << left << this->grupo_despesa
             << " | " << setw(71) << left << this->elemento_despesa
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_inicial
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_atualizado
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_empenhado
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_realizado << endl
             << left << setw(235) << setfill('=') << "=" << setfill(' ') << endl;
    }else if(strlen(this->grupo_despesa) <= 2*78 && strlen(this->elemento_despesa) <= 2*71){

        char tmp_grupo[79] = {'\0'};
        char tmp_elemento[72] = {'\0'};

        memcpy(tmp_grupo, this->grupo_despesa, 78);
        memcpy(tmp_elemento, this->elemento_despesa, 71);

        cout << "       | " << setw(78) << left << tmp_grupo
             << " | " << setw(71) << left << tmp_elemento
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_inicial
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_atualizado
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_empenhado
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_realizado << endl
 
             << "       | " << setw(78) << left << this->grupo_despesa + 78
             << " | " << setw(71) << left << this->elemento_despesa + 71
             << " | " << setw(15) << ""
             << " | " << setw(15) << ""
             << " | " << setw(15) << ""
             << " | " << setw(15) << "" << endl
             << left << setw(235) << setfill('=') << "=" << setfill(' ') << endl;

    }else{

        char tmp_grupo[79] = {'\0'};
        char tmp_elemento[72] = {'\0'};

        char tmp_grupo2[79] = {'\0'};
        char tmp_elemento2[72] = {'\0'};

        memcpy(tmp_grupo, this->grupo_despesa, 78);
        memcpy(tmp_elemento, this->elemento_despesa, 71);

        memcpy(tmp_grupo2, this->grupo_despesa + 78, 78);
        memcpy(tmp_elemento2, this->elemento_despesa + 71, 71);

        cout << "       | " << setw(78) << left << tmp_grupo
             << " | " << setw(71) << left << tmp_elemento
             << " | " << setw(15) << ""
             << " | " << setw(15) << ""
             << " | " << setw(15) << ""
             << " | " << setw(15) << "" << endl
 
             << "       | " << setw(78) << left << tmp_grupo2
             << " | " << setw(71) << left << tmp_elemento2
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_inicial
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_atualizado
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_empenhado
             << " | " << setprecision(2) << right << fixed << setw(15) << this->orcamento_realizado << endl
 
             << "       | " << setw(78) << left << this->grupo_despesa + 156
             << " | " << setw(71) << left << this->elemento_despesa + 142
             << " | " << setw(15) << ""
             << " | " << setw(15) << ""
             << " | " << setw(15) << ""
             << " | " << setw(15) << "" << endl
             << left << setw(235) << setfill('=') << "=" << setfill(' ') << endl;
    } 
}

void Registro::write_to_file(ofstream &file_out){

    file_out.write((char*)this->pget_id(), sizeof(int));
    file_out.write((char*)this->pget_ano_exercicio(), sizeof(int));
    file_out.write((char*)this->get_orgao_superior(), sizeof(char)*200);
    file_out.write((char*)this->get_orgao_subordinado(), sizeof(char)*200);
    file_out.write((char*)this->get_acao(), sizeof(char)*200);
    file_out.write((char*)this->get_grupo_despesa(), sizeof(char)*200);
    file_out.write((char*)this->get_elemento_despesa(), sizeof(char)*200);
    file_out.write((char*)this->pget_orcamento_inicial(), sizeof(double));
    file_out.write((char*)this->pget_orcamento_atualizado(), sizeof(double));
    file_out.write((char*)this->pget_orcamento_empenhado(), sizeof(double));
    file_out.write((char*)this->pget_orcamento_realizado(), sizeof(double));
}