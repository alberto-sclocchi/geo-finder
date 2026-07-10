#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <iomanip>
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
		getline(cin, choice);

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
			vector<int> sizes = {10000, 25000, 50000, 100000, (int)allPlaces.size()};
			vector<string> queries = {
				"Gainesville",           
				"New York City",         
				"Sint-Martens-Bodegem",  
				"23 August",            
				"Aci Trezza",            
				"Acitrezza",             
				"August",                
				"Newark",
				"Gainesboro",
				"Sint-Gillis-Waas",
				"gainesville",           
				"Atlantis",              
				"Zzzzz",                 
			};

			vector<string> prefixes = {
				"A",        
				"Sa",      
				"Sint",     
				"Sart",     
				"Gaines",   
				"New",     
				"Lond",     
				"Tal",     
				"Villa",       
				"Do",   
			};

			vector<long long> trie_results;
			vector<long long> tree_results;

			for (int n : sizes) {
				vector<Place> subset(allPlaces.begin(), allPlaces.begin() + n);

				Trie t;
				RedBlackTree r;

				trie_results.push_back(benchMarkBuildTrie(subset, t));
				tree_results.push_back(benchMarkBuildRBTree(subset, r));

				trie_results.push_back(benchMarkSearchTrie(t, queries));
				tree_results.push_back(benchMarkSearchRBTree(r, queries));

				trie_results.push_back(benchMarkPrefixSearchTrie(t, prefixes));
				tree_results.push_back(benchMarkPrefixSearchRBTree(r, prefixes));
			}


			// Display benchmark results
			cout << "\n====================== Benchmark Results ======================\n" << endl;

			cout << "\n"
				<< left  << setw(15) << "Rows"
				<< right << setw(20) << "Trie build (μs)"
				<< setw(20) << "RB build (μs)"
				<< setw(20) << "Trie search (ns)"
				<< setw(20) << "RB search (ns)"
				<< setw(20) << "Trie prefix (ns)"
				<< setw(20) << "RB prefix (ns)" << endl;
			cout << string(130, '-') << endl;

			for (int i = 0; i < (int)sizes.size(); i++){
				cout<< left  << setw(15) << sizes[i]
					<< right << setw(17) << trie_results[i * 3]
					<< setw(17) << tree_results[i * 3]
					<< setw(17) << trie_results[i * 3 + 1]
					<< setw(20) << tree_results[i * 3 + 1]
					<< setw(20) << trie_results[i * 3 + 2]
					<< setw(20) << tree_results[i * 3 + 2] << endl;
			}

		} else {
			cout << "\nInvalid choice, please try again." << endl;
		}
	}
	return 0;
}