#include "toneMapping.h"

using std::vector;



void clamp(vector<float>& valores){
    ifBiggerThenOne(valores, 1);
}


void equalize(vector<float>& valores, const float maxValue){
    ifSmallerThenNormalize(valores, maxValue);
}

void clampAndEqualize(vector<float>& valores, const float maxValue){
    float newValue = maxValue / 2;
    ifSmallerThenNormalize(valores, newValue);
    ifBiggerThenOne(valores, newValue);
}

void gamma(vector<float>& valores, const float maxValue){
    ifSmallerThenCurve(valores, maxValue);
}

void gammaAndClamp(vector<float>& valores, const float maxValue){
    float newValue = maxValue / 2;
    ifSmallerThenCurve(valores, newValue);
    ifBiggerThenOne(valores, newValue);
}

void ifBiggerThenOne(vector<float>& valores, const float threshold){
    for(int i = 0; i < valores.size(); i++){
        if(valores[i] > threshold){
            valores[i] = 1;
        }
    }
}

void ifSmallerThenNormalize(vector<float>& valores, const float threshold){
    for(int i = 0; i < valores.size(); i++){
        if(valores[i] <= threshold){
            valores[i] /= threshold;
        }
    }
}

void ifSmallerThenCurve(vector<float>& valores, const float threshold){
    for(int i = 0; i < valores.size(); i++){
        if(valores[i] <= threshold){
            // FALTA AÑADIR FUNCION LOGARITMICA? IDK
        }
    }
}
