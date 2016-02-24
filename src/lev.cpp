#include <fstream>
#include <string>
#include <iostream>
#include <tuple>
#include <functional>
#include <vector>
#include <chrono>
#include <ctime>


using namespace std;
using namespace std::chrono;

class SparseLevenshteinAutomaton {
  private:
	string word;
	int n;
  public:
    void set_values (string auto_word, int num_mis);
    tuple<vector<int>, vector<int>> start();
    tuple<vector<int>, vector<int>> step(tuple<vector<int>, vector<int>> &previous, char c);
    bool is_match(tuple<vector<int>, vector<int>>& previous);
};
    
void SparseLevenshteinAutomaton::set_values (string auto_word, int num_mis) {
      word = auto_word;
      n = num_mis;
    }

tuple<vector<int>, vector<int>> SparseLevenshteinAutomaton::start () {
	vector<int> range_max_edits;
	for (int i=0; i <= n; i++) {
		range_max_edits.push_back(i);
	}
	return make_tuple(range_max_edits, range_max_edits); 
}

tuple<vector<int>, vector<int>> SparseLevenshteinAutomaton::step(tuple<vector<int>, vector<int>>& previous, char c) {
	vector<int> new_indices;
	vector<int> new_values;
	if (get<0>(previous).size() and get<0>(previous).at(0) == 0 and get<1>(previous).at(0) < n) {
		new_indices = {0};
		new_values = {get<1>(previous).at(0) + 1};
	}
	else {
		new_indices = {};
		new_values = {};
	}
	for (int i = 0; i < get<0>(previous).size(); i++) {
	}
	for (int i = 0; i < get<0>(previous).size(); i++) {
		if (get<0>(previous).at(i) == word.length()) {
			break;
		}
		int cost;
		if (word[get<0>(previous).at(i)] == c)
		{
			cost = 0;
		}
		else {
			cost = 1;
		}
		int val = get<1>(previous).at(i) + cost;
		if (new_indices.size() and new_indices.at(new_indices.size() - 1) == get<0>(previous).at(i)) {
			val = min(val, (new_values.at(new_values.size() - 1) + 1));
		}
		if ((i + 1) < get<0>(previous).size() and get<0>(previous).at(i + 1) == get<0>(previous).at(i) + 1) {
			val = min(val, (get<1>(previous).at(i + 1) + 1));
		}
		if (val <= n) {
			new_indices.push_back(get<0>(previous).at(i) + 1);
			new_values.push_back(val);
		}
	}
	return make_tuple(new_indices, new_values);

}

bool SparseLevenshteinAutomaton::is_match(tuple<vector<int>, vector<int>>& previous) {
  return (get<0>(previous).size() and get<0>(previous).at(get<0>(previous).size() - 1) == word.length());
}


void benchmark(size_t mistakes, 
				const vector<string> &first_file_seq, 
				 const vector<string> &second_file_seq, 
				 size_t first_file_size, 
				 size_t second_file_size, ofstream &outfile)
{
	std::chrono::milliseconds tp1 = std::chrono::duration_cast<std::chrono::milliseconds>(system_clock::now().time_since_epoch());

	for (int k = 0; k < first_file_size; k++)  {
		string auto_word = first_file_seq[k];
		SparseLevenshteinAutomaton sparse;
		sparse.set_values (auto_word, mistakes);
		tuple<vector<int>, vector<int>> s_sparse1 = sparse.start();
		for (int j = 0; j < second_file_size; j++)  {
			tuple<vector<int>, vector<int>> s_sparse = s_sparse1;
			 for (int i = 0; i < second_file_seq[j].length(); i++) {
				 s_sparse = sparse.step(s_sparse, second_file_seq[j][i]);
				 if ((i == second_file_seq[j].length() - 1) && sparse.is_match(s_sparse)) {
						 // outfile << word_check << " " << auto_word << "\n";
					 	outfile << (int) i << " " << (int) j << "\n";
				 }
			 }
		}
	  }


	std::chrono::milliseconds tp2 = std::chrono::duration_cast<std::chrono::milliseconds>(system_clock::now().time_since_epoch());
	auto diff = tp2.count() - tp1.count();

	cout << "benchmark (" << (size_t) first_file_size << " : " << second_file_size << ") - " << diff << " milsec" << endl;
}


int main (int argc, char** argv) {
  string line1, line2;
  char* file1 = argv[1];
  char* file2 = argv[2];
  int mistakes = atoi(argv[3]);
  string outfilename = argv[4];
  ifstream myfile2 (file2);
  vector<string> first_file_seq;
  vector<string> second_file_seq;
  if (myfile2.is_open())
    {
      while ( getline (myfile2,line2) )
      {
        second_file_seq.push_back(line2);
      }
      myfile2.close();
    }
  ifstream myfile1 (file1);
  ofstream outfile;
  outfile.open(outfilename,  std::ios_base::app);
  if (myfile1.is_open())
      {
        while ( getline (myfile1,line1) )
        {        	
        	first_file_seq.push_back(line1);
        }
        myfile1.close();
      }
  benchmark(mistakes, first_file_seq, second_file_seq, 100, 100, outfile);
  benchmark(mistakes, first_file_seq, second_file_seq, 500, 500, outfile);
  // benchmark(mistakes, first_file_seq, second_file_seq, 1000, 1000, outfile);
  // benchmark(mistakes, first_file_seq, second_file_seq, 3000, 3000, outfile);
  // benchmark(mistakes, first_file_seq, second_file_seq, 5000, 5000, outfile);
  outfile.close();
  return 0;
}