#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Place.h"

using namespace std;

void loadData(const string& filename, vector<Place>& data) {
	ifstream file(filename);
	string line;
	if (!file.is_open()) {
		cout << "Error: Could not open " << filename << endl;
		return;
	}
	getline(file, line);
	while (getline(file, line)) {
		stringstream ss(line);
		string name, lat, lon, country, pop;
		getline(ss, name, ',');
		getline(ss, lat, ',');
		getline(ss, lon, ',');
		getline(ss, country, ',');
		getline(ss, pop, ',');
		if (!name.empty() && !lat.empty() && !lon.empty()) {
			data.emplace_back(name, stod(lat), stod(lon), country, stoi(pop));
		}
	}
	file.close();
}

int main() {
	vector<Place> allPlaces;
	loadData("geonames.csv", allPlaces);

	char choice;
	while (true) {
		cout << "=== PlaceFinder ===" << endl;
		cout << "Loaded " << allPlaces.size() << " places." << endl;
		cout << "[1] Prefix search" << endl;
		cout << "[2] Search place" << endl;
		cout << "[3] Benchmark" << endl;
		cout << "[Q] Quit" << endl;
		cout << "> ";
		cin >> choice;

		if (choice == 'Q' || choice == 'q') break;

		switch(choice) {
			case '1':
				cout << "Prefix search functionality pending Trie/RB-Tree integration." << endl;
				break;
			case '2':
				cout << "Search place functionality pending Trie/RB-Tree integration." << endl;
				break;
			case '3':
				cout << "Benchmark functionality pending Trie/RB-Tree integration." << endl;
				break;
			default:
				cout << "Invalid choice, please try again." << endl;
		}
	}
	return 0;
}