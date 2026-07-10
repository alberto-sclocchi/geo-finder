#pragma once
#include <string>
#include <cctype>

using namespace std;

const int PREFIX_RESULT_LIMIT = 10;

inline string toLowerName(const string& s) {
    string out = s;
    for (char& c : out) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return out;
}

class Place {
public:
    string asciiName;
    string lowerName; 
    double latitude;
    double longitude;
    string country;
    int population;
    Place() : asciiName(""), lowerName(""), latitude(0.0), longitude(0.0), country(""), population(0) {}
    Place(string name, double lat, double lon, string cntry, int pop)
        : asciiName(name), lowerName(toLowerName(name)), latitude(lat), longitude(lon), country(cntry), population(pop) {}
};

