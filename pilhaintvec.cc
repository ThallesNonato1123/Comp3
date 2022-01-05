#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <vector>

using namespace std;

class PilhaInt{
    public:
        PilhaInt(int valor);
        PilhaInt(const PilhaInt &p);
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
            if(this != &p){
            this->tamanho = p.tamanho;
            this->atual = p.atual;
            this->tab.resize(this->tamanho);
            this->tab.clear();
            for(int i = 0 ; i < this->atual ; i++){
                this->tab[i] = p.tab[i];
                }
            }
            return *this;
        }

    private:
        vector<int> tab;
        int atual;
        int tamanho;
};

PilhaInt::PilhaInt(int valor = 10){
    this->tab.resize(valor);
    this->tamanho = valor;
    this->atual = 0;
}

PilhaInt::PilhaInt(const PilhaInt &p){
    this->tamanho = p.tamanho;
    this->atual = p.atual;
    this->tab.resize(this->tamanho);
    for(int i = 0 ; i<this->atual; i++){
        this->tab[i] = p.tab[i];
    }
}

PilhaInt::~PilhaInt(){
    this->tab.clear();
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
        this->tab[this->atual] = valor;

    }else{
        this->tab[this->atual] = valor;
    }
    this->atual++;
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
        this->tab.resize(valor);
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

int main(){
   PilhaInt a{7}, b{5000}, c{5};
a << 8 << 3 << 1 << 4 << 5;
for( int i = 0; i < 5000; i++ )
  b << i;
c = a;
a = b;
b = c;
cout << a.capacidade() << ", " << b.capacidade() << ", " << c.capacidade() << endl;
}