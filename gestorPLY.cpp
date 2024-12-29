#include "gestorPLY.h"
#include <fstream>
#include <sstream>


vector<Triangulo> generarModeloPLY(const string& rutaArchivo, const float escala) {
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

    cout << "V = " << numVertices << ", C = " << numCaras << endl;
    // Leer los vértices
    for (size_t i = 0; i < numVertices; ++i) {
        //cout << linea << endl;
        getline(archivo, linea);
        std::stringstream ss(linea);
        float x, y, z;
        ss >> x >> y >> z;
        //cout << "x = " << x << ", y = " << y << ", z = " << z <<  endl;
        vertices.emplace_back(x*escala, y*escala, z*escala);
    }
    
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

    return triangulos;
}