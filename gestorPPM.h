

#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using std::string;
using std::ifstream;
using std::vector;

// Función que extrae los metadatos y los valores del fichero
bool leerFicheroPPM(const string&nombreFich, vector<float>& valores, 
                            float& max, int& ancho, int& alto, float& c);

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
void leer_valores(ifstream& fichero, float max, float c, vector<float>& valores);

// Función para imprimir los resultados
void imprimir_resultados(const vector<float>& valores, float max, int alto, int ancho, float c);

// Función principal que coordina el proceso
int transformarFicheroPPM(const string& nombreFichero);

// Recorta la ruta "nnn/mmm/fichero.ppm" para quedarse solo con fichero.ppm
string encontrarNombreFinalFichero(const string&ruta);

// Escribe los metadatos del fichero PPM resultado
void escribirCabeceraPPM(ofstream& fichero, const string nombreFichero, 
                                const float max, const int ancho, const int alto, const float c);

// Escribe los valores de los pixeles (r,g,b) en el fichero PPM resultado
void escribirValoresPPM(ofstream& fichero, const vector<float>& valores, 
                                 const float max, const int alto, const int ancho, const float c);

// Escribe todo el contenido del fichero PPM resultado
void escribirFicheroPPM(const string&nombreFich, const vector<float>& valores, 
                        const float max, const int ancho, const int alto, const float c);
