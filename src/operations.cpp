#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "place.h"
#include "red-black-tree.h"
#include "trie.h"

using namespace std;

void loadData(const string& filename, vector<Place>& data) {
	ifstream file(filename);
	string line;
	if (!file.is_open()) {
		cout << "Error: Could not open " << filename << endl;
		return;
	}
	getline(file, line); // skip header row
	// cities.tsv columns (tab-separated): asciiName, latitude, longitude, country, population
	while (getline(file, line)) {
		stringstream ss(line);
		string name, lat, lon, country, pop;
		getline(ss, name, '\t');
		getline(ss, lat, '\t');
		getline(ss, lon, '\t');
		getline(ss, country, '\t');
		getline(ss, pop, '\t');
		if (!name.empty() && !lat.empty() && !lon.empty()) {
			data.emplace_back(name, stod(lat), stod(lon), country, pop.empty() ? 0 : stoi(pop));

		}
	}
	file.close();
}
