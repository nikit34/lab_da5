#include <iostream>

using namespace std;


int main() {
    ios::sync_with_stdio(0);
    cout.tie(0), cin.tie(0);

    string text, pattern;
    cin >> text;

    TSuffixTree tree(text + "$");
    // text + "$"
    while (cin >> pattern){

    }

    return 0;
}