#pragma once

struct MarketData {
    double spot;        // Spot FX rate S (domestic / foreign)
    double vol;         // volatility (annual). Assuming it is constant over time 
    double rd;          // domestic risk-free rate (annual, continuous). Assuming it is constant over time 
    double rf;          // foreign interest rate (annual, continuous). Assuming it is constant over time 

    MarketData(double spot_, double vol_, double rd_, double rf_);
};