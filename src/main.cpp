#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "place.h"
#include "operations.cpp"
#include "red-black-tree.h"

int main() {
	vector<Place> allPlaces;
	RedBlackTree tree;

	loadData("cities.tsv", allPlaces);

	for (const auto& place : allPlaces) {
		tree.insert(place);
	}

	char choice;
	while (true) {
		cout << "======= PlaceFinder =======" << endl;
		cout << "Loaded " << allPlaces.size() << " places." << endl;
		cout << "[1] Prefix search" << endl;
		cout << "[2] Search place" << endl;
		cout << "[3] Benchmark" << endl;
		cout << "[Q] Quit" << endl;
		cout << "> ";
		cin >> choice;

		if (choice == 'Q' || choice == 'q') break;

		string input;

		switch(choice) {
			case '1':
				cout << "Enter prefix to search: ";
				cin >> input;
				break;
			case '2':
				cout << "Enter place name to search: ";
				cin >> input;
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