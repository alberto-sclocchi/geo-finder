#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>
#include "place.h"
#include "trie.h"
#include "red-black-tree.h"

using namespace std;

// Small synthetic dataset covering the edge cases found in cities.tsv:
// duplicates, case variants, punctuation collisions, digits in names.
static vector<Place> testPlaces() {
	return {
		Place("Gainesville", 29.65, -82.32, "United States", 141085),
		Place("Gainesville", 34.29, -83.82, "United States", 42296),
		Place("Aci Trezza", 37.56, 15.16, "Italy", 4913),
		Place("Acitrezza", 37.57, 15.17, "Italy", 100),      // collides with "Aci Trezza" in the trie
		Place("23 August", 43.92, 28.58, "Romania", 5483),   // digits are skipped by the trie
		Place("August", 37.98, -121.26, "United States", 8771),
		Place("New York City", 40.71, -74.01, "United States", 8804190),
		Place("Newark", 40.74, -74.17, "United States", 311549),
		Place("Newberry", 29.65, -82.61, "United States", 7342),
		Place("Newcastle", 54.98, -1.61, "United Kingdom", 192382),
		Place("Newport", 51.59, -3.00, "United Kingdom", 145700),
		Place("Newton", 42.34, -71.21, "United States", 88923),
		Place("Sint-Martens-Bodegem", 50.86, 4.21, "Belgium", 2715),
	};
}

static vector<string> names(const vector<Place>& places) {
	vector<string> out;
	for (const auto& p : places) out.push_back(p.asciiName);
	return out;
}

TEST_CASE("exact search finds the right city", "[search]") {
	Trie trie;
	RedBlackTree tree;
	for (const auto& p : testPlaces()) { trie.insert(p); tree.insert(p); }

	SECTION("exact match returns the queried name, not a collision") {
		// "23 August" and "August" share a trie node once digits are skipped
		REQUIRE(names(trie.search("23 August")) == vector<string>{"23 August"});
		REQUIRE(names(trie.search("August")) == vector<string>{"August"});
		REQUIRE(names(tree.searchNode("23 August")) == vector<string>{"23 August"});
		REQUIRE(names(tree.searchNode("August")) == vector<string>{"August"});

		// "Aci Trezza" and "Acitrezza" also collide in the trie
		REQUIRE(names(trie.search("Aci Trezza")) == vector<string>{"Aci Trezza"});
		REQUIRE(names(trie.search("Acitrezza")) == vector<string>{"Acitrezza"});
		REQUIRE(names(tree.searchNode("Aci Trezza")) == vector<string>{"Aci Trezza"});
	}

	SECTION("search is case-insensitive in both structures") {
		REQUIRE_FALSE(trie.search("gainesville").empty());
		REQUIRE_FALSE(tree.searchNode("gainesville").empty());
		REQUIRE_FALSE(tree.searchNode("GAINESVILLE").empty());
		REQUIRE(names(tree.searchNode("gainesville")) == names(trie.search("Gainesville")));
	}

	SECTION("duplicate city names return all records") {
		vector<Place> t = trie.search("Gainesville");
		vector<Place> r = tree.searchNode("Gainesville");
		REQUIRE(t.size() == 2);
		REQUIRE(r.size() == 2);
		REQUIRE(names(t) == names(r));
	}

	SECTION("missing names return empty results in both") {
		REQUIRE(trie.search("Atlantis").empty());
		REQUIRE(tree.searchNode("Atlantis").empty());
		REQUIRE(trie.search("").empty());
		REQUIRE(tree.searchNode("").empty());
		REQUIRE(trie.search("123!!!").empty());
		REQUIRE(tree.searchNode("123!!!").empty());
		// prefix of a real name is not an exact match
		REQUIRE(trie.search("Gaines").empty());
		REQUIRE(tree.searchNode("Gaines").empty());
	}
}

