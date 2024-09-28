#include <vector>

using std::vector;


void clamp(vector<float>& valores);

void equalize(vector<float>& valores, const float maxValue);

void clampAndEqualize(vector<float>& valores, const float maxValue);

void gamma(vector<float>& valores, const float maxValue);

void gammaAndClamp(vector<float>& valores, const float maxValue);

void ifBiggerThenOne(vector<float>& valores, const float threshold);

void ifSmallerThenNormalize(vector<float>& valores, const float threshold);

void ifSmallerThenCurve(vector<float>& valores, const float threshold);