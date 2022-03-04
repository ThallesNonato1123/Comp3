#include <iostream>
#include <type_traits>
#include <math.h>
#include <functional>
#include <sstream>
#include <string>

using namespace std;

template< typename E , typename Dx>
struct Expr{
    Expr(E e , Dx dx): str(e), dx_str(dx){}
    E str;
    Dx dx_str;
};



auto x = Expr{ [](){return "x";},[](){return "1";}};
auto cte = Expr{[](double v){
    std::ostringstream streamObj;
    streamObj << v;
    std::string strObj = streamObj.str();
    return strObj;},[](double v){return "0";}};
template<typename E, typename Dx>

auto sin(Expr<E,Dx> g){
    return Expr{[g](){return "sin(" + string(g.str()) +")";},[g](){return "(cos(" + string(g.str()) + ")*" + string(g.dx_str()) + ")";}};
}

template<typename E , typename Dx>
auto cos(Expr<E,Dx> g){
    return Expr{[g](){return "cos(" + string(g.str()) +")";},[g](){return "(-sin(" + string(g.str()) + ")*" + string(g.dx_str()) + ")" ;}};
}

template<typename E, typename Dx>
auto log(Expr<E,Dx> g){
    return Expr{ [g] (){return "log("  + string(g.str()) + ")";},[g]( ){return "(1/(" + g.str() + "))*(" +  g.dx_str() + ")" ;}};
}

template<typename E, typename Dx>
auto exp(Expr<E,Dx> g){
    return Expr{ [g] (){return "exp("+ string(g.str()) +")";},[g]( ){return "(exp(" + g.str() + ")*(" + g.dx_str() + "))";}};
}

template <typename T1, typename T2>
auto operator+(T1 a , T2 b){
    if constexpr(is_arithmetic<decltype(a)>::value && is_class<decltype(b)>::value){
        return Expr{[a,b](){return "((" + cte.str(a)  + ")+(" +  b.str() + "))";}, [a,b](){return "((" + string(cte.dx_str(a))  + std::string(")+(") +  string(b.dx_str())+ "))";}};
    }else if constexpr(is_arithmetic<decltype(b)>::value && is_class<decltype(a)>::value){
        return Expr{[a,b](){return "((" + string(a.str()) + ")+("  + string(cte.str(b)) + "))";}, [a,b](){return "((" + string(a.dx_str()) + ")+(" + string(cte.dx_str(b)) + "))";}};
    }else{
        return Expr{[a,b](){return "((" + std::string(a.str()) + ")+(" + std::string(b.str()) + "))";},[a,b](){return "((" +  string(a.dx_str())  + ")+(" +   string(b.dx_str()) + "))";}};
    }
}

template <typename T1, typename T2>
auto operator-(T1 a , T2 b){
    if constexpr(is_arithmetic<decltype(a)>::value && is_class<decltype(b)>::value){
        return Expr{[a,b](){return "((" + cte.str(a)  + ")-(" +  b.str() + "))";}, [a,b](){return "((" + string(cte.dx_str(a))  + std::string(")-(") +  string(b.dx_str())+ "))";}};
    }else if constexpr(is_arithmetic<decltype(b)>::value && is_class<decltype(a)>::value){
        return Expr{[a,b](){return "((" + string(a.str()) + ")-("  + string(cte.str(b)) + "))";}, [a,b](){return "((" + string(a.dx_str()) + ")-(" + string(cte.dx_str(b)) + "))";}};
    }else{
        return Expr{[a,b](){return "((" + std::string(a.str()) + ")-(" + std::string(b.str()) + "))";},[a,b](){return "((" +  string(a.dx_str())  + ")-(" +   string(b.dx_str()) + "))";}};
    }
}

template <typename T1, typename T2>
auto operator* ( T1 a , T2 b){
    if constexpr(is_arithmetic<decltype(a)>::value && is_class<decltype(b)>::value){ //3*x
        return Expr{[a,b](){return "((" +  string(cte.str(a)) + ")*(" +  string(b.str()) + "))";},[a,b](){return "((" + string(cte.dx_str(a)) + ")*(" + string(b.str()) + ")+(" +  string(cte.str(a)) + ")*(" + string(b.dx_str()) + "))";}};
    }else if constexpr(is_arithmetic<decltype(b)>::value && is_class<decltype(a)>::value){
        return Expr{[a,b](){return "((" +  string(a.str()) + ")*(" +  string(b.str()) + "))";},[a,b](){return "((" + string(a.dx_str()) + ")*(" + string(cte.str(b)) + ")+(" +  string(a.str()) + ")*(" + string(cte.dx_str(b)) + "))";}}; 
    }else{
        return Expr{[a,b](){return "((" +  string(a.str()) + ")*(" +  string(b.str()) + "))";},[a,b](){return "((" + string(a.dx_str()) + ")*(" + string(b.str()) + ")+(" +  string(a.str()) + ")*(" + string(b.dx_str()) + "))";}};
    }                                                                                                           
}

template <typename T1, typename T2>
auto operator/ ( T1 a , T2 b){  // a/b.dx  a/b   a/b.dx   a.dx/b // x/3.14
    if constexpr(is_arithmetic<decltype(a)>::value && is_class<decltype(b)>::value){ // 3/x.14
        return Expr{[a,b](){return "((" + string(cte.str(a)) + ")/(" +  string(b.str()) + "))";}, [a,b](){return "((("+ string(cte.dx_str(a))+")*("+ string(b.str())+")-("+string(cte.str(a))+")*("+string(b.dx_str())+"))/"+string(cte.str(a))+"^2)";}};
    }else if constexpr(is_arithmetic<decltype(b)>::value && is_class<decltype(a)>::value){ // x/3.14
        return Expr{[a,b](){return "((" + string(a.str()) + ")/(" + cte.str(b) + "))";}, [a,b](){return "(((" + string(a.dx_str()) + ")*(" +  string(cte.str(b)) + ")-(" + string(a.str()) +  ")*(" +  string(cte.dx_str(b)) + ")/" +  string(cte.str(b)) + "^2)";}};;
    }else{
    return Expr{[a,b](){return "((" + a.str() + ")/(" +  b.str() + "))";}, [a,b](){return "((("+a.dx_str()+")*("+b.str()+")-("+a.str()+")*("+b.dx_str()+"))/"+a.str()+"^2)";}};;
    }
}
template<typename E, typename Dx, typename T1>
auto operator->*(Expr<E,Dx> g, T1 b){
    static_assert( std::is_same_v< int, T1 >, "Operador de potenciação definido apenas para inteiros" );
    return Expr{[g,b]( ){ return "(" + string(g.str()) +")^" + to_string(b);},[g,b](){ return "(" +  to_string(b) +  "*("+ g.dx_str()+")*("+ g.str() + ")^" + to_string(b-1) + ")";}};
}

int main(){
     auto f = log( (8.1/(x+1.0)->*3 - 9.2 ) * (x + 3.0) *sin( cos( x / 3.14 ) ) + x );
    cout << f.dx_str();
}