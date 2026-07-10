#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "place.h"
#include "red-black-tree.h"
#include "trie.h"
#include "chrono"
#include "operations.h"

using namespace std;

void loadData(const string& filename, vector<Place>& data) {
	ifstream file(filename);

	if (!file.is_open()) {
		file.open("../" + filename);
	}

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


void displayPrefixResults(const string& input, const vector<Place>& results, long long timeUs) {
    cout << "\nPrefix: " << input << "\n" << endl;

    for (const auto& place : results) {
        cout << "  " << place.asciiName << ", " << place.country << endl;
    }

    cout << "\nLookup time: " << timeUs << " μs" << endl;

}

void displaySearchResults(const string& input, const vector<Place>& results, long long timeUs) {
    cout << "\nCity: " << input << "\n" << endl;

    if (results.empty()) {
        cout << "  No results found." << endl;
    }
    for (const auto& place : results) {
        cout << "  " << place.asciiName << ", " << place.country << endl;
        cout << "  pop " << place.population << "  (" << place.latitude << ", " << place.longitude << ")" << endl;
        cout << endl;
    }

    cout << "\n" << results.size() << " matches  |  " << timeUs << " μs" << endl;
}


long long benchMarkBuildTrie(const vector<Place>& data, Trie& trie) {
    auto start = chrono::steady_clock::now();
    for (const auto& place : data) {
        trie.insert(place);
    }
    auto stop = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::microseconds>(stop - start).count();
}

long long benchMarkBuildRBTree(const vector<Place>& data, RedBlackTree& tree) {
    auto start = chrono::steady_clock::now();
    for (const auto& place : data) {
        tree.insert(place);
    }
    auto stop = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::microseconds>(stop - start).count();
}

long long benchMarkSearchTrie(Trie& trie, vector<string>& inputs) {
    auto start = chrono::steady_clock::now();

    for (const auto& input : inputs) {
        trie.search(input);
    }

    auto stop = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(stop - start).count() / (long long)inputs.size();
}

long long benchMarkSearchRBTree(RedBlackTree& tree, vector<string>& inputs) {
    auto start = chrono::steady_clock::now();

    for (const auto& input : inputs) {
        tree.searchNode(input);
    }

    auto stop = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(stop - start).count() / (long long)inputs.size();
}

long long benchMarkPrefixSearchTrie(Trie& trie, vector<string>& inputs) {
    auto start = chrono::steady_clock::now();

    for (string input : inputs) {
        trie.autocomplete(input);
    }

    auto stop = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(stop - start).count() / (long long)inputs.size();
}

long long benchMarkPrefixSearchRBTree(RedBlackTree& tree, vector<string>& inputs) {
    auto start = chrono::steady_clock::now();

    for (string input : inputs) {
        tree.prefixSearch(input);
    }

    auto stop = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(stop - start).count() / (long long)inputs.size();
}
