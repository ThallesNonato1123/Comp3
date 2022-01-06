template <typename T , typename F>
auto apply(const T& vector , F func){
    std::vector<decltype(func(*std::begin(vector)))> resposta;
    for(const auto &x : vector)
        resposta.push_back(func(x));
    return resposta;
}

template <typename T , typename F>
auto apply(std::initializer_list<T>vector , F func){
    std::vector<decltype(func(*std::begin(vector)))> resposta;
    for(const auto &x : vector)
        resposta.push_back(func(x));
    return resposta;
}