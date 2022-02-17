#include <iostream>
#include <functional>

using std::invoke, std::is_class_v, std::cout, std::is_invocable_v;

template<typename Function>
struct Bind{
    Bind(Function f ):f(f){}
    template<typename Arg>
    auto operator()(Arg arg){
        return [this,arg](auto ...resto){return invoke(this->f, arg,resto...);};
    }
    
    template<typename ...Args> 
    auto operator()(Args ...args){
            if constexpr(is_invocable_v<Function,Args...>)
                return invoke(this->f, args...);
            else{
                return [this, args...](auto ...resto){ return invoke(this->f,args...,resto...); };
            }
    }
    Function f;
};

template<typename Function>
auto bind(Function f){
    return Bind{f};
}

template<typename Function,typename ...Args>
auto bind(Bind<Function> f, Args ...args){
     if constexpr(is_invocable_v<Function,Args...>)
                return invoke(f->f, args...);
            else{
                return [f, args...](auto ...resto){ return invoke(f.f,args...,resto...); };
            }
}

template<typename Function, typename ...Args>
auto bind(Function f, Args ...args){
     if constexpr(is_invocable_v<Function,Args...>)
                return invoke(f, args...);
    else if constexpr (is_class_v<Function>) {
            return [&f, args...](auto...resto){ return invoke(f, args..., resto...); };
        }
        else{
                return [f, args...](auto ...resto){ return invoke(f,args...,resto...); };
            }
}