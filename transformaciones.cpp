//*****************************************************************
// File:   transformaciones.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "transformaciones.h"

#define GRAD_A_RAD 3.1415926535898f/180

template<typename T>
using sh_ptr = std::shared_ptr<T>;

template<typename T, std::size_t N>
using array = std::array<T, N>;

template<typename T>
using init_list = std::initializer_list<T>;


Matriz<4, 1> translate(const PuntoDireccion& pd, float x, float y, float z) {
    Matriz<4, 4> m = Matriz<4, 4>(
            init_list<init_list<float>>{
                {1.0f, 0.0f, 0.0f, x},
                {0.0f, 1.0f, 0.0f, y},
                {0.0f, 0.0f, 1.0f, z},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    return m * p;
}


Matriz<4, 1> scale(const PuntoDireccion& pd, float x, float y, float z) {
    Matriz<4, 4> m = Matriz<4, 4>(
            init_list<init_list<float>>{
                {x, 0.0f, 0.0f, 0.0f},
                {0.0f, y, 0.0f, 0.0f},
                {0.0f, 0.0f, z, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    return m * p;
}


Matriz<4, 1> rotateX(const PuntoDireccion& pd, float d) {
    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    Matriz<4, 4> m = Matriz<4, 4>(
        init_list<init_list<float>>{
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, cosD, -sinD, 0.0f},
            {0.0f, sinD, cosD, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    return m * p;
}


Matriz<4, 1> rotateY(const PuntoDireccion& pd, float d) {
    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    Matriz<4, 4> m = Matriz<4, 4>(
        init_list<init_list<float>>{
            {cosD, 0.0f, sinD, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {-sinD, 0.0f, cosD, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    return m * p;
}


Matriz<4, 1> rotateZ(const PuntoDireccion& pd, float d) {
    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    Matriz<4, 4> m = Matriz<4, 4>(
        init_list<init_list<float>>{
            {cosD, -sinD, 0.0f, 0.0f},
            {sinD, cosD, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    return m * p;
}


//Matriz<4, 1> cambioBase(const Punto& p, const Base& b, const Punto& o) {
Punto cambioBase(const Punto& p, const Base& b, const Punto& o, const bool& invertir) {
    Matriz<4, 1> res;
    Matriz<4, 4> m = Matriz<4, 4>(init_list<init_list<float>>{
            {b.base[0][0], b.base[1][0], b.base[2][0], o.coord[0]},
            {b.base[0][1], b.base[1][1], b.base[2][1], o.coord[1]},
            {b.base[0][2], b.base[1][2], b.base[2][2], o.coord[2]},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );
    
    if (invertir) {
        Matriz<4, 4> ucsToLocal = m.inversa();
        res = Matriz<4, 1>((ucsToLocal * p.getCoordHomo()).matriz);
    } else {
        res = Matriz<4, 1>((m * p.getCoordHomo()).matriz);
    }

    return Punto(res.matriz[0][0], res.matriz[1][0], res.matriz[2][0]);
}


Direccion cambioBase(const Direccion& d, const Base& b, const Punto& o, const bool& invertir) {
    Matriz<4, 1> res;
    Matriz<4, 4> m = Matriz<4, 4>(init_list<init_list<float>>{
            {b.base[0][0], b.base[1][0], b.base[2][0], o.coord[0]},
            {b.base[0][1], b.base[1][1], b.base[2][1], o.coord[1]},
            {b.base[0][2], b.base[1][2], b.base[2][2], o.coord[2]},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );
    
    if (invertir) {
        Matriz<4, 4> ucsToLocal = m.inversa();
        res = Matriz<4, 1>((ucsToLocal * d.getCoordHomo()).matriz);
    } else {
        res = Matriz<4, 1>((m * d.getCoordHomo()).matriz);
    }

    // std::cout << m << "\n" << ucsToLocal << std::endl;
    return Direccion(res.matriz[0][0], res.matriz[1][0], res.matriz[2][0]);
}
