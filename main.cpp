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
    TArray(TSuffixTree tree);
    vector<int> Find(string pattern) const;
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
    ++remainder;

    while (remainder) {
        if (!current_lenght)
            current_point = point;

        map<char, TNode *>::iterator it = current_node->links.find(*current_point);
        TNode *next = (it == current_node->links.end()) ? NULL : it->second;
        if (!next) {
            TNode *leaf = new TNode(point, text.end());
            current_node->links[*current_point] = leaf;
            FakeSetup(current_node);
        } else {
            if (ReversePoint(point, next))
                continue;

            if (*(next->begin + current_lenght) == *point) {
                ++current_lenght;
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
        }

        --remainder;
        if (current_node == root && current_lenght) {
            --current_lenght;
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
        current_point += DistancePoint(node, cur_pos);
        current_lenght -= DistancePoint(node, cur_pos);
        current_node = node;
        return true;
    }
    return false;
}

void TSuffixTree::FakeSetup(TNode *node) {
    if (fake_node != root)
        fake_node->suffix_link = node;
    fake_node = node;
}

void TSuffixTree::DFS(TNode *node, vector<int> &result, int deep) {
    if (node->links.empty()) {
        result.push_back(text.size() - deep);
        return;
    }
    int tmp;
    for (map<char, TNode *>::iterator it = node->links.begin(); it != node->links.end(); ++it) {
        tmp = deep;
        tmp += it->second->end - it->second->begin;
        DFS(it->second, result, tmp);
    }
}

TArray::TArray(TSuffixTree tree):text(tree.text), array() {
    tree.DFS(tree.root, array, 0);
    tree.DeleteTree(tree.root);
}

vector<int> TArray::Find(string pattern) {
    pair<vector<int>::iterator, vector<int>::iterator> range(array.begin(), array.end());
    for (int i = 0; i < pattern.size() && range.first != range.second; ++i) {
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

int main(void)
{
    string text, pattern;
    cin >> text;

    TSuffixTree tree(text + "$");
    TArray array(tree);

    for (int cntPattern = 1; cin >> text; ++cntPattern) {
        vector<int> result = array.Find(text);
        if (!result.empty()) {
            cout << cntPattern << ": ";
            for (int i = 0; i < result.size(); ++i) {
                cout << result[i] + 1;
                if (i < result.size() -  1) {
                    cout << ", ";
                }
            }
            cout << '\n';
        }
    }

    return 0;
}