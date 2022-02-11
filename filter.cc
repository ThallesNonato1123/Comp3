#include <type_traits>
#include <algorithm>
#include <functional>

using namespace std;
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
    std::vector<decltype(&std::string::length)> resposta;
    for(const auto &x: list)
        resposta.push_back(std::invoke(callbackMethod, x));
    return resposta;    
}
