#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <algorithm>

using namespace std;
using namespace std::chrono;


#define MAGIC_NUMBER 27

struct trie
{
    struct nucmap {

        nucmap() {
            _data = new trie*[MAGIC_NUMBER];
            for (int i = 0; i < MAGIC_NUMBER; i++) {
                _data[i] = NULL;
            }
        }

        ~nucmap() {
            for (int i = 0; i < MAGIC_NUMBER; i++) {
                delete _data[i];
            }
            delete [] _data;
        }

        trie* operator[](char letter) {
            return _data[letter - 'A'];
        }

        trie* addTrie(char letter) {
            _data[letter - 'A'] = new trie();
            return _data[letter - 'A'];
        }

        trie **_data;
    };

    typedef nucmap next_t;

    // The set with all the letters which this node is prefix
    next_t next;

    //int index;
    vector<int> index;

    trie() : next(nucmap()) { 
      //index = 0;
      index.reserve(2);
    }
    
    ~trie() {}
    

    void insert(string w, int w_index = 0)
    {
  w = '[' + w;

        trie* n = this;
        for (int i = 0; i < w.size(); ++i) {
            if (!n->next[w[i]]) {
                n->next.addTrie(w[i]);
            }
            n = n->next[w[i]];
        }

        //n->index = w_index;
        n->index.push_back(w_index);
    }
};


//
std::vector<int> search_impl(trie* tree, char ch, int *last_row, int sz, const string& word, int min_cost)
{
    int *current_row = new int[sz + 1];
    current_row[0] = last_row[0] + 1;

    // Calculate the min cost of insertion, deletion, match or substution
    int insert_or_del, replace;
    for (int i = 1; i < sz + 1; ++i) {
        insert_or_del = min(current_row[i-1] + 1, last_row[i] + 1);
        replace = (word[i-1] == ch) ? last_row[i-1] : (last_row[i-1] + 1);

        current_row[i] = min(insert_or_del, replace);
    }

    // When we find a cost that is less than the min_cost, is because
    // it is the minimum until the current row, so we update
    std::vector<int> res;
    //if ((current_row[sz] < min_cost) && (tree->index)) {
    if ((current_row[sz] < min_cost) && (tree->index.size() != 0)) {
        //res.push_back(tree->index);
        res.insert(res.end(), tree->index.begin(), tree->index.end());
    }

    // If there is an element wich is smaller than the current minimum cost,
    //  we can have another cost smaller than the current minimum cost
    if (*min_element(current_row, current_row + sz + 1) < min_cost) {
        for (int i = 'A'; i < 'A' + MAGIC_NUMBER; ++i) {
            if (tree->next[i]) {
                std::vector<int> tmp = search_impl(tree->next[i], i, current_row, sz, word, min_cost);
                if (tmp.size() > 0) {
                  res.insert(res.end(), tmp.begin(), tmp.end());
                }
            }
        }
    }

    delete [] current_row;

    return res;
}

std::vector<int> search(string word, int min_cost, trie* tree)
{
  word = '[' + word;

    int sz = word.size();

    int *current_row = new int[sz + 1];

    // Naive DP initialization
    for (int i = 0; i < sz + 1; ++i) current_row[i] = i;


    std::vector<int> res;
    // For each letter in the root map wich matches with a
    //  letter in word, we must call the search
    for (int i = 0 ; i < sz; ++i) {
        if (tree->next[word[i]]) {
            std::vector<int> tmp = search_impl(tree->next[word[i]], word[i], current_row, sz, word, min_cost);
            if (tmp.size() > 0) {
              res.insert(res.end(), tmp.begin(), tmp.end());
            }
        }
    }

    delete [] current_row;

    return res;
}

// if patterns is emptry, than use input sequences in trie for search.
// [[Rcpp::export(".levenshtein_search")]]
void bench_trie(size_t mistakes, 
                const vector<string> &first_file_seq, 
                 const vector<string> &second_file_seq, 
                 size_t first_file_size, 
                 size_t second_file_size, 
                 ofstream &outfile)
{

    std::chrono::milliseconds tp1 = std::chrono::duration_cast<std::chrono::milliseconds>(system_clock::now().time_since_epoch());

    // The tree
    trie tree;
    for (int i = 0; i < second_file_size; i++) {
        tree.insert(second_file_seq[i], i + 1);
    }    

    vector<int> tmp;
    for (int j = 0; j < first_file_size; j++)  {
        tmp = search(first_file_seq[j], mistakes, &tree);
        for (int k = 0; k < tmp.size(); k++) {
            outfile << (int) (j+1) << " " << (int) (tmp[k]) << "\n";
        }
         
    }


    std::chrono::milliseconds tp2 = std::chrono::duration_cast<std::chrono::milliseconds>(system_clock::now().time_since_epoch());
    auto diff = tp2.count() - tp1.count();

    cout << "bench trie (" << (size_t) first_file_size << " : " << second_file_size << ") - " << diff << " milsec" << endl;
}
