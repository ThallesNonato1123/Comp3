#include <iostream>
#include <type_traits>
#include <math.h>
#include <functional>
using namespace std;

template< typename E , typename Dx>
struct Expr{
    Expr(E e , Dx dx): e(e), dx(dx){}
    E e;
    Dx dx;
};

auto x = Expr{ [] (double v){return v;},[](double v ){return 1;}};
auto Cte = Expr{[](double c){return [c](double v){return c;};},[](double c){return [c](double v){return 0;};}};

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
    auto f = x*x*x*(8+x)+x;
    cout << f.dx(1.1);
}
