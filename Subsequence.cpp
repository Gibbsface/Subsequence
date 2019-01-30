#include <iostream>
#include <fstream>
#include "ArgumentManager.h"

using namespace std;

void readLines(string infilename, string* stringSet, int &numOfStrings);
void writeAns(string outfilename, int lcs_final);
int findLCS(string* stringSet, int numSig);
int LCS_2D(string a, string b);
int LCS_3D(string a, string b, string c);
int LCS_4D(string a, string b, string c, string d);
int maximum(int a, int b);
int maximum(int a, int b, int c);
int maximum(int a, int b, int c, int d);

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << "Usage: Subsequence \"input=<file>;output=<file>\"" << endl;
		return -1;
	}
	
	ArgumentManager am(argc, argv);

	string infilename = am.get("input");
	string outfilename = am.get("output");
	//cout << "Input file name: " << infilename << endl << "Output file name: " << outfilename << endl << endl;

	string* stringSet = NULL;
	int numOfStrings = 0;
	stringSet = new string[4];
	for(int i = 0; i < 4; i++){
		stringSet[i] = "";
		//cout << stringSet[i] << endl;
	}

	readLines(infilename, stringSet, numOfStrings);

	//Test by printing out numOfStrings
	/*
	for(int i = 0; i < 4; i++){
		if(stringSet[i] != ""){
			cout << stringSet[i] << endl;
		}
	}
	cout << endl << "Number of signifigant lines: " << numOfStrings << endl << endl;
	*/

	//Find the Longest Common Subsequence
	int lcs_final = findLCS(stringSet, numOfStrings);

	//Print to output file
	writeAns(outfilename, lcs_final);

	// cout << "Len: " << lcs_final << endl;

	delete [] stringSet;

	return 0;
}

void readLines(string infilename, string* StringSet, int &numOfStrings) {
	ifstream in;
	string buffer;
	int i = 0;

	in.open(infilename);

	while(getline(in, buffer)){
		if(buffer != ""){
			StringSet[i] = buffer;
			i++;
			numOfStrings++;
		}
	}

	in.close();
}

void writeAns(string outfilename, int ans){
	ofstream out;
	out.open(outfilename);
	out << "Len: " << ans;

	out.close();
}

int findLCS(string* stringSet, int numSig) {

	int lcs_final = 0;

	cout << "Sig lines: " << numSig << endl;

	switch(numSig) {
		case 1 : 
			lcs_final = stringSet[0].length();
			break;
		case 2 :
			lcs_final = LCS_2D(stringSet[0], stringSet[1]);
			cout << "call to 2D" << endl;
			break;
		case 3 : 
			lcs_final = LCS_3D(stringSet[0], stringSet[1], stringSet[2]);
			cout << "call to 3D" << endl;
			break;
		case 4 : 
			lcs_final = LCS_4D(stringSet[0], stringSet[1], stringSet[2], stringSet[3]);
			cout << "call to 4D" << endl;
			break;
	}
	
	/*
	int lcs_final = LCS(stringSet[0], stringSet[1]);

	for(int i = 0; i < numSig - 1; i++){
		//cout << lcs_final << endl;
		lcs_final = min(LCS(stringSet[i], stringSet[i+1]), lcs_final);
	}*/

	return lcs_final;
}

int LCS_2D(string a, string b) {
	int max = 0;

	const char* a_ary = a.c_str();
	const char* b_ary = b.c_str();

	int numRows = b.length() + 1;
	int numCols = a.length() + 1;

	//Create and Initialize 2D dynamic array
	int** backpack = new int*[numRows];
	for(int i = 0; i < numRows; i++) {
		backpack[i] = new int[numCols];
		for(int j = 0; j < numCols; j++){
			backpack[i][j] = 0;
			//cout << i << ',' << j << " \t";
		}
		//cout << endl;
	}
	//cout << endl;

	//Populate array with values
	for(int i = 1; i < numRows; i++){
		for(int j = 1; j < numCols; j++){
			if((char)a_ary[j-1] == (char)b_ary[i-1]){
				backpack[i][j] = 1 + backpack[i-1][j-1];
				//cout << (char)b_ary[i-1];
			} else {
				backpack[i][j] = maximum(backpack[i-1][j], backpack[i][j-1]);
				//cout << (char)b_ary[i-1];
			}
			//cout << backpack[i][j] << '\t';
		}
		//cout << endl;
	}
	//cout << endl;

	for(int i = 0; i < numRows; i++){
		delete [] backpack[i];
	}
	delete [] backpack;

	//cout << backpack[numRows-1][numCols-1] << endl;
	return backpack[numRows-1][numCols-1];
}

