#include <iostream>
#include <vector>
#include <set>


using namespace std;

class TSuffixTree {
public:
    TSuffixTree(string str);

};

TSuffixTree::TSuffixTree(string str){

}

class TArray{
public:
    TArray(TSuffixTree tree);
    std::set<int> Find(string str);

private:
    vector<int> index;
    string data;
};

set<int> TArray::Find(string str){
    set<int> result;
    long long left = 0;
    long long right = this->index.size();

    vector<const char*> tmp(this->index.size());
    for (size_t i = 0; i < this->index.size(); ++i){
        tmp[i] = this->data.data() + (this->index[i] - 1);
    }
    int buffer_left, buffer_right;
    for (size_t i = 0; i < str.size(); ++i){
        buffer_left = left;
        buffer_right = right;
        left = lower_bound(tmp.begin() + buffer_left, tmp.begin() +  buffer_right, str.data()) - tmp.begin();
        right = upper_bound(tmp.begin() + buffer_right, tmp.begin() + buffer_right, str.data()) - tmp.begin();
    }

    for (size_t j = left; j < right; ++j) {
        result.insert(this->index[j]);
    }
    return result;
}

int main() {
    ios::sync_with_stdio(0);
    cout.tie(0), cin.tie(0);

    string text, pattern;
    cin >> text;

    TSuffixTree tree(text + "$");
    TArray array(tree);

    set<int> result;
    uint32_t count = 1;
    while (cin >> pattern){
        result = array.Find(pattern);
        if (!result.empty())
            cout << cout << ": " << result << endl;
        ++count;
    }
    return 0;
}