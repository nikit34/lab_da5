


int main(void)
{
    string text, pattern;
    cin >> text;

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