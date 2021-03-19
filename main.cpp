#include <iostream>
#include <vector>


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
    std::vector<int> Find(string str);
private:
    vector<int> array;
}


int main() {
    ios::sync_with_stdio(0);
    cout.tie(0), cin.tie(0);

    string text, pattern;
    cin >> text;

    TSuffixTree tree(text + "$");
    TArray array(tree);

    while (cin >> pattern){

    }

    return 0;
}