int LCS_3D(string a, string b, string c) {
	int max = 0;

	const char* a_ary = a.c_str();
	const char* b_ary = b.c_str();
	const char* c_ary = c.c_str();

	int numA = a.length() + 1;
	int numB = b.length() + 1;
	int numC = c.length() + 1;

	//Create and Initialize 3D dynamic array
	int*** backpack = new int**[numA];
	for(int i = 0; i < numA; i++) {
		backpack[i] = new int*[numB];
		for(int j = 0; j < numB; j++){
			backpack[i][j] = new int[numC];
			for(int k = 0; k < numC; k++){
				backpack[i][j][k] = 0;
			}
		}
	}

	//Populate array with values
	for(int i = 1; i < numA; i++){
		for(int j = 1; j < numB; j++){
			for(int k = 1; k < numC; k++){
				if((char)a_ary[i-1] == (char)b_ary[j-1] && (char)c_ary[k-1] == (char)b_ary[j-1]){
					backpack[i][j][k] = 1 + backpack[i-1][j-1][k-1];
				} else {
					backpack[i][j][k] = maximum(backpack[i-1][j][k], backpack[i][j-1][k], backpack[i][j][k-1]);
				}
			}
		}
	}

	int lcs_final = backpack[numA-1][numB-1][numC - 1];

	for(int i = 0; i < numA; i++){
		for(int j = 0; j < numB; j++){
			delete [] backpack[i][j];
		}
		delete [] backpack[i];
	}
	delete [] backpack;

	return lcs_final;
}

int LCS_4D(string a, string b, string c, string d) {
	int max = 0;

	const char* a_ary = a.c_str();
	const char* b_ary = b.c_str();
	const char* c_ary = c.c_str();
	const char* d_ary = d.c_str();

	int numA = a.length() + 1;
	int numB = b.length() + 1;
	int numC = c.length() + 1;
	int numD = d.length() + 1;

	//Create and Initialize 3D dynamic array
	int**** backpack = new int***[numA];
	for(int i = 0; i < numA; i++) {
		backpack[i] = new int**[numB];
		for(int j = 0; j < numB; j++){
			backpack[i][j] = new int*[numC];
			for(int k = 0; k < numC; k++){
				backpack[i][j][k] = new int[numD];
				for(int l = 0; l < numD; l++){
					backpack[i][j][k][l] = 0;
				}
			}
		}
	}

	//Populate array with values
	for(int i = 1; i < numA; i++){
		for(int j = 1; j < numB; j++){
			for(int k = 1; k < numC; k++){
				for(int l = 1; l < numD; l++){
					if((char)a_ary[i-1] == (char)b_ary[j-1] 
						&& (char)b_ary[j-1] == (char)c_ary[k-1] 
						&& (char)c_ary[k-1] == (char)d_ary[l-1])
					{
						backpack[i][j][k][l] = 1 + backpack[i-1][j-1][k-1][l-1];
					} else {
						backpack[i][j][k][l] = maximum(backpack[i-1][j][k][l], backpack[i][j-1][k][l],
													   backpack[i][j][k-1][l], backpack[i][j][k][l-1]);
					}
				}
			}
		}
	}

	int lcs_final = backpack[numA-1][numB-1][numC-1][numD-1];

	for(int i = 0; i < numA; i++){
		for(int j = 0; j < numB; j++){
			for(int k = 0; k < numC; k++){
				delete [] backpack[i][j][k];
			}
			delete [] backpack[i][j];
		}
		delete [] backpack[i];
	}
	delete [] backpack;

	return lcs_final;
}

int maximum(int a, int b) {
	if(a >= b) {
		return a;
	} else {
		return b;
	}
}

int maximum(int a, int b, int c) {return maximum(maximum(a, b), c);}

int maximum(int a, int b, int c, int d) {return maximum(maximum(a, b, c), d);}