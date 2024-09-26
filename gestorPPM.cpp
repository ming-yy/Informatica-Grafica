
#include "gestorPPM.h"

using std::string;
using std::ifstream;
using std::vector;
using std::cerr;
using std::endl;
using std::cout;
using std::istringstream;


ifstream abrir_fichero(const string& nombreFichero) {
    ifstream fichero(nombreFichero);
    if (!fichero.is_open()) {
        cerr << "Error al abrir el fichero: " << nombreFichero << endl;
        exit(1); // Salir del programa si no se puede abrir el fichero
    }
    return fichero;
}


bool validar_formato(ifstream& fichero) {
    string linea;
    getline(fichero, linea);
    linea.pop_back();
    if (linea != "P3") {
        cerr << "Formato de fichero no soportado, se esperaba 'P3'" << endl;
        return false;
    }
    return true;
}


void leer_cabecera(ifstream& fichero, float& max, int& ancho, int& alto, float& c) {
    string linea;
    while (getline(fichero, linea)) {
        if (linea[0] == '#') {
            if (linea.find("#MAX=") != string::npos) {
                istringstream iss(linea.substr(5));
                iss >> max;
            }
            continue; // Saltar línea de comentario
        } else {
            leer_dimensiones(ancho, alto, linea);
            leer_resolucion(fichero, c);
            break;
        }
    }
}


void leer_dimensiones(int& ancho, int& alto, string& linea) {
    istringstream ss(linea);
    ss >> ancho >> alto;
}


float leer_resolucion(ifstream& fichero, float& c) {
    fichero >> c;
    return c;
}


vector<float> leer_valores(ifstream& fichero, float max, float c) {
    vector<float> valores; // Vector para almacenar los valores resultantes
    int valor;
    while (fichero >> valor) {
        valores.push_back(static_cast<float>(valor) * max / c);
    }
    return valores;
}


void imprimir_resultados(const vector<float>& valores, float max, int alto, int ancho, float c) {
    /*
    // DEBUG
    cout << "Valores resultantes:" << endl;
    for (const float& v : valores) {
        cout << v << " ";
    }
    cout << endl;
    */

    cout << "Max Value: " << max << endl;
    cout << "Alto: " << alto << ", Ancho: " << ancho << endl;
    cout << "Resolución máxima de color: " << c << endl;
}


int leer_pagina(const string& nombreFichero) {
    ifstream fichero = abrir_fichero(nombreFichero);

    if (!validar_formato(fichero)) {
        return 1;
    }

    float max = 1.0f;
    int ancho, alto;
    float c;

    leer_cabecera(fichero, max, ancho, alto, c);
    vector<float> valores = leer_valores(fichero, max, c);

    fichero.close();

    imprimir_resultados(valores, max, alto, ancho, c);

    return 0;
}
