#include <cmath>  // contains the definition of mathematical operators such as std::sqrt, std::exp etc. 
#include "Utils.h"  // contains the declaration for normalCDF and normalPDF
#include "FXOption.h"
// ----- Option description (European vanilla) -----

// ----- Pricer (Garman–Kohlhagen for FX) -----

FXOptionPricer::FXOptionPricer(Type type_, double strike_, double maturityYears_, std::string name_)
    : type{ type_ }, K{ strike_ }, T{ maturityYears_ }, name{ name_ } {
}

FXOptionPricer::~FXOptionPricer() = default;

FXOptionPricer::Result FXOptionPricer::price(const MarketData& mkt_) {   
    if (T <= 0.0) {
        // case where the option is expiring now, to avoid divide by zero errors
        double intrinsicValue = payoffAtExpiry(mkt_); // price output
        return { intrinsicValue, (type == FXOptionPricer::Call) ? ((mkt_.spot > K) ? 1.0 : 0.0)  // delta
                                                        : ((mkt_.spot < K) ? -1.0 : 0.0),
                0.0, 0.0, 0.0, 0.0, 0.0 };  // other risks
    }
    name = "Test";

    const double S = mkt_.spot;
    const double sigma = mkt_.vol;
    const double rd = mkt_.rd;
    const double rf = mkt_.rf;

    const double sqrtT = std::sqrt(T);
    const double sigmaSqrtT = sigma * sqrtT;

    const double d1 = (std::log(S / K) + (rd - rf + 0.5 * sigma * sigma) * T) / sigmaSqrtT;
    const double d2 = d1 - sigmaSqrtT;

    const double df_dom = std::exp(-rd * T); // domestic discount factor 
    const double df_for = std::exp(-rf * T); // foreign discount factor 

    double price = 0.0;

    // Price and Delta
    double delta = 0.0;
    if (type == FXOptionPricer::Call) {
        price = S * df_for * normalCDF(d1) - K * df_dom * normalCDF(d2);
        delta = df_for * normalCDF(d1);
    }
    else { // Put option
        price = K * df_dom * normalCDF(-d2) - S * df_for * normalCDF(-d1);
        delta = -df_for * normalCDF(-d1);
    }

    // Gamma and Vega
    const double pdf_d1 = normalPDF(d1);
    const double gamma = df_for * pdf_d1 / (S * sigmaSqrtT);
    const double vega = S * df_for * pdf_d1 * sqrtT;

    // Theta
    double theta = 0.0;
    if (type == FXOptionPricer::Call) {
        theta = -(S * df_for * pdf_d1 * sigma) / (2.0 * sqrtT)
            - rf * S * df_for * normalCDF(d1)
            + rd * K * df_dom * normalCDF(d2);
    }
    else {  // Put option
        theta = -(S * df_for * pdf_d1 * sigma) / (2.0 * sqrtT)
            + rf * S * df_for * normalCDF(-d1)
            - rd * K * df_dom * normalCDF(-d2);
    }

    return { price, delta, gamma, vega, theta};
}

double FXOptionPricer::payoffAtExpiry(const MarketData& mkt_) const {
    if (type == FXOptionPricer::Call) {
        return std::max(mkt_.spot - K, 0.0);
    }
    else {
        return std::max(K - mkt_.spot, 0.0);
    }
}
