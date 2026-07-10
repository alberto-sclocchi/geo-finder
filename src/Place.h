#pragma once
#include <string>
#include <cctype>

using namespace std;

// both structures cap prefix-search output at the same size so results match
const int PREFIX_RESULT_LIMIT = 5;

class Place {
public:
    string asciiName;
    double latitude;
    double longitude;
    string country;
    int population;
    Place() : asciiName(""), latitude(0.0), longitude(0.0), country(""), population(0) {}
    Place(string name, double lat, double lon, string cntry, int pop)
        : asciiName(name), latitude(lat), longitude(lon), country(cntry), population(pop) {}
};

