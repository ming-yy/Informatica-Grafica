#include "gestorPLY.h"
#include <fstream>
#include <sstream>
#include "transformaciones.h"

vector<Triangulo> generarModeloPLY(const string& rutaArchivo, Esfera& boundingSphere, vector<Punto>& verticesFinales,
                                    const float escala, const Punto& centro,
                                    const float rotacionX, const bool invertirX,
                                    const float rotacionY, const bool invertirY,
                                    const float rotacionZ, const bool invertirZ) {
    if(escala <= 0.0f) {
        cout << "Escala incorrecta, no se cargará el modelo";
        return vector<Triangulo>();
    }

    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo: " + rutaArchivo);
    }

    string linea;
    size_t numVertices = 0, numCaras = 0;
    vector<Punto> vertices;
    vector<Triangulo> triangulos;

    // Leer el encabezado
    while (getline(archivo, linea)) {
        if (linea.find("element vertex") != std::string::npos) {
            numVertices = stoi(linea.substr(15));
        } else if (linea.find("element face") != std::string::npos) {
            numCaras = stoi(linea.substr(13));
        } else if (linea == "end_header") {
            break;
        }
    }

    cout << "Archivo = " << rutaArchivo << ", Vertices = " << numVertices << ", Triangulos = " << numCaras << endl;
    // Leer los vértices
    for (size_t i = 0; i < numVertices; ++i) {
        //cout << linea << endl;
        getline(archivo, linea);
        std::stringstream ss(linea);
        float x, y, z;
        ss >> x >> y >> z;
        //cout << "x = " << x << ", y = " << y << ", z = " << z <<  endl;
        Punto p(x,y,z);
        
        // Primero invertimos (en torno al origen de coordenadas)
        if(invertirX){ p = Punto(-p.coord[0], p.coord[1], p.coord[2]); }
        if(invertirY){ p = Punto(p.coord[0], -p.coord[1], p.coord[2]); }
        if(invertirZ){ p = Punto(p.coord[0], p.coord[1], -p.coord[2]); }

        // Luego giramos (en torno al origen de coordenadas)
        if(rotacionX != 0.0f) {p = rotateX(p, rotacionX); }
        if(rotacionY != 0.0f) { p = rotateY(p, rotacionY); }
        if(rotacionZ != 0.0f) { p = rotateZ(p, rotacionZ); }
        
        // Luego escalamos
        if(escala != 1.0f) { p = scale(p, escala, escala, escala); }

        vertices.emplace_back(p);
    }

    Esfera esferaActual = minimumBoundingSphere(vertices);

    // Centramos (movemos) los puntos para centrar la boundingBox al nuevo centro
    Punto desplazamiento(centro.coord[0] - esferaActual.centro.coord[0],
                            centro.coord[1] - esferaActual.centro.coord[1],
                            centro.coord[2] - esferaActual.centro.coord[2]);

    // Mover cada punto sumando el desplazamiento
    for (auto& p : vertices) {
        p = translate(p, desplazamiento.coord[0], desplazamiento.coord[1], desplazamiento.coord[2]);
    }

    boundingSphere = Esfera(centro, esferaActual.radio);
    
    
    // Leer las caras (triángulos)
    for (size_t i = 0; i < numCaras; ++i) {
        //cout << linea << endl;
        getline(archivo, linea);
        std::stringstream ss(linea);
        size_t numIndices, i0, i1, i2;
        ss >> numIndices >> i0 >> i1 >> i2;
        //cout << "i0 = " << i0 << ", i1 = " << i1 << ", i2 = " << i2 <<  endl;
        //cout << "x = " << x << ", y = " << y << ", z = " << z <<  endl;
        if (numIndices == 3) { // Verificamos que sea un triángulo
            triangulos.emplace_back(vertices[i0], vertices[i1], vertices[i2]);
        }
    }

    verticesFinales = vertices;
    return triangulos;
}

Esfera calcularBoundingSphere(const vector<Punto>& puntos) {
    // Paso 1: Encuentra los dos puntos más alejados entre sí
    Punto p1 = puntos[0];
    Punto p2 = puntos[0];
    float maxDist = 0.0f;

    for (size_t i = 0; i < puntos.size(); ++i) {
        for (size_t j = i + 1; j < puntos.size(); ++j) {
            float dist = modulo(puntos[i] - puntos[j]);
            if (dist > maxDist) {
                maxDist = dist;
                p1 = puntos[i];
                p2 = puntos[j];
            }
        }
    }

    // Paso 2: Calcula el centro como el punto medio de los puntos más alejados
    Punto centro = p1.puntoMedio(p2);

    // Paso 3: El radio es la mitad de la distancia entre los dos puntos más alejados
    float radio = maxDist / 2;

    return Esfera(centro, radio);
}

Esfera minimumBoundingSphere(const vector<Punto>& vertices) {
    return calcularBoundingSphere(vertices);
}