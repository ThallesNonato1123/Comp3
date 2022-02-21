#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <type_traits>
#include <vector>

using namespace std;


template<typename E> 
struct Expr { 
    Expr(E e): e(e) {} 
 
    template<typename Tipo> 
    auto operator() (Tipo v) const { 
        return e(v); 
    } 
 
    template<typename Tipo> 
    auto operator() (Tipo v) { 
        return e(v); 
    } 
    E e; 
}; 

auto x = Expr{[](auto x) { return x; }};

template <typename Tipo>
class OStream{
public:
  OStream(ostream &o, Tipo t) : o(o), t(t) {}
  template<typename T1> auto operator()(T1 v){
    o << t(v);
    o << buffer;
  }
  
  auto depois(char c){
    buffer += c;
  }
private:
  ostream &o;
  Tipo t;
  string buffer = "";
};

template <typename T,enable_if_t<!is_same_v<T,string>,bool> = true> 
  OStream<T> operator<<(ostream &o, T t){
  return OStream<T>(o, t);
}

template <typename A>
auto operator<<(OStream<A> a, const char &c){
  a.depois(c);
  return a;
}

template <typename T1, typename T2> auto operator+(T1 a, T2 b) {
  if constexpr (is_arithmetic<decltype(a)>::value){
    return Expr{[a, b](auto v) { return a + b(v); }};
  } else if constexpr (is_arithmetic<decltype(b)>::value) {
    printf("oi");
    return Expr{[a, b](auto v) { return b + a(v); }};
  } else {
    return Expr{[a, b](auto v) { return a(v) + b(v); }};
  }
}

template <typename T1, typename T2> auto operator-(T1 a, T2 b) {
  if constexpr (is_arithmetic<decltype(a)>::value) {
    return Expr{[a, b](auto v) { return a - b(v); }};
  } else if constexpr (is_arithmetic<decltype(b)>::value) {
    return Expr{[a, b](auto v) { return b - a(v); }};
  } else {
    return Expr{[a, b](auto v) { return a(v) - b(v); }};
  }
}

template <typename A, typename B> 
auto operator*(Expr<A> a, Expr<B> b) {return Expr{[a, b](auto v){ return a(v) * b(v); }}; };
template <typename T1, typename T2>
auto operator/(T1 a, T2 b) { // a/b.dx  a/b   a/b.dx   a.dx/b
  if constexpr (is_arithmetic<decltype(a)>::value) {
    return Expr{[a, b](auto v) { return a / b(v); }};
  } else if constexpr (is_arithmetic<decltype(b)>::value) {
    return Expr{[a, b](auto v) { return a(v) / b; }};
  } else {
    return Expr{[a, b](auto v) { return a(v) / b(v); }};
  }
}

template<typename A, typename B> 
auto operator% (Expr<A> a, B b) { 
    return Expr{ [a, b](auto v) { return a(v) % b; } }; 
} 

template<typename T1,typename T2>
auto operator | (const T1 &list , T2 callback){
    if constexpr (std::is_same<decltype(callback(*std::begin(list))), bool>::value){
        auto type = *std::begin(list);
        std::vector<decltype(type)> resposta;
        copy_if(begin(list), end(list),back_inserter(resposta), callback);
        return resposta;
    }else if constexpr (std::is_void<decltype(callback(*std::begin(list)))>::value) {for_each(begin(list),end(list),callback);}
    
    else if constexpr (std::is_function<decltype(callback)>::value){
        vector<decltype(callback(*std::begin(list)))> resposta;
        transform(*std::begin(list),*std::end(list),back_inserter(resposta),callback);
        return resposta;
    }    
    else{
        std::vector<decltype(callback(*std::begin(list)))> resposta;
        for(const auto &x: list)
            resposta.push_back(callback(x));
        return resposta;
    }
}
template<typename T1>
auto operator |(const T1 &list, decltype(&std::string::length) callbackMethod){
    std::vector<size_t> resposta;
    for(const auto &x: list)
        resposta.push_back(std::invoke(callbackMethod, x));
    return resposta;    
}

template <typename A, typename B>
auto operator==(Expr<A> a, B b){
  return Expr{[a, b](auto v)
              { return a(v) == b; }};
};

int main() {
    auto s = x + 2;
    cout << s(2);
}
