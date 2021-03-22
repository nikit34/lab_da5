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

        bool CheckNextSymbol(char s);
        bool OnInfo();
        TNodeInfo GoDown(std::pair<int, int> arc);
        pair<int, int> AddNode(int index, TSuffixNode*& preview_created);


        TSuffixNode* link = nullptr;
        int info_size;
        int info_start;
        const string* str;
    };
    struct TSuffixNode {
        TSuffixNode() = default;
        TSuffixNode(TSuffixNode* parent);

        map<char, TNodeInfo> links;
        TSuffixNode* parent = nullptr;
        TSuffixNode* suffix_link = nullptr;
    };
    void FindOccurrencies(TSuffixNode* node, set<int>& set) const;

    std::string data;
    TSuffixNode *advancement;
    map<TSuffixNode*, int> numeration;
    TSuffixNode *root;
};

TSuffixTree::TNodeInfo::TNodeInfo(const string* str, TSuffixNode* link, int info_start, int info_size)
: link(link), info_start(info_start), info_size(info_size), str(str){
    if(link == nullptr)
        throw logic_error("Suffix Node is not exists");
}

TSuffixTree::TSuffixTree(string str)
:data(move(str)), advancement(new TSuffixNode), root(new TSuffixNode){
    for (int i = 0; i < this->data.size(); ++i){
        advancement->links.insert(make_pair(data[i], TNodeInfo(&data, root, i, 1)));
    }
    this->root->suffix_link = advancement;
    TNodeInfo position = TNodeInfo(&data, root, -1, 0);
    TSuffixNode* preview_created = nullptr;

    char symbol;
    for(int i = 0; i < this->data.size(); ++i){
        symbol = data[i];
        while(!position.CheckNextSymbol(symbol)){
            pair<int, int> arc = position.AddNode(i, preview_created);
        }
    }
}

bool TSuffixTree::TNodeInfo::OnInfo(){
    return this->info_size;
}

bool TSuffixTree::TNodeInfo::CheckNextSymbol(char symbol){
    if(this->OnInfo())
        return (*str)[info_start + info_size] == symbol;
    else
        return this->link->links.count(symbol) != 0;
}

set<int> TSuffixTree::Search(const string& sought){
    if (sought.size() > this->data.size())
        return {};
    set<int> result;
    TSuffixNode* current_node = root;
    int current_index = 0;
    char symbol;
    while(current_index < sought.size()){
        symbol = sought[current_index];
        if(current_node->links.count(sought[current_index]) == 0)
            return {};
        for (int i = 0; i < min<int>(sought.size() - current_index, current_node->links.at(symbol).info_size); ++i)
            if (sought[current_index + i] != data[current_node->links.at(symbol).info_start + i])
                return {};
        current_index += min<int>(sought.size() - current_index, current_node->links.at(symbol).info_size);
        current_node = current_node->links.at(symbol).link;
    }
    FindOccurrencies(current_node, result);
    return result;
}

void TSuffixTree::FindOccurrencies(TSuffixNode* node, set<int>& set) const {
    if (node->links.empty()) {
        set.insert(numeration.at(node));
        return;
    }
    for (int i = 0; i < node->links.size(); ++i) {
        FindOccurrencies(node->links[i].link, set);
    }
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

TSuffixTree::TNodeInfo TSuffixTree::TNodeInfo::GoDown(pair<int, int> arc){

}

pair<int, int> TSuffixTree::TNodeInfo::AddNode(int index, TSuffixNode*& preview_created){
    if(info_size == 0){
        if(link->links.count((*str)[index]) == 1)
            throw logic_error("replacing of exist leaf");
        else
            this->link->links.insert(make_pair((*str)[index], TNodeInfo(str, new TSuffixNode(link), index, str->size() - index)));
        return {0, 0};
    } else {
        pair<int, int> first_info = make_pair(info_start, info_size);
        pair<int, int> second_info = make_pair(info_start + info_size, link->links.at((*str)[info_start]).info_size - info_size);
        TSuffixNode* new_node = new TSuffixNode(link);
        TSuffixNode* child = link->links.at((*str)[info_start]).link;
        child->parent = new_node;
        link->links.at((*str)[info_start]) = TNodeInfo(str, new_node, first_info.first, first_info.second);
        new_node->links.insert(make_pair((*str)[second_info.first], TNodeInfo(str, child, second_info.first, second_info.second)));
        new_node->links.insert(make_pair((*str)[index], TNodeInfo(str, new TSuffixNode(new_node), index, (int)str->size() - index)));
        if (preview_created != nullptr && preview_created->suffix_link == nullptr) {
            preview_created->suffix_link = new_node;
        }
        preview_created = new_node;
        return first_info;
    }
}


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