TEST_CASE("prefix search returns identical results from both structures", "[prefix]") {
	Trie trie;
	RedBlackTree tree;
	for (const auto& p : testPlaces()) { trie.insert(p); tree.insert(p); }

	SECTION("same names, same order") {
		vector<Place> t = trie.autocomplete("New");
		vector<Place> r = tree.prefixSearch("New");
		REQUIRE(names(t) == names(r));
		// "New York City" sorts before "Newark" (space < letters)
		REQUIRE(t[0].asciiName == "New York City");
	}

	SECTION("results are capped at the same limit") {
		// insert more Springfields than the limit; both structures must cap
		for (int i = 0; i < PREFIX_RESULT_LIMIT + 3; i++) {
			Place p("Springfield", i, -i, "United States", 1000 + i);
			trie.insert(p);
			tree.insert(p);
		}
		REQUIRE(trie.autocomplete("Spring").size() == (size_t)PREFIX_RESULT_LIMIT);
		REQUIRE(tree.prefixSearch("Spring").size() == (size_t)PREFIX_RESULT_LIMIT);
		REQUIRE(names(trie.autocomplete("Spring")) == names(tree.prefixSearch("Spring")));
	}

	SECTION("prefix search is case-insensitive in both structures") {
		REQUIRE(names(trie.autocomplete("new")) == names(tree.prefixSearch("new")));
		REQUIRE_FALSE(tree.prefixSearch("new").empty());
		REQUIRE(names(tree.prefixSearch("NEW")) == names(tree.prefixSearch("new")));
	}

	SECTION("punctuation in prefix is handled consistently") {
		vector<Place> t = trie.autocomplete("Sint-Mar");
		vector<Place> r = tree.prefixSearch("Sint-Mar");
		REQUIRE(names(t) == vector<string>{"Sint-Martens-Bodegem"});
		REQUIRE(names(t) == names(r));

		// a squashed collision must not leak into prefix results:
		// the trie subtree for "au" contains "23 August", which does not start with "au"
		REQUIRE(names(trie.autocomplete("Au")) == vector<string>{"August"});
		REQUIRE(names(tree.prefixSearch("Au")) == vector<string>{"August"});

		// and a digit prefix still finds the digit-named city
		REQUIRE(names(trie.autocomplete("23")) == vector<string>{"23 August"});
		REQUIRE(names(tree.prefixSearch("23")) == vector<string>{"23 August"});
	}

	SECTION("empty and unmatched prefixes return nothing in both") {
		REQUIRE(trie.autocomplete("").empty());
		REQUIRE(tree.prefixSearch("").empty());
		REQUIRE(trie.autocomplete("Zzz").empty());
		REQUIRE(tree.prefixSearch("Zzz").empty());
	}

	SECTION("duplicates appear in prefix results from both") {
		vector<Place> t = trie.autocomplete("Gaines");
		vector<Place> r = tree.prefixSearch("Gaines");
		REQUIRE(t.size() == 2);
		REQUIRE(names(t) == names(r));
	}
}

TEST_CASE("red-black tree stays consistent under many inserts", "[rbtree]") {
	// inserting sorted input is the classic BST worst case; a working
	// red-black tree must still answer correctly
	RedBlackTree tree;
	Trie trie;
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			string name = string(1, char('A' + i)) + string(1, char('a' + j)) + "ville";
			Place p(name, 0.0, 0.0, "Testland", i * 26 + j);
			tree.insert(p);
			trie.insert(p);
		}
	}
	REQUIRE(names(tree.searchNode("Mmville")) == vector<string>{"Mmville"});
	REQUIRE(names(tree.searchNode("Aaville")) == vector<string>{"Aaville"});
	REQUIRE(names(tree.searchNode("Zzville")) == vector<string>{"Zzville"});
	REQUIRE(tree.searchNode("Zzzville").empty());
	REQUIRE(names(tree.prefixSearch("Mm")) == names(trie.autocomplete("Mm")));
	REQUIRE(tree.prefixSearch("M").size() == (size_t)PREFIX_RESULT_LIMIT);
}
