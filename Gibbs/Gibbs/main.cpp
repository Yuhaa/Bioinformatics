#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#define NUM_ALPHA 4
#define NUM_REPEAT 20
enum alpha {A, C, G ,T};

void makeProfile(long double ** profile, vector<string> &motifs, int k);
string findMotif(long double** profile, vector<string> &sequences, string sequence, int k);
int getScore(vector<string> &motifs, int k);
int getIndex(char ch);

int main(int argc, char** argv) {
	srand((unsigned int)time(NULL)); //Random

	vector<string> sequences;
	int k, num_string, max_string_length;
	ifstream in_file("test01.txt");
	ofstream out_file("output.txt");

	//Read file
	sequences.clear();
	if (in_file.is_open()) {
		in_file >> k >> num_string >> max_string_length;

		string temp;
		while (true) {
			in_file >> temp;
			if (in_file.eof()) break;
			sequences.push_back(temp);
		}

		/*while (!in_file.eof()) {
			in_file >> temp;
			sequences.push_back(temp);
		}*/
 	}

	int best_score;
	vector<string> motifs, best_motifs;
	long double **profile = new long double*[NUM_ALPHA]; //0: a, 1: c, 2: g, 3: t
	for (int i = 0; i < NUM_ALPHA; i++) {
		profile[i] = new long double[k];
	}

	for (int iterator = 0; iterator < NUM_REPEAT; iterator++) {
		motifs.clear();

		//Choose initial motifs randomly
		for (int i = 0; i < num_string; i++) {
			int max_index = sequences[i].length() - k + 1;
			int start = rand() % max_index;
			string motif = sequences[i].substr(start, k);
			motifs.push_back(motif);
		}

		if (iterator == 0) {
			best_score = getScore(motifs, k);
			best_motifs = motifs;
		}

		int temp_best_score = getScore(motifs, k);
		vector<string> temp_best_motifs = motifs;

		for (int i = 0; i < num_string * 10; i++) {
			//Remove a motif randomly
			int random_index = rand() % num_string;
			motifs.erase(motifs.begin() + random_index);

			//Make profile with erased motifs
			makeProfile(profile, motifs, k);

			//Find most probable motif and add it
			string sequence = sequences[random_index];
			string result_motif = findMotif(profile, sequences, sequence, k);
			motifs.insert(motifs.begin() + random_index, result_motif);

			//Get Score
			int score = getScore(motifs, k);

			if (score > temp_best_score) {
				temp_best_score = score;
				temp_best_motifs = motifs;
			}
		}

		if (temp_best_score > best_score) {
			best_score = temp_best_score;
			best_motifs = temp_best_motifs;
		}
	}

	makeProfile(profile, best_motifs, k);

	for (int i = 0; i < num_string; i++) {
		for (int j = 0; j < k; j++) {
			out_file << best_motifs[i][j];
		}
		out_file << endl;
	}

	for (int i = 0; i < NUM_ALPHA; i++) {
		for (int j = 0; j < k; j++) {
			out_file << profile[i][j] << " ";
		}
		out_file << endl;
	}

	//Deallocation
	for (int i = 0; i < NUM_ALPHA; i++) {
		delete[] profile[i];
	}
	delete[] profile;
	in_file.close();
	out_file.close();
	return 0;
}

void makeProfile(long double** profile, vector<string>& motifs, int k) {
	for (int i = 0; i < NUM_ALPHA; i++) {
		for (int j = 0; j < k; j++) {
			profile[i][j] = 1.0; //pseudo count
		}
	}

	for (int i = 0; i < k; i++) {
		for (int j = 0; j < motifs.size(); j++) {
			int index = getIndex(motifs[j][i]);

			if (index != -1) {
				profile[index][i] += 1.0;
			}
		}

		for (int j = 0; j < NUM_ALPHA; j++) {
			profile[j][i] /= (motifs.size() * 2);
		}
	}
}

string findMotif(long double** profile, vector<string>& sequences, string sequence, int k) {
	long double max_prob = 0.0;
	string result_motif;

	for (int i = 0; i <= sequence.length() - k; i++) {
		string temp_motif = sequence.substr(i, k);
		long double prob = 1.0;
		for (int j = 0; j < k; j++) {
			int index = getIndex(temp_motif[j]);

			if (index != -1) {
				prob *= profile[index][j];
			}
		}

		if (prob > max_prob) {
			max_prob = prob;
			result_motif = temp_motif;
		}
	}

	return result_motif;
}

//Sum of maximum number of each vertical lines
int getScore(vector<string>& motifs, int k) {
	int score = 0;
	for (int i = 0; i < k; i++) {
		int count[NUM_ALPHA] = { 0, };

		for (int j = 0; j < NUM_ALPHA; j++) {
			int index = getIndex(motifs[j][i]);
			if (index != -1) {
				count[index]++;
			}
		}

		int max = 0;
		for (int j = 0; j < NUM_ALPHA; j++) {
			if (count[j] > max) {
				max = count[j];
			}
		}

		score += max;
	}

	return score;
}

int getIndex(char ch) {
	int index = -1;
	switch (ch) {
	case 'a':
	case 'A': index = 0; break;
	case 'c':
	case 'C': index = 1; break;
	case 'g':
	case 'G': index = 2; break;
	case 't':
	case 'T': index = 3; break;
	}

	return index;
}