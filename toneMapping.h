#include <vector>

using std::vector;


void clamp(vector<float>& valores);

void equalize(vector<float>& valores, const float maxValue);

void clampAndEqualize(vector<float>& valores);

void gamma(vector<float>& valores);

void gammaAndClamp(vector<float>& valores);
