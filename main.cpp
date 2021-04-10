#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>
#include <algorithm>
#include <map>
#include <array>
#include <tuple>

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <algorithm>
#include <chrono>


using namespace std;
using namespace chrono;

class TArray;

class TNode {
public:
    map<char, TNode *> links;
    string::iterator begin, end;
    TNode *suffix_link;
    TNode(string::iterator begin, string::iterator end);
    ~TNode() {};
};

class TSuffixTree {
public:
    TSuffixTree(string str);
    ~TSuffixTree() {};

    friend TArray;

private:
    string text;
    TNode *root;
    int remainder;
    TNode *fake_node, *current_node;
    int current_lenght;
    string::iterator current_point;

    int DistancePoint(TNode *node, string::iterator pos);
    void DeleteTree(TNode *node);
    bool ReversePoint(string::iterator cur_pos, TNode *node);
    void FakeSetup(TNode *node);
    void ExtendPoint(string::iterator point);
    void DFS(TNode *node, vector<int> &result, int deep);
};

class TArray {
public:
    TArray(TSuffixTree& tree);
    vector<int> Find(string& pattern);
    ~TArray() {};

private:
    string text;
    vector<int> array;
};



TSuffixTree::TSuffixTree(string str)
: text(str), root(new TNode(text.end(), text.end())), remainder(0) {
    this->current_point = text.begin();
    this->current_node = this->fake_node = root->suffix_link = root;
    this->current_lenght = 0;

    for (string::iterator it = text.begin(); it != text.end(); ++it)
        this->ExtendPoint(it);
}

TNode::TNode(string::iterator begin, string::iterator end): begin(begin), end(end), suffix_link(0) {}

// struct Compare {
//     Compare(int new_index): comp_index(new_index) {}

//     bool operator ()(const char* left_handler, const char* right_handler) {
//         return *(left_handler + comp_index) < *(right_handler + comp_index);
//     }

// private:
//     int comp_index;
// };

void TSuffixTree::DeleteTree(TNode *node) {
    for (map<char, TNode *>::iterator it = node->links.begin(); it != node->links.end(); ++it)
        this->DeleteTree(it->second);
    delete node;
}

void TSuffixTree::ExtendPoint(string::iterator point) {
    this->fake_node = this->root;
    ++(this->remainder);

    while (this->remainder) {
        if (!this->current_lenght)
            this->current_point = point;

        map<char, TNode *>::iterator it = this->current_node->links.find(*this->current_point);
        TNode *next = (it == this->current_node->links.end()) ? nullptr : it->second;
        if (!next) {
            TNode *leaf = new TNode(point, this->text.end());
            this->current_node->links[*this->current_point] = leaf;
            this->FakeSetup(this->current_node);
        } else {
            if (this->ReversePoint(point, next))
                continue;

            if (*(next->begin + this->current_lenght) == *point) {
                ++(this->current_lenght);
                this->FakeSetup(this->current_node);
                break;
            }

            TNode *split = new TNode(next->begin, next->begin + this->current_lenght);
            TNode *leaf = new TNode(point, this->text.end());
            this->current_node->links[*this->current_point] = split;

            split->links[*point] = leaf;
            next->begin += this->current_lenght;
            split->links[*next->begin] = next;
            this->FakeSetup(split);
        }

        --(this->remainder);
        if (this->current_node == this->root && this->current_lenght) {
            --(this->current_lenght);
            this->current_point = point - this->remainder + 1;
        } else {
            this->current_node = (this->current_node->suffix_link) ? this->current_node->suffix_link : this->root;
        }
    }
}

int TSuffixTree::DistancePoint(TNode *node, string::iterator pos) {
    return min(node->end, pos + 1) - node->begin;
}

bool TSuffixTree::ReversePoint(string::iterator cur_pos, TNode *node) {
    if (this->current_lenght >= this->DistancePoint(node, cur_pos)) {
        this->current_point += this->DistancePoint(node, cur_pos);
        this->current_lenght -= this->DistancePoint(node, cur_pos);
        this->current_node = node;
        return true;
    }
    return false;
}

void TSuffixTree::FakeSetup(TNode *node) {
    if (this->fake_node != this->root)
        this->fake_node->suffix_link = node;
    this->fake_node = node;
}

void TSuffixTree::DFS(TNode *node, vector<int> &result, int deep) {
    if (node->links.empty()) {
        result.push_back(this->text.size() - deep);
        return;
    }
    int tmp;
    for (map<char, TNode *>::iterator it = node->links.begin(); it != node->links.end(); ++it) {
        tmp = deep;
        tmp += it->second->end - it->second->begin;
        this->DFS(it->second, result, tmp);
    }
}

TArray::TArray(TSuffixTree& tree) : text(tree.text), array() {
    tree.DFS(tree.root, array, 0);
    tree.DeleteTree(tree.root);
}

vector<int> TArray::Find(string& pattern) {

    pair<vector<int>::iterator, vector<int>::iterator> range(this->array.begin(), this->array.end());
    for (int i = 0; i < pattern.size() && range.first != range.second; ++i) {
        // возврат левой и правой равной границы
        range = equal_range(range.first, range.second, numeric_limits<int>::max(), [this, &pattern, &i] (int idx1, int idx2) -> bool {
            if (idx1 == numeric_limits<int>::max()) {
                return bool(pattern[i] < this->text[i + idx2]);
            } else {
                return bool(this->text[i + idx1] < pattern[i]);
            }
        });
    }
    vector<int> result(range.first, range.second);
    sort(result.begin(), result.end());

    return result;

    // set<long long> result;
    // long long left = 0;
    // long long right = array.size();
    // vector<const char*> tempArray(array.size());
    // for (size_t i = 0; i < array.size(); ++i) {
    //     tempArray[i] = data.data() + (array[i] - 1);
    // }
    // for (size_t i = 0; i < sub.size(); ++i) {
    //     long long localLeft = left;
    //     long long localRight = right;
    //     left = lower_bound(tempArray.begin() + localLeft, tempArray.begin() + localRight, sub.data(), Compare(i)) - tempArray.begin();
    //     right = upper_bound(tempArray.begin() + localLeft, tempArray.begin() + localRight, sub.data(), Compare(i)) - tempArray.begin();
    // }
    // for (size_t j = left; j < right; ++j) {
    //     result.insert(array[j]);
    // }
    // return result;
}

int main(){
    auto start = high_resolution_clock::now();

    string text, pattern;
    cin >> text;

    // for Castom Find
    text = text + "$";
    TSuffixTree tree(text + "$");
    TArray array(tree);
    vector<int> result;
    int res_size;
    //


    // for std find
    int index;
    //

    for (int i = 1; cin >> pattern; ++i) {

        // for Castom Find
        // result = array.Find(pattern);
        // res_size = result.size();
        // if (!result.empty()) {
        //     cout << i << ": ";
        //     for (int j = 0; j < res_size; ++j) {
        //         cout << result[j] + 1;
        //         if (j < res_size -  1) {
        //             cout << ", ";
        //         }
        //     }
        //     cout << endl;
        // }

        // for std find
        index = 0;
        cout << i << ": ";
        while (true) {
            index = text.find(pattern, index);
            if (index != string::npos) {
                ++index;
                cout << index << ", ";
            } else {
                break;
            }
        }
        cout << endl;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << endl;

    return 0;
}