#include "toneMapping.h"
#include <cmath>

using std::vector;
using std::pow;

// GAMMA puede ser otro, pero 2.2 es un valor común 
// utilizado para la corrección gamma en monitores estándar.
#define GAMMA 2.2


void clamp(vector<float>& valores){
    clampAndEqualize(valores, 1);   // Igual que la función clamp+equalize si threshold = 1
}


void equalize(vector<float>& valores, const float maxValue){
    clampAndEqualize(valores, maxValue);    // Igual que la función clamp+equalize si todos 
                                            // los valores caen por debajo del threshold
                                            // (threshold = máximo valor)
}

void clampAndEqualize(vector<float>& valores, const float threshold){
    for(int i = 0; i < valores.size(); i++){
        if(valores[i] <= threshold){
            valores[i] /= threshold; // Clamping
        } else {
            valores[i] = 1; // Equalization
        }
    }
}

void gamma(vector<float>& valores, const float maxValue){
    gammaAndClamp(valores, maxValue);   // Igual que la función gamma+clamp si todos 
                                        // los valores caen por debajo del threshold
                                        // (threshold = máximo valor)
}

void gammaAndClamp(vector<float>& valores, const float threshold){
    int size = valores.size();
    for(int i = 0; i < size; i++){
        if(valores[i] <= threshold){
            valores[i] /= threshold; // Ecualization
            valores[i] = pow(valores[i], GAMMA); // Gamma
        } else {
            valores[i] = 1; // Clamping
        }
    }
}
