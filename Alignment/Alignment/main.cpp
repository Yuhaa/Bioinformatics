#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#define RIGHT 1
#define DIAG 2
#define DOWN 3

int direction;
string alignment_a, alignment_b, seq1, seq2;

int getMax(int right, int down, int diag);
void backTrack(int **dir_arr, int i, int j);

int main() {
	ifstream in_f_seq1, in_f_seq2, in_f_score;
	ofstream out_f_output;
	string temp;
	int match, mismatch, gap;

	/******file read*********/
	in_f_seq1.open("input1.txt");
	in_f_seq2.open("input2.txt");
	in_f_score.open("score.txt");
	out_f_output.open("output.txt");

	if (in_f_seq1.is_open() && in_f_seq2.is_open() && in_f_score.is_open()) {
		in_f_seq1 >> temp >> seq1;
		in_f_seq2 >> temp >> seq2;
		
		int index_eq;
		in_f_score >> temp;
		index_eq = temp.find("=");
		match = stoi(temp.substr(index_eq + 1));
		in_f_score >> temp;
		index_eq = temp.find("=");
		mismatch = stoi(temp.substr(index_eq + 1));
		in_f_score >> temp;
		index_eq = temp.find("=");
		gap = stoi(temp.substr(index_eq + 1));
	} else {
		fprintf(stderr, "Need 3 files\n");
		return 0;
	}

	//allocate matrix
	int **arr = new int*[seq2.length() + 1];
	int **dir_arr = new int*[seq2.length() + 1];
	for (int i = 0; i < seq2.length() + 1; i++) {
		arr[i] = new int[seq1.length() + 1];
		dir_arr[i] = new int[seq1.length() + 1];
	}

	/******initialize matrix******/
	for (int i = 0; i < seq2.length() + 1; i++) {
		for (int j = 0; j < seq1.length() + 1; j++) {
			arr[i][j] = dir_arr[i][j] = 0;
		}
	}

	for (int i = 1; i < seq1.length() + 1; i++) {
		arr[0][i] = arr[0][i - 1] + gap;
	}

	for (int i = 1; i < seq2.length() + 1; i++) {
		arr[i][0] = arr[i - 1][0] + gap;
	}

	cout << "seq1 = " << seq1 << endl;
	cout << "seq2 = " << seq2 << endl;
	cout << "match = " << match << endl;
	cout << "mismatch = " << mismatch << endl;
	cout << "gap = " << gap << endl;
	cout << endl;

	int right, diag, down; //Direction
	for (int i = 1; i < seq2.length() + 1; i++) {
		for (int j = 1; j < seq1.length() + 1; j++) {
			right = arr[i][j - 1] + gap;
			down = arr[i - 1][j] + gap;
			if (seq1[j - 1] == seq2[i - 1]) {
				diag = arr[i - 1][j - 1] + match;
			} else {
				diag = arr[i - 1][j - 1] + mismatch;
			}

			arr[i][j] = getMax(right, down, diag);
			dir_arr[i][j] = direction;
		}
	}

	for (int i = 0; i < seq2.length() + 1; i++) {
		for (int j = 0; j < seq1.length() + 1; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}

	backTrack(dir_arr, seq2.length(), seq1.length());

	cout << endl;
	cout << alignment_a << endl;
	cout << alignment_b << endl;

	int num_match = 0, num_mismatch = 0, num_gap = 0, score;

	for (int i = 0; i < alignment_a.length(); i++) {
		if (alignment_a[i] == '-' || alignment_b[i] == '-') {
			num_gap++;
		} else if (alignment_a[i] == alignment_b[i]) {
			num_match++;
		} else {
			num_mismatch++;
		}
	}

	score = num_match * match + num_mismatch * mismatch + num_gap * gap;

	out_f_output << "matches: " << num_match << endl;
	out_f_output << "mismatches: " << num_mismatch << endl;
	out_f_output << "gaps: " << num_gap << endl;
	out_f_output << "score: " << score << endl;
	out_f_output << alignment_a << endl;
	out_f_output << alignment_b << endl;

	for (int i = 0; i < seq2.length() + 1; i++) {
		delete[] arr[i];
		delete[] dir_arr[i];
	}

	delete[] arr;
	delete[] dir_arr;

	in_f_seq1.close();
	in_f_seq2.close();
	in_f_score.close();
	out_f_output.close();


	
	return 0;

}

int getMax(int right, int down, int diag) {
	int max, temp;

	if (right > down) {
		temp = right;
		direction = RIGHT;
	} else {
		temp = down;
		direction = DOWN;
	}

	if (temp > diag) {
		max = temp;
	} else {
		max = diag;
		direction = DIAG;
	}

	return max;
}

void backTrack(int **dir_arr, int i, int j) {
	if (i == 0 && j == 0) return;

	if (dir_arr[i][j] == RIGHT) {
		backTrack(dir_arr, i, j - 1);
		alignment_a += seq1[j - 1];
		alignment_b += '-';
	} else if (dir_arr[i][j] == DIAG) {
		backTrack(dir_arr, i - 1, j - 1);
		alignment_a += seq1[j - 1];
		alignment_b += seq2[i - 1];
	} else {
		backTrack(dir_arr, i - 1, j);
		alignment_a += '-';
		alignment_b += seq2[i - 1];
	}
}