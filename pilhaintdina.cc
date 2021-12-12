#include <iostream>
#include <stdlib.h>
#include <sstream>

using namespace std;

class PilhaInt{
    public:
        PilhaInt(int valor);
        ~PilhaInt();
        void empilha(int valor);
        int desempilha();
        int capacidade();
        void redimensiona(int valor);
        bool cheio();
        bool vazio();
        void print(ostream& o);
        
        
        PilhaInt& operator << (int valor){
            empilha(valor);
            return *this;
        }
        
        const PilhaInt& operator = ( const PilhaInt& p ){
            atual = p.atual;
            for(int i = 0 ; i < atual ; i++)
                tab[i] = p.tab[i];
            return *this;
        }

    private:
        int *tab;
        int atual;
        int tamanho;
};

PilhaInt::PilhaInt(int valor = 10){
    tab = (int*) malloc(sizeof(int*)*valor);
    this->tamanho = valor;
    this->atual = 0;
}

PilhaInt::~PilhaInt(){
        free(tab);
}

int PilhaInt::capacidade(){
    return this->tamanho;
}

bool PilhaInt::cheio(){
    return this->atual == this->tamanho;
}

bool PilhaInt::vazio(){
    return this->atual == 0;
}

void PilhaInt::empilha(int valor){
    if(cheio()){
        redimensiona(2*this->tamanho);
    }else{
        this->tab[this->atual] = valor;
        this->atual++;
    }
}

int PilhaInt::desempilha(){
    if(vazio()){
        cout << "underflow" << endl;
        return -1;
    }
    
    this->atual--;
    return this->tab[this->atual];
}

void PilhaInt::redimensiona(int valor){
    if(valor < this->atual){
        int retiradas = this->atual - valor;
        for(int i = 0 ; i < retiradas ; i++)
            this->desempilha();
     }else{
        this->tab = (int*) realloc(tab, sizeof(int) * valor);
    }
    this->tamanho = valor;
}

void PilhaInt::print(ostream& o){
    o << "[ ";
    o << this->tab[0];
   for(int i  = 1 ; i < this->atual ; i++)
        o << ", " << this->tab[i];
    o << " ]";
}