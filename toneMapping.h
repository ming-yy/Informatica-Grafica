#include <vector>

using std::vector;



void clamp(vector<float>& valores);

void equalize(vector<float>& valores, const float maxValue);

void clampAndEqualize(vector<float>& valores, const float threshold);

void gamma(vector<float>& valores, const float maxValue);

void gammaAndClamp(vector<float>& valores, const float threshold);
