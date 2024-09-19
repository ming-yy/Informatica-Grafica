//*****************************************************************
// File:   transformaciones.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "transformaciones.h"

#define GRAD_A_RAD 3.1415926535898f/180


Matriz<4, 1> translate(const PuntoDireccion& pd, float x, float y, float z) {
    std::shared_ptr<Matriz<4, 4>> m = std::make_shared<Matriz<4, 4>>(
            std::initializer_list<std::initializer_list<float>>{
                {1.0f, 0.0f, 0.0f, x},
                {0.0f, 1.0f, 0.0f, y},
                {0.0f, 0.0f, 1.0f, z},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    std::shared_ptr<Matriz<4, 1>> p = std::make_shared<Matriz<4, 1>>(pd.getCoordHomo());
    return *m * *p;
}


Matriz<4, 1> scale(const PuntoDireccion& pd, float x, float y, float z) {
    std::shared_ptr<Matriz<4, 4>> m = std::make_shared<Matriz<4, 4>>(
            std::initializer_list<std::initializer_list<float>>{
                {x, 0.0f, 0.0f, 0.0f},
                {0.0f, y, 0.0f, 0.0f},
                {0.0f, 0.0f, z, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    std::shared_ptr<Matriz<4, 1>> p = std::make_shared<Matriz<4, 1>>(pd.getCoordHomo());
    return *m * *p;
}


Matriz<4, 1> rotateX(const PuntoDireccion& pd, float d) {
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

    std::shared_ptr<Matriz<4, 1>> p = std::make_shared<Matriz<4, 1>>(pd.getCoordHomo());
    return *m * *p;
}


Matriz<4, 1> rotateY(const PuntoDireccion& pd, float d) {
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

    std::shared_ptr<Matriz<4, 1>> p = std::make_shared<Matriz<4, 1>>(pd.getCoordHomo());
    return *m * *p;
}


Matriz<4, 1> rotateZ(const PuntoDireccion& pd, float d) {
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

    std::shared_ptr<Matriz<4, 1>> p = std::make_shared<Matriz<4, 1>>(pd.getCoordHomo());
    return *m * *p;
}


Matriz<4, 1> cambioBase(const Punto& p, const Base& b, const Punto& o) {
    std::shared_ptr<Matriz<4, 4>> m = std::make_shared<Matriz<4, 4>>(
        std::initializer_list<std::initializer_list<float>>{
            {b.base[0][0], b.base[1][0], b.base[2][0], o.coord[0]},
            {b.base[0][1], b.base[1][1], b.base[2][1], o.coord[1]},
            {b.base[0][2], b.base[1][2], b.base[2][2], o.coord[2]},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );
    
    std::shared_ptr<Matriz<4, 1>> pCH = std::make_shared<Matriz<4, 1>>(p.getCoordHomo());
    
    return *m * *pCH;
}


// // Function to calculate and store inverse, returns false if
// // matrix is singular
// Matriz<4,4> invert(const Matriz<4,4>& m)
// {
//     // Find determinant of A[][]
//     int det = determinant(m, 4);
//     if (det == 0) {
//         cout << "Singular matrix, can't find its inverse";
//         return false;
//     }

//     // Find adjoint
//     Matriz<4,4> = adj(N, vector<int>(N));
//     adjoint(A, adj);

//     // Find Inverse using formula "inverse(A) =
//     // adj(A)/det(A)"
//     for (int i = 0; i < N; i++)
//         for (int j = 0; j < N; j++)
//             inv[i][j] = adj[i][j] / float(det);

//     return true;
// }


// /* Recursive function for finding determinant of matrix.
// n is current dimension of A[][]. */
// int determinant(const Matriz<4,4> &m)
// {
//     int D = 0; // Initialize result


//     vector<vector<int> > temp(
//         N, vector<int>(N)); // To store cofactors

//     int sign = 1; // To store sign multiplier

//     // Iterate for each element of first row
//     for (int f = 0; f < n; f++) {
//         // Getting Cofactor of A[0][f]
//         getCofactor(A, temp, 0, f, n);
//         D += sign * A[0][f] * determinant(temp, n - 1);

//         // terms are to be added with alternate sign
//         sign = -sign;
//     }

//     return D;
// }
