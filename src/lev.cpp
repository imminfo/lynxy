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
		indices.reserve(1000);
		values.reserve(1000);
		indices1.reserve(1000);
		values1.reserve(1000);
	}
	void write_index(int c, int ind);
	void write_value(int c, int val);
	void clear_sparsevec();
	int get_value_val(int ind);
	int get_index_val(int ind);
	void write_index_1(int c, int ind);
	void write_value_1(int c, int val);
	void clear_sparsevec_1();
	int get_value_val_1(int ind);
	int get_index_val_1(int ind);
private:
	vector<int> indices;
	vector<int> values;
	vector<int> indices1;
	vector<int> values1;
};

void SparseVector::write_value (int c, int val) {
	values[c] = val;
}

void SparseVector::write_index (int c, int ind) {
	indices[c] = ind;
}

void SparseVector::clear_sparsevec () {
	indices.clear();
	values.clear();
}

int SparseVector::get_value_val (int ind) {
	return values[ind];
}

int SparseVector::get_index_val (int ind) {
	return indices[ind];
}

void SparseVector::write_value_1 (int c, int val) {
	values1[c] = val;
}

void SparseVector::write_index_1 (int c, int ind) {
	indices1[c] = ind;
}

void SparseVector::clear_sparsevec_1 () {
	indices1.clear();
	values1.clear();
}

int SparseVector::get_value_val_1 (int ind) {
	return values1[ind];
}

int SparseVector::get_index_val_1 (int ind) {
	return indices1[ind];
}


class SparseLevenshteinAutomaton {
  private:
	string word;
	int n;
  public:
    void set_values (string auto_word, int num_mis);
    void start(SparseVector *vec);
    void step(SparseVector *vec, char c, int *vector_size);
    bool is_match(SparseVector *vec, int *vector_size);
};
    
void SparseLevenshteinAutomaton::set_values (string auto_word, int num_mis) {
      word = auto_word;
      n = num_mis;
    }

void SparseLevenshteinAutomaton::start (SparseVector *vec) {
	vec -> clear_sparsevec();
	for (int i=0; i <= n; i++) {
		vec -> write_index(i, i);
		vec -> write_value(i, i);
	}
}

void SparseLevenshteinAutomaton::step(SparseVector *vec, char c, int *vector_size) {
//	cout << "prev1 ";
//	for (int i=0; i < get<0>(previous).size(); i++) {
//		cout << get<0>(previous)[i] << " ";
//	}
//	cout <<"\n";
//	cout << "prev2 ";
//	for (int i=0; i < get<1>(previous).size(); i++) {
//		cout << get<1>(previous)[i] << " ";
//	}
//	cout <<"\n";
//	
//	vector<int> new_indices;
//	vector<int> new_values;
	int new_size = 0;
	if (*vector_size > 0 and vec->get_index_val(0) == 0 and vec->get_value_val(0) < n) {
		vec->write_index_1(0, 0);
		vec->write_value_1(0, (vec->get_value_val(0) + 1)); // {get<1>(previous).at(0) + 1};
		new_size = 1;
	}
//	for (int i=0; i<new_size; i++) {
//		cout << "11 " << vec->get_index_val_1(i) << "\n";
//		cout << "22 " << vec->get_value_val_1(i) << "\n";
//	}
//	else {
//		new_indices = {};
//		new_values = {};
//	}
	for (int i = 0; i < *vector_size; i++) {
		if (vec->get_index_val(i) == word.length()) {
			break;
		}
		int cost;
		if (word[vec->get_index_val(i)] == c)
		{
			cost = 0;
		}
		else {
			cost = 1;
		}
//		cout << "w " << c <<" " << vec->get_index_val(i) << " " << word[vec->get_index_val(i)] << "\n";
		int val = vec->get_value_val(i) + cost;
		if (new_size > 0 and vec->get_index_val_1(new_size - 1) == vec->get_index_val(i)) {
			val = min(val, (vec->get_value_val_1(new_size - 1) + 1));
		}
		if ((i + 1) < *vector_size and vec->get_index_val(i + 1) == vec->get_index_val(i) + 1) {
			val = min(val, (vec->get_value_val(i + 1) + 1));
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
			vec->write_index_1(new_size, (vec->get_index_val(i) + 1));
			vec->write_value_1(new_size, val);
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
//	for (int i=0; i < new_size; i++) {
//		cout << vec->get_index_val_1(i) << " ";
//	}
//	cout <<"\n";
//	cout << "newval ";
//	for (int i=0; i < new_size; i++) {
//		cout << vec->get_value_val_1(i) << " ";
//	}
//	cout <<"\n";
	vec->clear_sparsevec();
	for (int j = 0; j < new_size; j++) {
	  vec->write_index(j, vec->get_index_val_1(j));
	  vec->write_value(j, vec->get_value_val_1(j));
	}
	vec->clear_sparsevec_1();
	
	*vector_size = new_size; 

}

bool SparseLevenshteinAutomaton::is_match(SparseVector *vec, int *vector_size) {
  return (*vector_size > 0 and vec->get_index_val(*vector_size - 1) == word.length());
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
		// string auto_word = first_file_seq[k];
		// SparseLevenshteinAutomaton sparse;
		sparse.set_values (first_file_seq[k], mistakes); 
		for (int j = 0; j < second_file_size; j++)  {
			 int vector_size = mistakes + 1;
			 sparse.start(&vec);   //start automaton every time
//			 cout <<"start ";
//			 for (int y=0; y <= mistakes;y++) {
//				 cout << vec.get_index_val(y) << " ";
//			 }
//			 cout <<"\n";
//			 for (int y=0; y <= mistakes;y++) {
//				 cout << vec.get_value_val(y) << " ";
//			 }
//			 cout <<"\n";
			 for (int i = 0; i < second_file_seq[j].length(); i++) {
				 sparse.step(&vec, second_file_seq[j][i], &vector_size); /// ПЕРЕНЕСТИ В SPARSE_VECTOR
				 if ((i == second_file_seq[j].length() - 1) && sparse.is_match(&vec, &vector_size)) {
//						  outfile << word_check << " " << auto_word << "\n";
					 	outfile << (int) (k+1) << " " << (int) (j+1) << "\n";
					 	// cout << (size_t) get<0>(s_sparse).size() << ":" << (size_t) get<1>(s_sparse).size() << endl;
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
//  benchmark(mistakes, first_file_seq, second_file_seq, 2, 4, outfile);
//  benchmark(mistakes, first_file_seq, second_file_seq, 500, 500, outfile);
//   benchmark(mistakes, first_file_seq, second_file_seq, 1000, 1000, outfile);
//   benchmark(mistakes, first_file_seq, second_file_seq, 3000, 3000, outfile);
   benchmark(mistakes, first_file_seq, second_file_seq, 5000, 5000, outfile);
  outfile.close();
  return 0;
}
