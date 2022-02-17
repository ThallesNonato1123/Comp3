#include <iostream>
#include <functional>

using std::is_invocable_v, std::invoke, std::is_class_v, std::cout;
template<typename Function>
auto bind(Function f){
    return f;
}

template<typename Function, typename ...Args>
auto bind(Function f, Args ...args){
     if constexpr(is_invocable_v<Function,Args...>)
                return invoke(f, args...);
            else
                return [f, args...](auto ...outros){ return invoke(f, args..., outros...); };
}
