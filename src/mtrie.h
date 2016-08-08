#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <armadillo>
#include <map>
using namespace std;

using namespace arma;

int next_free_node = 1;
int matrix_size = 100000;
 
 
 
 typedef std::map<char, int> let_index;

 int letter_index(char c)
 {
	 let_index m;
     m['A'] = 0;
     m['C'] = 1;
     m['G'] = 2;
     m['T'] = 3;

     return m[c];

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
}

void step(string s, mat& trie_matrix, int current_node, int maxmis, int current_mistakes, int i, vector<int>& all_words) 
{
//	cout << "curr " << current_node << " mis " << current_mistakes << " " << i <<  "\n";
	int current_mistake = 0;
	if (i == s.length()) {
//		cout << "curr " << current_node << "\n";
//		cout << "mis " << current_mistakes << "\n";
		if (current_mistakes < maxmis) {
//			cout << "curr2 " << current_node << "\n";
			for (int j = 0; j < 4; j++) {
				if (trie_matrix(j, current_node) != 0) {
					step(s, trie_matrix, trie_matrix(j, current_node), maxmis, current_mistakes + 1, i, all_words); 		
				}
			}
		}
		if (trie_matrix(4, current_node) != 0) {
			all_words.push_back(trie_matrix(4, current_node));
		}
	}
	else {
//		if (trie_matrix(4, current_node) == -1) { 
//			cout << "curr6 " << current_node << " " << current_mistake << " " << current_mistakes << "\n";
			for (int j = 0; j < 4; j++) {
				if (trie_matrix(j, current_node) != 0) {
//					cout << "curr7 " << current_node << " " << current_mistake << " " << current_mistakes << "\n";
					current_mistake = letter_index(s[i]) != j;
//					cout << "curr3 " << current_node << " " << current_mistake << " " << current_mistakes << "\n";
					if (current_mistake + current_mistakes <= maxmis) {			
//						 << "curr " << current_node << " " << current_mistake << " " << current_mistakes << "\n";
//						 << "mis2 " << current_mistakes + current_mistake <<"\n";
						step(s, trie_matrix, trie_matrix(j, current_node), maxmis, current_mistakes + current_mistake, i + 1, all_words); 
					}
					if (current_mistakes + 1 <= maxmis) {				
						step(s, trie_matrix, current_node, maxmis, current_mistakes + 1, i + 1, all_words); 
					}
					if (current_mistakes + 1 <= maxmis) {				
						step(s, trie_matrix, trie_matrix(j, current_node), maxmis, current_mistakes + 1, i, all_words); 
					}
	//				else {
	//					cout <<"max mistake achieved\n";
	//				}
				}
			}
//			for (int j = 0; j < 4; j++) {
//				if (trie_matrix(j, current_node) != 0) {
//					
//	//				else {
//	//					cout <<"max mistake achieved\n";
//	//				}
//				}
//			}			
//		}
			if (trie_matrix(4, current_node) != 0) {
			if (current_mistakes < maxmis) {
				step(s, trie_matrix, current_node, maxmis, current_mistakes + 1, i + 1, all_words); 
			}
		}
	}
}

void searchWordmismatch(string s, mat& trie_matrix, int maxmis, int number)
{
	int current_node = 0;
	vector<int> all_words;
	int i = 0;
	int current_mistakes = 0;
    step(s, trie_matrix, current_node, maxmis, current_mistakes, i, all_words);     
    sort( all_words.begin(), all_words.end() );
    all_words.erase( unique( all_words.begin(), all_words.end() ), all_words.end() );
    if (all_words.size()) {    
		for (int j = 0; j < all_words.size(); j++ ) 
		{
			cout << number << " " << all_words[j] << "\n";
		}
    }
//    return all_words;
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


