#include "toneMapping.h"

using std::vector;

void clamp(vector<float>& valores){
    for(auto& valor : valores){
        if(valor > 1){
            valor = 1;
        }
    }
}

void equalize(vector<float>& valores, const float maxValue){
    for(auto& valor : valores){
        valor /= maxValue;
    }
}

void clampAndEqualize(vector<float>& valores){
    for(auto& valor : valores){
        
    }
}

void gamma(vector<float>& valores){
    for(auto& valor : valores){
        
    }
}

void gammaAndClamp(vector<float>& valores){
    for(auto& valor : valores){
        
    }
}
