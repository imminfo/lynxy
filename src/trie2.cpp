#include "mtrie.h"
#include "trie.h"



int main () {
//	  string line1, line2;
//	  char* file1 = argv[1];
//	  char* file2 = argv[2];
//	  int mistakes = atoi(argv[3]);
//	  string outfilename = argv[4];
//	  ifstream myfile2 (file2);
//	  vector<string> first_file_seq;
//	  vector<string> second_file_seq;
//	  if (myfile2.is_open())
//	    {
//	      while ( getline (myfile2,line2) )
//	      {
//	        second_file_seq.push_back(line2);
//	      }
//	      myfile2.close();
//	    }
//	  ifstream myfile1 (file1);
//	  ofstream outfile;
//	  outfile.open(outfilename,  std::ios_base::out);
//	  if (myfile1.is_open())
//	      {
//	        while ( getline (myfile1,line1) )
//	        {               
//	                first_file_seq.push_back(line1);
//	        }
//	        myfile1.close();
//	      }
//	size_t first_file_size = first_file_seq.size();
//	size_t second_file_size = second_file_seq.size();
//	cout << "mis "<<mistakes;
//	bench_trie(mistakes, first_file_seq, second_file_seq, 10000, 10000, outfile);
//	 outfile.close();

  string file1 = "/home/marge/lynxy/src/b.txt";
  string file2 = "/home/marge/lynxy/src/a.txt";
  string line1;
  string line2;
  vector<string> first_file_seq;
  vector<string> second_file_seq;  
  ifstream myfile1 (file1);
  if (myfile1.is_open())
	{
	  while ( getline (myfile1,line1) )
	  {        	
		first_file_seq.push_back(line1);
	  }
	  myfile1.close();
	}
   ifstream myfile2 (file2);
   if (myfile2.is_open())
		{
		  while ( getline (myfile2,line2) )
		  {        	
			second_file_seq.push_back(line2);
		  }
		  myfile2.close();
		}
	mat trie_matrix = mat(5, matrix_size, fill::zeros);
	for (int i = 0; i < 10000; i++) 
	{
		addWord(first_file_seq[i], trie_matrix, i + 1);					 					 
	}
//    addWord("GGA", trie_matrix, 8);
//	addWord("ACGT", trie_matrix, 0);
//	addWord("ACG", trie_matrix, 1);
//    addWord("ACCCC", trie_matrix, 2);
//    addWord("GGT", trie_matrix, 3);
//    addWord("GAT", trie_matrix, 4);
//    addWord("GGGAT", trie_matrix, 5);
//    addWord("GGATA", trie_matrix, 6);
//    addWord("AGGAT", trie_matrix, 7);
	for (int i = 0; i < 10000; i++) 
	{
		searchWordmismatch(second_file_seq[i], trie_matrix, 2, i + 1);					 					 
	}
    return 0;
}
