//*****************************************************************
// File:   base.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "base.h"

template<typename T, std::size_t N>
using array = std::array<T, N>;

template<typename T>
using init_list = std::initializer_list<T>;

using std::cerr;
using std::endl;
using std::size_t;
using std::ostream;
using std::copy;
using std::invalid_argument;

Base::Base() {
    for (int i=0; i<3; i++) {
        base[i].fill(0.0f);
        base[i][i] = 1.0f;
    }
}


Base::Base(const Base& b) : base(b.base) {}


Base::Base(init_list<init_list<float>> valores){
    try{
        if((valores.size() != 3) || (valores.begin()->size() != 3)){
            for (auto& vector : base) {
                vector.fill(0.0f);
            }
            throw invalid_argument(
                "Error: Dimensiones de la base a crear son incorrectas.");
        }

        size_t i = 0;

        for (const auto& fila : valores) {
            size_t j = 0;
            for (const auto& valor : fila) {
                base[i][j] = valor;
                j++;
            }
            i++;
        }
    } catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }
}


Base::Base(array<array<float, 3>, 3> valores) {
    for (size_t i = 0; i < 3; ++i) {
        copy(valores[i].begin(), valores[i].end(), base[i].begin());
    }
}


Base::Base(const array<float, 3>& arr1, const array<float, 3>& arr2,
           const array<float, 3>& arr3) {
    copy(arr1.begin(), arr1.end(), base[0].begin());
    copy(arr2.begin(), arr2.end(), base[1].begin());
    copy(arr3.begin(), arr3.end(), base[2].begin());
}


ostream& operator<<(ostream& os,const Base& b)
{
    for (const auto& vector : b.base) {
        os << "("  << vector[0] << ", " << vector[1] << ", " << vector[2] << ")"
           << endl;
    }
    return os;
}
