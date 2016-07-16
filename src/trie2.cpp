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
	string s;
	if (i == 0) {
		s = 'A';
	}
	else if (i == 1) {
		s = 'C';
	}
	else if (i == 2) {
		s = 'G';
	}
	else {
		s = 'T';
	}
	return s;
}


void resize_matrix(mat& trie_matrix) {
	matrix_size += 100000;
	trie_matrix.resize(5, matrix_size);
}

int searchWord(string s, mat& trie_matrix)
{
	int current_node = 0;
    for ( int i = 0; i < s.length(); i++ )
    {   
    	if (trie_matrix(letter_index(s[i]), current_node) == 0) {
    		return -1;
    	}
    	current_node = trie_matrix(letter_index(s[i]), current_node);
    }
    return trie_matrix(4, current_node);
//    if (trie_matrix(4, current_node) != -1) {
//    	return true;
//    }
//    else {
//    	return false;
//    }    
}

void step(string s, mat& trie_matrix, int current_node, int maxmis, int current_mistakes, int i, vector<int>& all_words) 
{
	cout << "i " << i <<"\n";
	cout << "cur node " << current_node <<"\n";
	cout << " cur_mis " <<current_mistakes <<"\n";
	int current_mistake = 0;
	if (i == s.length()) {
		if (trie_matrix(4, current_node) != -1) {
			cout << "ind " << trie_matrix(4, current_node) <<"\n";
			all_words.push_back(trie_matrix(4, current_node));
		}
	}
	else {
		if (trie_matrix(4, current_node) != -1)
		{
			return;
		}
		for (int j = 0; j < 4; j++) {
			cout << "j " << j << "\n";
			if (trie_matrix(j, current_node) != 0) {
				cout << "i " << i << " current_node " << current_node << " s[i] " <<s[i] << "\n"; 
				if (letter_index(s[i]) == j) {
					current_mistake = 0;
				}
				else {
					current_mistake = 1;
				}
				cout << "current_mistake " << current_mistake << "\n";
				if (current_mistake + current_mistakes > maxmis) {
					cout <<"max mistake achieved\n";
					return;
				}
				current_mistakes += current_mistake;
				cout <<"total_current_mistakes "<< current_mistakes << "\n";
				current_node = trie_matrix(j, current_node);
				cout << "current_node " << current_node << "\n";
				i += 1;
				step(s, trie_matrix, current_node, maxmis, current_mistakes, i, all_words); 
			}
		}
	}
}

vector<int> searchWordmismatch(string s, mat& trie_matrix, int maxmis)
{
	int current_node = 0;
	vector<int> all_words;
	int i = 0;
	int current_mistakes = 0;
    step(s, trie_matrix, current_node, maxmis, current_mistakes, i, all_words); 
    for (int j = 0; j < all_words.size(); j++ ) 
	{
		cout << all_words[j] << "\t";
	}
    cout << "\n";
    return all_words;
}


void addWord(string s, mat& trie_matrix, int j)
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
    trie_matrix(4, current_node) = j;    
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
	for (int i = 0; i < matrix_size; i++) {
		trie_matrix(4, i) = -1;
	}
//	for (int i = 0; i < 10000; i++) 
//	{
//		addWord(first_file_seq[i], trie_matrix, i);					 					 
//	}
	addWord("ACGT", trie_matrix, 0);
	addWord("ACG", trie_matrix, 1);
    addWord("ACCCC", trie_matrix, 2);
    addWord("GGT", trie_matrix, 3);
//	cout<< searchWord("GGT", trie_matrix) << "\n";
//    cout<< searchWord("GCT", trie_matrix) << "\n";
//    cout<< searchWordmismatch("GCT", trie_matrix, 1) << "\n"; 
    vector<int> wo = searchWordmismatch("GCT", trie_matrix, 1);
    printTrie(trie_matrix);
//    cout << "last " << next_free_node << "\n";
    return 0;
}