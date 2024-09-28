
#include "gestorPPM.h"
#include "toneMapping.h"
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


bool leerFicheroPPM(const string&nombreFich, vector<float>& valores, 
                            float& maxColorRes, int& ancho, int& alto, float& c) {
    
    ifstream fichero = abrir_fichero(nombreFich);
    if (!validar_formato(fichero)) {
        return false;
    }

    leer_cabecera(fichero, maxColorRes, ancho, alto, c);
    leer_valores(fichero, maxColorRes, c, valores);
    fichero.close();

    //imprimir_resultados(valores, maxColorRes, alto, ancho, c);

    return true;
}


ifstream abrir_fichero(const string& nombreFich) {
    ifstream fichero(nombreFich);
    if (!fichero.is_open()) {
        cerr << "Error al abrir el fichero: " << nombreFich << endl;
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


void leer_cabecera(ifstream& fichero, float& maxColorRes, int& ancho, int& alto, float& c) {
    string linea;
    while (getline(fichero, linea)) {
        if (linea[0] == '#') {
            if (linea.find("#MAX=") != string::npos) {
                istringstream iss(linea.substr(5));
                iss >> maxColorRes;
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


void leer_valores(ifstream& fichero, float maxColorRes, float c, vector<float>& valores) {
    float valor;
    while (fichero >> valor) {
        valores.push_back((valor * maxColorRes) / c);
        //valores.push_back(static_cast<float>(valor)); // Para generar la imagen de nuevo, inalterada (pruebas)
    }
}


void imprimir_resultados(const vector<float>& valores, float maxColorRes, int alto, int ancho, float c) {
    /*
    // DEBUG
    cout << "Valores resultantes:" << endl;
    for (const float& v : valores) {
        cout << v << " ";
    }
    cout << endl;
    */

    cout << "Max Value: " << maxColorRes << endl;
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


void escribirCabeceraPPM(ofstream& fichero, const string nombreFichero, 
        const float maxColorRes, const int ancho, const int alto, const float c){
    fichero << "P3" << "\n";
    fichero << "#MAX=" << fixed << setprecision(0) << maxColorRes << "\n";
    fichero << "# " << nombreFichero << "\n";
    fichero << ancho << " " << alto << "\n";
    fichero  << fixed << setprecision(0) << c << "\n";
}

void escribirValoresPPM(ofstream& fichero, const vector<float>& valores, 
                                const float maxColorRes, const int ancho, const int alto, const float c){

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
            fichero << ((valores.at(indice)*c)/maxColorRes) << " " 
                    << ((valores.at(indice + 1)*c)/maxColorRes) << " "
                    << ((valores.at(indice + 2)*c)/maxColorRes)<< "     ";
        }
        fichero << "\n";
    }
    
}



void escribirFicheroPPM(const string&nombreFich, const vector<float>& valores, 
                            const float maxColorRes, const int ancho, const int alto, 
                                const float c, const string& nombreFuncion){
    
    string rutaFichRes = nombreFich.substr(0, nombreFich.find_last_of(".")) + "_" + nombreFuncion + ".ppm";
    string nombreFichRes = encontrarNombreFinalFichero(nombreFich);

    ofstream ficheroRes(rutaFichRes);
    if(!ficheroRes){
        cerr << "ERROR al crear el archivo de salida." << endl;
        return;
    }

    
    escribirCabeceraPPM(ficheroRes, nombreFichRes, maxColorRes, ancho, alto, c);
    escribirValoresPPM(ficheroRes, valores, maxColorRes, ancho, alto, c);

    ficheroRes.close();
}

string transformarValores(vector<float>& valores, const int tipoTransform, const float maxValue){
    string res = "";
    switch (tipoTransform)
    {
    case 0:
        // NINGUNA FUNCION
        // (misma imagen de entrada y salida)
        res = "0_Ninguna";
        break;

    case 1:
        // FUNCION CLAMPING
        res = "1_Clamping";
        clamp(valores);
        break;

    case 2:
        // FUNCION EQUALIZATION
        res = "2_Equalization";
        equalize(valores, maxValue);
        break;

    case 3:
        // FUNCION CLAMPING+EQUALIZATION
        res = "3_Clamping+Equalization";
        clampAndEqualize(valores, maxValue/2);
        break;

    case 4:
        // FUNCION GAMMA
        res = "4_Gamma";
        gamma(valores, maxValue);
        break;

    case 5:
        // FUNCION GAMMA+EQUALIZATION
        res = "5_Gamma+Equalization";
        gammaAndClamp(valores, maxValue/2);
        break;

    default:
        // ERROR
        res = "ERROR";
        break;
    }

    return res;
}




int transformarFicheroPPM(const string& nombreFichero, const int idFuncion) {
    vector<float> valores;

    float maxColorRes = 1.0f;
    int ancho, alto;
    float c;
    string nombreFuncion = "";
    
    cout << "FICHERO \"" << nombreFichero << "\"" << endl;
    cout << "Aplicando la funcion de tone mapping numero " << idFuncion << endl;

    if(leerFicheroPPM(nombreFichero, valores, maxColorRes, ancho, alto, c)){

        nombreFuncion = transformarValores(valores, idFuncion, maxColorRes);
        escribirFicheroPPM(nombreFichero, valores, maxColorRes, ancho, alto, c, nombreFuncion);

        cout << nombreFuncion << " ha terminado!"<< endl << endl;

    } else {
        cerr << "ERROR: Ha habido un problema en la lectura del Fichero PPM" << endl;
    }

    return 0;
}