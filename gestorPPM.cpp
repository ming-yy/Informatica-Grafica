
#include "gestorPPM.h"
#include <iomanip>

using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::cerr;
using std::endl;
using std::cout;
using std::istringstream;
using std::fixed;
using std::setprecision;

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
    
    if (linea != "P3") {
        cerr << "Formato de fichero no soportado, se esperaba 'P3', no " << linea << endl;
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


void leer_valores(ifstream& fichero, float max, float c, vector<float>& valores) {
    int valor;
    while (fichero >> valor) {
        valores.push_back(static_cast<float>(valor) * max / c);
        //valores.push_back(static_cast<float>(valor)); // Para generar la imagen de nuevo, inalterada (pruebas)

    }
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
    cout << "Resolucion maxima de color: " << c << endl;
}

string encontrarNombreFinalFichero(const string&ruta){
     // Encontrar la posición del último '/'
    size_t pos = ruta.find_last_of("/");

    // Si encontró el '/' obtener la parte posterior
    if (pos != std::string::npos) {
        return ruta.substr(pos + 1); // Obtener el substring después del '/'
    } else {
        return ruta; // Si no hay '/', devolver el string completo
    }
}


void escribirCabeceraPPM(ofstream& fichero, const float max, const string nombreFichero, const int ancho, const int alto, const float c){
    fichero << "P3" << "\n";
    fichero << "#MAX=" << fixed << setprecision(0) << max << "\n";
    fichero << "# " << nombreFichero << "\n";
    fichero << ancho << " " << alto << "\n";
    fichero  << fixed << setprecision(0) << c << "\n";
}

void escribirValoresPPM(ofstream& fichero, const vector<float>& valores, const int alto, const int ancho){

    int anchoTriple = ancho*3;

    if(valores.size() != anchoTriple * alto){
        cerr << "ERROR: El tamano (ancho x alto) del fichero de entrada es incorrecto" << endl;
        return;
    }

            
    int indice = 0;
    // Imprimir la matriz en filas y columnas
    for (int h = 0; h < alto; ++h) {
        for (int w = 0; w < anchoTriple; w+=3) {
            indice = h*anchoTriple + w;
            fichero << valores.at(indice) << " " 
                    << valores.at(indice + 1) << " "
                    << valores.at(indice + 2) << "     ";
        }
        fichero << "\n";
    }
    
}



void escribirFicheroPPM(const string&nombreFich, const vector<float>& valores, const float max, const int ancho, const int alto, const float c){
    string rutaFichRes = nombreFich.substr(0, nombreFich.find_last_of(".")) + "_altered.ppm";
    string nombreFichRes = encontrarNombreFinalFichero(nombreFich);

    ofstream ficheroRes(rutaFichRes);
    if(!ficheroRes){
        cerr << "ERROR al crear el archivo de salida." << endl;
        return;
    }

    
    escribirCabeceraPPM(ficheroRes, max, nombreFichRes, ancho, alto, c);
    escribirValoresPPM(ficheroRes, valores, ancho, alto);
}



int tratarFicheroPPM(const string& nombreFichero) {
    ifstream fichero = abrir_fichero(nombreFichero);

    if (!validar_formato(fichero)) {
        return 1;
    }

    float max = 1.0f;
    int ancho, alto;
    float c;

    leer_cabecera(fichero, max, ancho, alto, c);
    vector<float> valores;
    leer_valores(fichero, max, c, valores);
    fichero.close();

    //imprimir_resultados(valores, max, alto, ancho, c);

    escribirFicheroPPM(nombreFichero, valores, max, ancho, alto, c);


    return 0;
}