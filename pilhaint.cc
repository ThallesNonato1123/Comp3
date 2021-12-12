#include <iostream>
#include <sstream>

using namespace std;

class PilhaInt{
    public:
        PilhaInt();
        void empilha(int valor);
        int desempilha();
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
        int tab[10];
        int atual;
};

PilhaInt::PilhaInt(){
    for(int i = 0 ; i < 10 ; i++)
        tab[i] = 0;
    atual = 0;
}

void PilhaInt::empilha(int valor){
    tab[atual++] = valor;
}


int PilhaInt::desempilha(){
    return tab[--atual];
}

void PilhaInt::print(ostream& o){
    o << "[ ";
    o << tab[0];
   for(int i  = 1 ; i < atual ; i++)
         o << ", " << tab[i];
    o << " ]";
}

int main(){
    PilhaInt p(5);
    p.empilha( 1 );
    p << 3 << 9 << 13 << 89;
    cout << p.desempilha() << endl;
    cout << p.desempilha() << endl;
    p.print( cout );
    p << 19 << 18 << 17 << 30;
    stringstream ss;
    p.print( ss );
    cout << endl << "{" << ss.str() << "}" << endl;
}