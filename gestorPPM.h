

#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using std::string;
using std::ifstream;
using std::vector;

// Función que itera por cada valor del RGB de un ppm y lo convierte al
// nuevo estándar en función de la resolución de color <c> y el valor
// máximo real como un número real <max>
ifstream abrir_fichero(const string& nombreFichero);

// Función para validar el formato del fichero
bool validar_formato(ifstream& fichero);

// Función para leer comentarios y extraer el max
void leer_cabecera(ifstream& fichero, float& max, int& ancho, int& alto, float& c);

// Función para leer dimensiones (ancho y alto)
void leer_dimensiones(int& ancho, int& alto, string& linea);

// Función para leer el valor máximo de color
float leer_resolucion(ifstream& fichero, float& c);

// Función para leer los valores de los píxeles
vector<float> leer_valores(ifstream& fichero, float max, float c);

// Función para imprimir los resultados
void imprimir_resultados(const vector<float>& valores, float max, int alto, int ancho, float c);

// Función principal que coordina el proceso
int leer_pagina(const string& nombreFichero);