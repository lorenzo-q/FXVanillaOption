#include <iostream>
#include <iomanip>  
#include <array>
#include "MarketData.h"
#include "FXOption.h"

// Helper function to print test results
void reportTest(const int& testNum, bool passed) {
    std::cout << "Test " << testNum << (passed ? "[PASS]" : "[FAIL]") << "\n";
}

int main() {
    // Example market data:
    // Spot = 1.15 domestic/foreign (e.g. USD per EUR)
    // vol = 12% 
    // rd = 4% domestic rate, rf = 2% foreign rate
    std::cout << std::fixed << std::setprecision(6);

    double spot = 1.15;
    double vol = 0.12;
    double rd = 0.04;
    double rf = 0.02;
    MarketData mkt(spot, vol, rd, rf);
    double T = 0.5;
    std::array<double, 5> Ks = { 0.9, 1, 1.15, 1.30, 1.40 }; // strikes used in tests
    std::string nameCall = "EURUSD_6m_Call";
    std::string namePut = "EURUSD_6m_Put";
    double thres = std::pow(10,-6); // error threshold for tests
    bool testPassed = true;
    bool allTestsPassed = true;

    // Call-Put Parity
    std::cout << "----- Test 1: Call - Put parity -----" << "\n";
    double parity = 0;
    for (const double& k : Ks) {
        FXOptionPricer call(FXOptionPricer::Call, k, T, nameCall);
        FXOptionPricer put(FXOptionPricer::Put, k, T, namePut);
        parity = call.price(mkt).price - put.price(mkt).price - (spot * std::exp(-rf * T) - k * std::exp(-rd * T)); // Call-Put parity relation for FX
        std::cout << "Call-Put parity for strike " << k << " = " << parity << "\n";
        testPassed = testPassed * (std::abs(parity) < thres);
    }
    reportTest(1, testPassed);
    allTestsPassed = allTestsPassed * testPassed; 

    // Delta of a call option minus the Delta of a put option should equal the discount factor
    std::cout << "----- Test 2: Delta Parity -----" << "\n";
    testPassed = true;
    double deltaParity = 0;
    for (double k : Ks) {
        FXOptionPricer call(FXOptionPricer::Call, k, T, nameCall);
        FXOptionPricer put(FXOptionPricer::Put, k, T, nameCall);
        deltaParity = (call.price(mkt).delta - put.price(mkt).delta);
        std::cout << "Call Delta minus Put Delta for strike " << k << " = " << deltaParity << "\n";
        testPassed = testPassed * ( std::abs(deltaParity - std::exp(-rf * T)) < thres );
    }
    reportTest(2, testPassed);
    allTestsPassed = allTestsPassed * testPassed;


    if (allTestsPassed) {
        std::cout << "All tests passed!" << "\n";
    }

    /*
    FXOptionPricer fxOption(FXOptionPricer::Call, k, T, nameCall);
    std::cout << "Option: " << fxOption.name << "\n";
    std::cout << "Spot S = " << mkt.spot << ", Strike K = " << fxOption.K << ", T = " << fxOption.T << " years\n";
    std::cout << "Vol = " << mkt.vol << ", rd = " << mkt.rd << ", rf = " << mkt.rf << "\n\n";

    std::cout << "Price = " << r.price << " (in domestic currency per option unit)\n";
    std::cout << "Delta = " << r.delta << "\n";
    std::cout << "Gamma = " << r.gamma << "\n";
    std::cout << "Vega  = " << r.vega << " (per absolute vol. unit)\n";
    std::cout << "Theta = " << r.theta << " (per year)\n";
    std::cout << "Rho (domestic) = " << r.rho_dom << "\n";
    std::cout << "Rho (foreign)  = " << r.rho_for << "\n";
    */
    return 0;
}