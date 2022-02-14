#include <iostream>
#include <type_traits>
#include <math.h>
#include <functional>
using namespace std;

template< typename E , typename Dx>
struct Expr{
    Expr(E e , Dx dx): e(e), dx(dx){}
    template<typename E2, typename Dx2, typename E3, typename Dx3>
    auto operator()(Expr<E2,Dx2> g) const {// sin(x) | sin(g(x))
        return Expr<E3,Dx3>{[*this,g](double v){return e(g.e(v));},[*this,g](double v){return {g.dx(v) * dx(g.e(v))};}};
    }
    E e;
    Dx dx;
};

auto x = Expr{ [] (double v){return v;},[](double v ){return 1;}};
auto Cte = Expr{[](double c){return [c](double v){return c;};},[](double c){return [c](double v){return 0;};}};


template<typename E, typename Dx>
auto sin(Expr<E,Dx> g){
    return Expr{ [g] (double v){return sin(g.e(v));},[g](double v ){return g.dx(v) * cos(g.e(v));}};
}

template<typename E , typename Dx>
auto cos(Expr<E,Dx> g){
    return Expr{[g](double v){return cos(g.e(v));},[g](double v){return g.dx(v) * -sin(g.e(v));}};
}

template<typename E, typename Dx>
auto log(Expr<E,Dx> g){
    return Expr{ [g] (double v){return std::log(g.e(v));},[g](double v ){return g.dx(v) * 1/g.e(v);}};
}

template <typename T1, typename T2>
auto operator+(T1 a , T2 b){
    if constexpr(is_arithmetic<decltype(a)>::value && is_class<decltype(b)>::value){
        return Expr{[a,b](double v){return Cte.e(a)(a) + b.e(v);}, [a,b](double v){return Cte.dx(a)(a) + b.dx(v);}};
    }else if constexpr(is_arithmetic<decltype(b)>::value && is_class<decltype(a)>::value){
        return Expr{[a,b](double v){return Cte.e(b)(b) + a.e(v);}, [a,b](double v){return Cte.dx(b)(b) + a.dx(v);}};
    }else{
        return Expr{[a,b](double v){return a.e(v) + b.e(v);},[a,b](double v){return a.dx(v) + b.dx(v);}};
    }
}

template <typename T1, typename T2>
auto operator-(T1 a , T2 b){
    if constexpr(is_arithmetic<decltype(a)>::value && is_class<decltype(b)>::value){
        return Expr{[a,b](double v){return Cte.e(a)(a) - b.e(v);}, [a,b](double v){return Cte.dx(a)(a) - b.dx(v);}};
    }else if constexpr(is_arithmetic<decltype(b)>::value && is_class<decltype(a)>::value){
        return Expr{[a,b](double v){return Cte.e(b)(b) - a.e(v);}, [a,b](double v){return Cte.dx(b)(b) - a.dx(v);}};
    }else{
        return Expr{[a,b](double v){return a.e(v) - b.e(v);},[a,b](double v){return a.dx(v) - b.dx(v);}};
    }
}

template <typename T1, typename T2>
auto operator* ( T1 a , T2 b){
    if constexpr(is_arithmetic<decltype(a)>::value && is_class<decltype(b)>::value){
        return Expr{[a,b](double v){return Cte.e(a)(a) * b.e(v);}, [a,b](double v){return Cte.e(a)(a)* b.dx(v);}};
    }else if constexpr(is_arithmetic<decltype(b)>::value && is_class<decltype(a)>::value){
        return Expr{[a,b](double v){return b * a.e(v);}, [a,b](double v){return Cte.e(b)(b) * a.dx(v);}};
    }else{
        return Expr{[a,b](double v){return a.e(v) * b.e(v);},[a,b](double v){return a.dx(v) * b.e(v) + b.dx(v) * a.e(v);}};
    }
}

template <typename T1, typename T2>
auto operator/ ( T1 a , T2 b){  // a/b.dx  a/b   a/b.dx   a.dx/b
    if constexpr(is_arithmetic<decltype(a)>::value && is_class<decltype(b)>::value){
        return Expr{[a,b](double v){return Cte.e(a)(a) / b.e(v);}, [a,b](double v){return Cte.e(a)(a) / b.dx(v);}};
    }else if constexpr(is_arithmetic<decltype(b)>::value && is_class<decltype(a)>::value){
        return Expr{[a,b](double v){return a.e(v)/Cte.e(b)(b);}, [a,b](double v){return a.dx(v) / Cte.e(b)(b);}};
    }else{
    return Expr{[a,b](double v){return a.e(v) / b.e(v);},[a,b](double v){return (a.dx(v) * b.e(v) - b.dx(v) * a.e(v)) / pow(b.e(v),2);}};
    }
}

int main(){
    
    auto f = sin( x * x - cos( 3.14 * x + 1.0 ));
    cout << f.e(3.14) << endl;
    cout << f.dx(3.14) << endl;
}