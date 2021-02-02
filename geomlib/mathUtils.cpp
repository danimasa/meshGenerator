#include "mathUtils.hpp"

std::pair<int, int> lessErrorApproximation(
    std::vector<std::pair<int, double>> values,
    APROX_DIRECTION direction
) {
    std::vector<std::pair<int, double>> errorValues;
    for(auto v : values) {
        int greater = v.first + 1;
        int lesser = v.first - 1;
        double gError = abs((v.second - greater) / greater);
        double lError = abs((v.second - lesser) / lesser);
        if(direction == APROX_DIRECTION::DOWN)
            errorValues.push_back({lesser, lError});
        else if (direction == APROX_DIRECTION::UP)
            errorValues.push_back({greater, gError});
        else {
            if(gError < lError)
                errorValues.push_back({greater, gError});
            else
                errorValues.push_back({lesser, lError});
        }
    }

    int index = 0, i = 0;
    int value = 0;
    double lError = 0.0;
    for(auto e : errorValues) {
        if(lError == 0.0 || e.second < lError) {
            lError = e.second;
            index = i;
            value = e.first;
        }
        
        i++;
    }

    return {index, value};
}
