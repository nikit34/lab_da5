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


using namespace std;


class TNode {
public:
    map<char, TNode *> links;
    string::iterator begin, end;
    TNode *suffix_link;
    TNode(string::iterator begin, string::iterator end);
};



class TSuffixTree {
public:
    TSuffixTree(string str);
    string text;
    TNode *root;
    ~TSuffixTree() { DeleteTree(this->root); }

private:
    int remainder;
    TNode *fake_node, *current_node;
    int current_lenght;
    string::iterator current_point;

    int DistancePoint(TNode *node, string::iterator pos);
    bool ReversePoint(string::iterator cur_pos, TNode *node);
    void FakeSetup(TNode *node);
    void ExtendPoint(string::iterator point);
    void DeleteTree(TNode *node);
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
    current_point = text.begin();
    current_node = fake_node = root->suffix_link = root;
    current_lenght = 0;

    for (string::iterator it = text.begin(); it != text.end(); ++it)
        ExtendPoint(it);
}

TNode::TNode(string::iterator begin, string::iterator end): begin(begin), end(end), suffix_link(0) {}

void TSuffixTree::DeleteTree(TNode *node) {
    for (map<char, TNode *>::iterator it = node->links.begin(); it != node->links.end(); ++it)
        DeleteTree(it->second);
    delete node;
}

void TSuffixTree::ExtendPoint(string::iterator point) {
    fake_node = root;
    ++(remainder);

    while (remainder) {
        if (!current_lenght)
            current_point = point;

        map<char, TNode *>::iterator it = current_node->links.find(*current_point);
        TNode *next = (it == current_node->links.end()) ? nullptr : it->second;
        if (bool(next)) {
            if (ReversePoint(point, next))
                continue;

            if (*(next->begin + current_lenght) == *point) {
                ++(current_lenght);
                FakeSetup(current_node);
                break;
            }

            TNode *split = new TNode(next->begin, next->begin + current_lenght);
            TNode *leaf = new TNode(point, text.end());
            current_node->links[*current_point] = split;

            split->links[*point] = leaf;
            next->begin += current_lenght;
            split->links[*next->begin] = next;
            FakeSetup(split);
        } else {
            TNode *leaf = new TNode(point, text.end());
            current_node->links[*current_point] = leaf;
            FakeSetup(current_node);
        }

        --(remainder);
        if (current_node == root && current_lenght) {
            --(current_lenght);
            current_point = point - remainder + 1;
        } else {
            current_node = (current_node->suffix_link) ? current_node->suffix_link : root;
        }
    }
}

int TSuffixTree::DistancePoint(TNode *node, string::iterator pos) {
    return min(node->end, pos + 1) - node->begin;
}

bool TSuffixTree::ReversePoint(string::iterator cur_pos, TNode *node) {
    if (current_lenght >= DistancePoint(node, cur_pos)) {
        current_node = node;
        current_point += DistancePoint(node, cur_pos);
        current_lenght -= DistancePoint(node, cur_pos);
        return true;
    }
    return false;
}

void TSuffixTree::FakeSetup(TNode *node) {
    if (fake_node != root)
        fake_node->suffix_link = node;
    fake_node = node;
}

void DFS(string& text, TNode *node, vector<int> &result, int deep) {
    if (!node->links.empty()) {
        int tmp;
        for (map<char, TNode *>::iterator it = node->links.begin(); it != node->links.end(); ++it) {
            tmp = it->second->end - it->second->begin;
            tmp += deep;
            DFS(text, it->second, result, tmp);
        }
        return;
    }
    result.push_back(text.size() - deep);
}

TArray::TArray(TSuffixTree& tree) : text(tree.text), array() {
    DFS(text, tree.root, array, 0);
}

vector<int> TArray::Find(string& pattern) {
    pair<vector<int>::iterator, vector<int>::iterator> range(array.begin(), array.end());
    for (int i = 0; i < pattern.size() && range.first != range.second; ++i) {
        // возврат левой и правой равной границы
        range = equal_range(range.first, range.second, numeric_limits<int>::max(), [this, &pattern, &i] (int idx1, int idx2) -> bool {
            if (idx1 == numeric_limits<int>::max()) {
                return bool(pattern[i] < text[i + idx2]);
            } else {
                return bool(text[i + idx1] < pattern[i]);
            }
        });
    }
    vector<int> result(range.first, range.second);
    sort(result.begin(), result.end());

    return result;
}

int main(){
    string text, pattern;
    cin >> text;

    text = text + "$";
    TSuffixTree tree(text);
    TArray array(tree);

    vector<int> result;
    int res_size;
    for (int i = 1; cin >> text; ++i) {
        result = array.Find(text);
        res_size = result.size();
        if (!result.empty()) {
            cout << i << ": ";
            for (int j = 0; j < res_size; ++j) {
                cout << result[j] + 1;
                if (j < res_size -  1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
    }
    return 0;
}