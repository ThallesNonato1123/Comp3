#include<iostream>
#include<vector>
using namespace std;

template <int qtd, typename T> class Vetor{
    public:
        Vetor(){
            for(int i = 0 ; i < qtd ; i++)
                coordenadas[i] = 0;
        }
        
        class MeioDoProdutoVetorial{
            public:
                MeioDoProdutoVetorial(const Vetor<qtd,T> &aux): coordenadas(aux.coordenadas){ }
        private:
                T *coordenadas = new T[qtd];;
                friend Vetor;
        };  
        void print(ostream& o) const{
            for(int i = 0 ; i < qtd ; i++)
                o << this->coordenadas[i] << " ";
            o << endl;
        }
        Vetor& operator = (initializer_list<T> coords){
            int indice = 0; 
            for(T x : coords){
                coordenadas[indice] = x;
                indice++;
            }
            indice = 0;
            return *this;
        }
        
        Vetor<qtd,T> operator + (const Vetor rhs){
            Vetor<qtd,T> resultado;
            for(int i = 0 ; i < qtd ; i++){
                resultado.coordenadas[i] = this->coordenadas[i] + rhs.coordenadas[i];
            }
            return resultado;
        }
        
        Vetor<qtd,T> operator - (const Vetor rhs){
            Vetor<qtd,T> resultado;
            for(int i = 0 ; i < qtd ; i++){
                resultado.coordenadas[i] = this->coordenadas[i] - rhs.coordenadas[i];
            }
            return resultado;
        }
        
        Vetor<qtd,T> operator * (T escalar){
            Vetor<qtd,T> resultado;
            for(int i = 0 ; i < qtd ; i++){
                resultado.coordenadas[i] = this->coordenadas[i] * escalar;
            }
            return resultado;
        }
        
        Vetor<qtd,T> operator / (T escalar){
            Vetor<qtd,T> resultado;
            for(int i = 0 ; i < qtd ; i++){
                resultado.coordenadas[i] = this->coordenadas[i] / escalar;
            }
            return resultado;
        }

        T operator * (const Vetor rhs){
            T prodint = 0;
            for(int i = 0 ; i < qtd ; i++){
                prodint += this->coordenadas[i] * rhs.coordenadas[i];
            }
            return prodint;
        }
        MeioDoProdutoVetorial operator*(){
            return MeioDoProdutoVetorial(*this);
        }
        Vetor<qtd,T> operator*(const MeioDoProdutoVetorial &rhs){
            Vetor<qtd,T> prodvet;      
            if(qtd == 3){
                prodvet.coordenadas[0] = this->coordenadas[1] *rhs.coordenadas[2] - this->coordenadas[2]*rhs.coordenadas[1];
                prodvet.coordenadas[1] = this->coordenadas[2] *rhs.coordenadas[0] - this->coordenadas[0]*rhs.coordenadas[2];
                prodvet.coordenadas[2] = this->coordenadas[0] *rhs.coordenadas[1] - this->coordenadas[1]*rhs.coordenadas[0];
            }
        return prodvet;
    }
    private:
        T *coordenadas = new T[qtd];
};

template<int qtd , typename T>
ostream& operator << (ostream& o , const Vetor<qtd,T> &rhs){
    rhs.print(o);
    return o;
}

template<int N, typename T>
auto operator*(T escalar, Vetor<N, T> &v){    
    return v * escalar;
}

int main( int argc, char* argv[]) {
    Vetor< 3, double> a, b;
    int caso;
    double x, y, z;
    cin >> caso >> x >> y >> z;
    a = { x, y, z };
    b = { 1, 2, 3 };
    switch( caso ) {
    case 1: cout << a << endl; break;
    case 2: cout << a + b << endl; break;
    case 3: {
        Vetor< 3, double> c;
        c = a ** b; 
        cout << c << endl;
        break; 
    } 
      case 4: cout << a * b << endl; break;
      case 5: cout << a * 3.0 << endl; break;
      case 6: cout << 2.1 * a << endl; break;
      case 7: cout << a + b + a * 2.0 << endl; break;
    }
  return 0;
}