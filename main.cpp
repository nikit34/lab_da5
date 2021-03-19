#include <iostream>
#include <vector>
#include <set>
#include <map>


using namespace std;

class TSuffixTree {
public:
    TSuffixTree(string str);
    set<int> Search(const string& sought);

private:
    struct TSuffixNode;
    struct TNodeInfo {
        TNodeInfo() = default;
        TNodeInfo(const string* str, TSuffixNode* link, int info_start, int info_size);
    };
    struct TSuffixNode {
        TSuffixNode() = default;
        TSuffixNode(TSuffixNode* parent);

        map<char, TNodeInfo> links;
        TSuffixNode* parent = nullptr;
        TSuffixNode* suffix_link = nullptr;
    };
    std::string data;
    TSuffixNode *advancement;
    TSuffixNode *root;
};

TSuffixTree::TSuffixTree(string str)
:data(move(str)), advancement(new TSuffixNode), root(new TSuffixNode){
    for (int i = 0; i < this->data.size(); ++i){
        // advancement->links.emplace(data[i], (&data, root, i, 1));
    }
}

set<int> TSuffixTree::Search(const string& sought){
    if (sought.size() > this->data.size())
        return {};

}

class TArray {
public:
    TArray(vector<int> new_index, string new_data);
    std::set<int> Find(string str);

    friend ostream& operator <<(ostream& os, const TArray& array);

private:
    vector<int> index;
    string data;
};

TArray::TArray(vector<int> new_index, string new_data)
: index(move(new_index)), data(move(new_data))
{}

set<int> TArray::Find(string str){
    set<int> result;
    long long left = 0;
    long long right = this->index.size();

    vector<const char*> tmp(this->index.size());
    for (int i = 0; i < this->index.size(); ++i){
        tmp[i] = this->data.data() + (this->index[i] - 1);
    }
    int buffer_left, buffer_right;
    for (int i = 0; i < str.size(); ++i){
        buffer_left = left;
        buffer_right = right;
        left = lower_bound(tmp.begin() + buffer_left, tmp.begin() +  buffer_right, str.data(), Compare(i)) - tmp.begin();
        right = upper_bound(tmp.begin() + buffer_right, tmp.begin() + buffer_right, str.data(), Compare(i)) - tmp.begin();
    }

    for (int j = left; j < right; ++j) {
        result.insert(this->index[j]);
    }
    return result;
}

ostream& operator <<(ostream& os, const TArray& arr) {
    for (int i = 0; i < arr.index.size(); ++i)
        os << arr.index[i] << " ";
    return os;
}

struct Compare {
    Compare(int new_index): comp_index(new_index) {}

    bool operator ()(const char* left_handler, const char* right_handler) {
        return *(left_handler + comp_index) < *(right_handler + comp_index);
    }

private:
    int comp_index;
};

int main() {
    ios::sync_with_stdio(0);
    cout.tie(0), cin.tie(0);

    string text, pattern;
    cin >> text;

    TSuffixTree tree(text + "$");
    TArray array(tree);

    set<int> result;
    int count = 1;
    while (cin >> pattern){
        result = array.Find(pattern);
        if (!result.empty())
            cout << cout << ": " << result << endl;
        ++count;
    }
    return 0;
}