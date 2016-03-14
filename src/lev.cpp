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

class SparseVector {
public:
	SparseVector(){
		indices.resize(2); 
		values.resize(2);
		indices[0].reserve(1000);
		values[0].reserve(1000);
		indices[1].reserve(1000);
		values[1].reserve(1000);
	}
	void pushback_index(int layer, int ind);
	void pushback_value(int layer, int ind);
	void clear_sparsevec(int layer);
	int get_value_val(int layer, int ind);
	int get_index_val(int layer, int ind);
	int indices_vector_size(int layer);
	int values_vector_size(int layer);
private:
	vector<vector<int> > indices;   
	vector<vector<int> > values;    
};


void SparseVector::pushback_value (int layer, int val) {
	values[layer].push_back(val);
}

void SparseVector::pushback_index (int layer, int ind) {
	indices[layer].push_back(ind);
}

void SparseVector::clear_sparsevec (int layer) {
	indices[layer].clear();
	values[layer].clear();
}

int SparseVector::indices_vector_size (int layer) {
	return indices[layer].size();
}

int SparseVector::values_vector_size (int layer) {
	return indices[layer].size();
}

int SparseVector::get_value_val (int layer, int ind) {
	return values[layer][ind];
}

int SparseVector::get_index_val (int layer, int ind) {
	return indices[layer][ind];
}



class SparseLevenshteinAutomaton {
  private:
	string word;
	int n;
  public:
    void set_values (string auto_word, int num_mis);
    void start(SparseVector *vec);
    void step(SparseVector *vec, char c, int *i);
    bool is_match(SparseVector *vec, int *i);
};
    
void SparseLevenshteinAutomaton::set_values (string auto_word, int num_mis) {
      word = auto_word;
      n = num_mis;
    }

void SparseLevenshteinAutomaton::start (SparseVector *vec) {
	vec -> clear_sparsevec(0);
	for (int i=0; i <= n; i++) {
		vec -> pushback_index(0, i);
		vec -> pushback_value(0, i);
	}
}

void SparseLevenshteinAutomaton::step(SparseVector *vec, char c, int *i) {
	int new_size = 0;
	if (vec->indices_vector_size(*i%2) > 0 and vec->get_index_val(*i%2, 0) == 0 and vec->get_value_val(*i%2, 0) < n) {
		vec -> pushback_index((*i+1)%2, 0);
		vec -> pushback_value((*i+1)%2, vec->get_value_val(*i%2, 0) + 1);
		new_size = 1;
	}
//	for (int h=0; h<new_size; h++) {
//		cout << "11 " << vec->get_index_val_1(h) << "\n";
//		cout << "22 " << vec->get_value_val_1(h) << "\n";
//	}
//	else {
//		new_indices = {};
//		new_values = {};
//	}
	for (int j = 0; j < vec->indices_vector_size(*i%2); j++) {
		if (vec->get_index_val(*i%2, j) == word.length()) {
			break;
		}
		int cost;
		if (word[vec->get_index_val(*i%2, j)] == c)
		{
			cost = 0;
		}
		else {
			cost = 1;
		}
		int val = vec->get_value_val(*i%2, j) + cost;
		if (new_size > 0 and vec->get_index_val((*i+1)%2, new_size - 1) == vec->get_index_val(*i%2, j)) {
			val = min(val, (vec->get_value_val((*i+1)%2, new_size - 1) + 1));
		}
		if ((j + 1) < vec->indices_vector_size(*i%2) and vec->get_index_val(*i%2, j + 1) == vec->get_index_val(*i%2, j) + 1) {
			val = min(val, (vec->get_value_val(*i%2, j + 1) + 1));
		}
//		cout <<"val " << val <<"\n";
//		for (int u=0; u<new_size;u++) {
//			cout <<vec->get_index_val_1(u) << " ";
//		}
//		cout <<"\n";
//		for (int u=0; u<new_size;u++) {
//			cout <<vec->get_value_val_1(u) << " ";
//		}
//		cout<<"\n";
		if (val <= n) {
			vec->pushback_index((*i+1)%2, vec->get_index_val(*i%2, j) + 1);
			vec->pushback_value((*i+1)%2, val);
			new_size += 1;
//			for (int u=0; u<new_size;u++) {
//				cout <<vec->get_index_val_1(u) << " ";
//			}
//			cout <<"\n";
//			for (int u=0; u<new_size;u++) {
//				cout <<vec->get_value_val_1(u) << " ";
//			}
//			cout <<"\n";
		}
	  
	}
//	cout << "newind ";
//	for (int l=0; l < new_size; l++) {
//		cout << vec->get_index_val((*i+1)%2, l) << " ";
//	}
//	cout <<"\n";
//	cout << "newval ";
//	for (int l=0; l < new_size; l++) {
//		cout << vec->get_value_val((*i+1)%2, l) << " ";
//	}
//	cout <<"\n";
	vec->clear_sparsevec(*i%2);

	
//	*vector_size = new_size; 


}

bool SparseLevenshteinAutomaton::is_match(SparseVector *vec, int *i) {
		return (vec->indices_vector_size((*i+1)%2) > 0 and vec->get_index_val((*i+1)%2, vec->indices_vector_size((*i+1)%2) - 1) == word.length());
}


void benchmark(size_t mistakes, 
				const vector<string> &first_file_seq, 
				 const vector<string> &second_file_seq, 
				 size_t first_file_size, 
				 size_t second_file_size, ofstream &outfile)
{
	std::chrono::milliseconds tp1 = std::chrono::duration_cast<std::chrono::milliseconds>(system_clock::now().time_since_epoch());

	SparseLevenshteinAutomaton sparse;
	SparseVector vec;
	for (int k = 0; k < first_file_size; k++)  {
		sparse.set_values (first_file_seq[k], mistakes); 
		for (int j = 0; j < second_file_size; j++)  {
//			 int vector_size = mistakes + 1;
//			 vec.vector_size = mistakes + 1;
			 sparse.start(&vec);   //start automaton every time
//			 cout <<"start ";
//			 for (int y=0; y <= mistakes;y++) {
//				 cout << vec.get_index_val(0, y) << " ";
//			 }
//			 cout <<"\n";
//			 for (int y=0; y <= mistakes;y++) {
//				 cout << vec.get_value_val(0, y) << " ";
//			 }
//			 cout <<"\n";
			 for (int i = 0; i < second_file_seq[j].length(); i++) {
				 sparse.step(&vec, second_file_seq[j][i], &i); 
				 if ((i == second_file_seq[j].length() - 1) && sparse.is_match(&vec, &i)) {
					 	outfile << (int) (k+1) << " " << (int) (j+1) << "\n";
				 }
				 
			 }
			 vec.clear_sparsevec(0);
			 vec.clear_sparsevec(1);
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
//  benchmark(mistakes, first_file_seq, second_file_seq, 2, 4, outfile);
  benchmark(mistakes, first_file_seq, second_file_seq, 500, 500, outfile);
//   benchmark(mistakes, first_file_seq, second_file_seq, 1000, 1000, outfile);
//   benchmark(mistakes, first_file_seq, second_file_seq, 3000, 3000, outfile);
//   benchmark(mistakes, first_file_seq, second_file_seq, 5000, 5000, outfile);
  outfile.close();
  return 0;
}
