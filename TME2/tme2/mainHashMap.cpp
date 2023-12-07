#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include "HashMap.hpp"


int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("./WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;
	
	HashMap<string, int> h(100);
	std::vector<std::pair<std::string, int>> v;
	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		
		if (nombre_lu % 100 == 0) {
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": " << word << endl;
		}
		nombre_lu++;

		// Vérifier si le mot existe déjà et le mettre à jour
		int* count = h.get(word);
		if (count) {
			// Le mot existe déjà, mise à jour de son compteur
			(*count)++;
		} else {
			// Nouveau mot, initialisation à 1
			h.put(word, 1);
		}
	}

	for (auto it = h.begin(); it != h.end(); ++it) {
		v.push_back(std::make_pair((*it).key, (*it).value));
	}

	// Trier par nombre d'occurrences décroissantes
	std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
		return a.second > b.second; 
	});

	// Afficher les 10 mots les plus fréquents
	cout << "Les 10 mots les plus fréquents : " << endl;
	for (int i = 0; i < 10 && i < v.size(); ++i) {
		cout << v[i].first << ": " << v[i].second << endl;
	}

	std::cout << "Word: " <<  "war" << ", est présent: " << *h.get("war") << " fois "<<std::endl;
	std::cout << "Word: " <<  "peace" << ", est présent: " << *h.get("peace") << " fois "<<std::endl;
	std::cout << "Word: " <<  "toto" << ", est présent: " << *h.get("toto") << " fois "<<std::endl;

	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;
	
    return 0;
}


