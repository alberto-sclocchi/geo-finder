#pragma once
#include <string>

using namespace std;

class Place {
public:
    string asciiName;
    double latitude;
    double longitude;
    string country;
    int population;

    Place(string name, double lat, double lon, string cntry, int pop)
        : asciiName(name), latitude(lat), longitude(lon), country(cntry), population(pop) {}
};

