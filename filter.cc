#include <type_traits>
#include <algorithm>

using namespace std;
template<typename T1,typename T2>
auto operator | (const T1 &list , T2 callback){
    if constexpr (std::is_same<decltype(callback(*std::begin(list))), bool>::value){
        auto type = *std::begin(list);
        std::vector<decltype(type)> resposta;
        copy_if(begin(list), end(list),back_inserter(resposta), callback);
        return resposta;

    } else if constexpr (std::is_same<decltype(callback(*std::begin(list))), void>::value) {for_each(begin(list),end(list),callback);}

    else{
        vector<decltype(callback(*std::begin(list)))> resposta;
        transform(*std::begin(list),*std::end(list),back_inserter(resposta),callback);
        return resposta;
    }
}