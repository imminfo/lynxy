#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <armadillo>
using namespace std;

using namespace arma;

int next_free_node = 1;
int matrix_size = 100000;

int letter_index(char c) {
	int d = (int)c;
	if (d == 65) {
		return 0;
	}
	else if (c == 67) {
		return 1;
	}
	else if (c == 71) {
		return 2;
	}
	else {
		return 3;
	}
}

string index_letter(int i) {
	if (i == 0) {
		return 'A';
	}
	else if (c == 1) {
		return 'C';
	}
	else if (c == 2) {
		return 'G';
	}
	else {
		return 'T';
	}
}


void resize_matrix(mat& trie_matrix) {
	matrix_size += 100000;
	trie_matrix.resize(5, matrix_size);
}

bool searchWord(string s, mat& trie_matrix)
{
	int current_node = 0;
    for ( int i = 0; i < s.length(); i++ )
    {   
    	if (trie_matrix(letter_index(s[i]), current_node) == 0) {
    		return false;
    	}
    	current_node = trie_matrix(letter_index(s[i]), current_node);
    }
    if (trie_matrix(4, current_node) == 1) {
    	return true;
    }
    else {
    	return false;
    }    
}

vector<string> searchWordmismatch(string s, mat& trie_matrix, int maxmis)
{
	vector<string> words = {""};
	vector<double> mistakes = {0};
	vector<double> mistakes = {0};
	vector<string> words1;
	vector<double> mistakes1;
	int current_mistake;
	int current_node = 0;
    for ( int i = 0; i < s.length(); i++ )
    {   
    	if (i%2 == 0) {
    		for (int j = 0; j < words.length(); j++) {
    			for (int k = 0; k < 4; k++) {
    				if (trie_matrix(letter_index(s[i]), current_node) == 0) {
    					current_mistake = 1;
    				}
    				else {
    					current_mistake = 0;
    				}
    				if ((mistakes[j] + current_mistake) <= maxmis) {
    					words.pushback(strcat(words[j], index_letter(k)))
    					
    				}
    			}
    		}
    	}
    	if (trie_matrix(letter_index(s[i]), current_node) == 0) {
    		return false;
    	}
    	current_node = trie_matrix(letter_index(s[i]), current_node);
    }
    if (trie_matrix(4, current_node) == 1) {
    	return true;
    }
    else {
    	return false;
    }    
}


void addWord(string s, mat& trie_matrix)
{
	int current_node = 0;
    for ( int i = 0; i < s.length(); i++ )
    {   
    	int let_index = letter_index(s[i]);
    	if (trie_matrix(let_index, current_node) == 0) {
    		trie_matrix(let_index, current_node) = next_free_node;
    		current_node = next_free_node;
			next_free_node += 1;
			if (next_free_node >= matrix_size - 1) {
				resize_matrix(trie_matrix);
			}
    	}
    	else {
    		current_node = trie_matrix(let_index, current_node);
    	}
    }
    trie_matrix(4, current_node) = 1;
    
}


void printTrie(mat& trie_matrix) 
{
	for (int i = 0; i < 5; i++ ) 
	{
		for (int j = 0; j < 50; j++)
		{
			cout << trie_matrix(i, j) << " ";
		}
		cout << "\n";
	}

}


int main()
{	
  string file1 = "/home/marge/lynxy/src/a.txt";
  string line1;
  vector<string> first_file_seq;
  ifstream myfile1 (file1);
	if (myfile1.is_open())
		{
		  while ( getline (myfile1,line1) )
		  {        	
			first_file_seq.push_back(line1);
		  }
		  myfile1.close();
		}
	mat trie_matrix = mat(5, matrix_size, fill::zeros);
//	for (int i = 0; i < 10000; i++) 
//	{
//		addWord(first_file_seq[i], trie_matrix);					 					 
//	}
	addWord("ACGT", trie_matrix);
	addWord("ACG", trie_matrix);
    addWord("ACCCC", trie_matrix);
    addWord("GGT", trie_matrix);
	cout<< searchWord("GGT", trie_matrix) << "\n";
    cout<< searchWord("GCT", trie_matrix) << "\n";
//    printTrie(trie_matrix);
//    cout << "last " << next_free_node << "\n";
    return 0;
}