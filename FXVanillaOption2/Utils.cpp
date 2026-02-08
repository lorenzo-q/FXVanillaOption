#include "Utils.h"
#include <cmath>

constexpr double INV_SQRT_2PI = 0.3989422804014327;    // 1/sqrt(2*pi)
constexpr double M_SQRT1_2 = 0.70710678118654752440;   // M_SQRT1_2 = 1/sqrt(2)
// ----- Utilities -----
double normalPDF(double x) {  
    return INV_SQRT_2PI * std::exp(-0.5 * x * x);
}

double normalCDF(double x) {
    // normal CDF (x) = 1/2 * (1 + erf(x/sqrt(2)))
    return 0.5 * (1.0 + std::erf(x * M_SQRT1_2));
}
