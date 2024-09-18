//*****************************************************************
// File:   transformaciones.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "transformaciones.h"
#include <cmath>
#include <numbers>

#define GRAD_A_RAD 3.1415926535898f/180


Matriz<4, 1> translate(const std::shared_ptr<PuntoDireccion>& pd, float x, float y, float z) {

    std::shared_ptr<Matriz<4, 4>> m = std::make_shared<Matriz<4, 4>>(
            std::initializer_list<std::initializer_list<float>>{
                {1.0f, 0.0f, 0.0f, x},
                {0.0f, 1.0f, 0.0f, y},
                {0.0f, 0.0f, 1.0f, z},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    std::shared_ptr<Matriz<4, 1>> p = std::make_shared<Matriz<4, 1>>(pd->aMatriz4x1());

    return *m * *p;
}

Matriz<4, 1> scale(const std::shared_ptr<PuntoDireccion>& pd, float x, float y, float z) {

    std::shared_ptr<Matriz<4, 4>> m = std::make_shared<Matriz<4, 4>>(
            std::initializer_list<std::initializer_list<float>>{
                {x, 0.0f, 0.0f, 0.0f},
                {0.0f, y, 0.0f, 0.0f},
                {0.0f, 0.0f, z, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    std::shared_ptr<Matriz<4, 1>> p = std::make_shared<Matriz<4, 1>>(pd->aMatriz4x1());

    return *m * *p;
}

Matriz<4, 1> rotateX(const std::shared_ptr<PuntoDireccion>& pd, float d) {

    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    std::shared_ptr<Matriz<4, 4>> m = std::make_shared<Matriz<4, 4>>(

            std::initializer_list<std::initializer_list<float>>{
                {1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, cosD, -sinD, 0.0f},
                {0.0f, sinD, cosD, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    std::shared_ptr<Matriz<4, 1>> p = std::make_shared<Matriz<4, 1>>(pd->aMatriz4x1());

    return *m * *p;
}

Matriz<4, 1> rotateY(const std::shared_ptr<PuntoDireccion>& pd, float d) {

    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    std::shared_ptr<Matriz<4, 4>> m = std::make_shared<Matriz<4, 4>>(

            std::initializer_list<std::initializer_list<float>>{
                {cosD, 0.0f, sinD, 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {-sinD, 0.0f, cosD, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    std::shared_ptr<Matriz<4, 1>> p = std::make_shared<Matriz<4, 1>>(pd->aMatriz4x1());

    return *m * *p;
}

Matriz<4, 1> rotateZ(const std::shared_ptr<PuntoDireccion>& pd, float d) {
    
    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    std::shared_ptr<Matriz<4, 4>> m = std::make_shared<Matriz<4, 4>>(

            std::initializer_list<std::initializer_list<float>>{
                {cosD, -sinD, 0.0f, 0.0f},
                {sinD, cosD, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    std::shared_ptr<Matriz<4, 1>> p = std::make_shared<Matriz<4, 1>>(pd->aMatriz4x1());

    return *m * *p;
}


/*
Matriz<4, 1> changeBase(const std::shared_ptr<PuntoDireccion>& pd, ...) {

}
*/
