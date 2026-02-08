#pragma once
#include <string>
#include "MarketData.h" // required because we use MarketData

class FXOptionPricer {
public:
    enum Type { Call, Put };
    struct Result {     // used to return price and risks
        double price;
        double delta;
        double gamma;
        double vega;
        double theta;   // per year
        double rho_dom; // sensitivity to domestic rate rd
        double rho_for; // sensitivity to foreign rate rf
    };

    Result price(const MarketData& mkt_);

    FXOptionPricer(Type type_, double strike_, double maturityYears_, std::string name_ = "");
    virtual ~FXOptionPricer();

private:


    Type type;
    double K;    // strike
    double T;    // time to expiry in years
    std::string name;

    double payoffAtExpiry(const MarketData& mkt_) const;
};
