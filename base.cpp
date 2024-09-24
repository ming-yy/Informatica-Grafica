//*****************************************************************
// File:   base.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "base.h"


Base::Base() {
    for (int i=0; i<3; i++) {
        base[i].fill(0.0f);
        base[i][i] = 1.0f;
    }
}


Base::Base(const Base& b) : base(b.base) {}


Base::Base(std::initializer_list<std::initializer_list<float>> valores){
    try{
        if((valores.size() != 3) || (valores.begin()->size() != 3)){
            for (auto& vector : base) {
                vector.fill(0.0f);
            }
            throw std::invalid_argument(
                "Error: Dimensiones de la base a crear son incorrectas.");
        }

        std::size_t i = 0;

        for (const auto& fila : valores) {
            std::size_t j = 0;
            for (const auto& valor : fila) {
                base[i][j] = valor;
                j++;
            }
            i++;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
}


Base::Base(std::array<std::array<float, 3>, 3> valores) {
    for (std::size_t i = 0; i < 3; ++i) {
        std::copy(valores[i].begin(), valores[i].end(), base[i].begin());
    }
}


Base::Base(const std::array<float, 3>& arr1, const std::array<float, 3>& arr2,
           const std::array<float, 3>& arr3) {
    std::copy(arr1.begin(), arr1.end(), base[0].begin());
    std::copy(arr2.begin(), arr2.end(), base[1].begin());
    std::copy(arr3.begin(), arr3.end(), base[2].begin());
}


std::ostream& operator<<(std::ostream& os,const Base& b)
{
    for (const auto& vector : b.base) {
        os << "("  << vector[0] << ", " << vector[1] << ", " << vector[2] << ")"
           << std::endl;
    }
    return os;
}
