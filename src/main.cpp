#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include "place.h"
#include "operations.h"
#include "red-black-tree.h"
#include "trie.h"

int main() {
	vector<Place> allPlaces;
	RedBlackTree tree;
	Trie trie;

	loadData("cities.tsv", allPlaces);

	for (const auto& place : allPlaces) {
		tree.insert(place);
		trie.insert(place);
	}

	cout << "\n====================== PlaceFinder ======================\n" <<endl;
	cout << "Loaded " << allPlaces.size() << " places." << endl;

	string choice;
	while (true) {
		cout<< endl;
		cout<<"========= Menu =========" << endl;
		cout << "[1] Prefix search" << endl;
		cout << "[2] Search place" << endl;
		cout << "[3] Benchmark" << endl;
		cout << "[Q] Quit" << endl;
		cout<< endl;
		cout << "> ";

		if (!getline(cin, choice)) break; // input ended: quit instead of looping

		if (choice == "Q" || choice == "q") break;

		string input;
		string dsa_type;

		if (choice == "1") {
			cout << "\nEnter prefix to search: ";
			getline(cin, input);

			cout << "\nSelect data structure (1: Trie, 2: Red-Black Tree): ";
			getline(cin, dsa_type);

			if (dsa_type == "1" || dsa_type == "2") {

				auto start = chrono::steady_clock::now();

				vector<Place> results = (dsa_type == "1") ? trie.autocomplete(input) : tree.prefixSearch(input);

				auto stop = chrono::steady_clock::now();

				long long timeUs = chrono::duration_cast<chrono::microseconds>(stop - start).count();

				displayPrefixResults(input, results, timeUs);

			} else {
				cout << "\nInvalid choice, please try again." << endl;
			}

		} else if (choice == "2") {
			cout << "\nEnter place name to search: ";
			getline(cin, input);

			cout << "\nSelect data structure (1: Trie, 2: Red-Black Tree): ";
			getline(cin, dsa_type);

			if (dsa_type == "1" || dsa_type == "2") {

				auto start = chrono::steady_clock::now();

				vector<Place> results = (dsa_type == "1") ? trie.search(input) : tree.searchNode(input);

				auto stop = chrono::steady_clock::now();

				long long timeUs = chrono::duration_cast<chrono::microseconds>(stop - start).count();

				displaySearchResults(input, results, timeUs);

			} else {
				cout << "\nInvalid choice, please try again." << endl;
			}
		} else if (choice == "3") {
			cout << "\nBenchmark functionality pending Trie/RB-Tree integration." << endl;
		} else {
			cout << "\nInvalid choice, please try again." << endl;
		}
	}
	return 0;
}