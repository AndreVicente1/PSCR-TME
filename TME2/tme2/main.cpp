#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>


int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("./WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;
	
	vector<pair<string,int>> v;
	
	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	bool trouve;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		for (auto& pair: v){
			if (pair.first == word){
				trouve = true;
				pair.second++;
				break;
			}
		}	
		if (!trouve){
			v.push_back(make_pair(word,1));	
		trouve = false;
		}

	}
	
	for (const auto& pair : v) {
		if (pair.first == "war" || pair.first == "peace" ||pair.first == "toto"){
			std::cout << "Word: " << pair.first << ", est présent: " << pair.second << " fois "<<std::endl;
		}
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;
	
    return 0;
}


