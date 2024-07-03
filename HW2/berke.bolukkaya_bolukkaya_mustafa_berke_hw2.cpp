//MUSTAFA BERKE BÖLÜKKAYA 32683
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;

struct documentItem{
    std::string documentName;
    int count;
};

struct word_se{
    std::vector<documentItem> documents;
    string name;

};

template <class T>
struct wordItem {
    wordItem *left;
    wordItem *right;
    std::vector<documentItem> documentInfo;

    const T value;
    int count;  // how many nodes are there in this subtree
    int height;

    wordItem(T value);
    void updateValues();
    int balanceFactor();

    wordItem* left_rotate();
    wordItem* right_rotate();
};

template <class T>
wordItem<T>::wordItem(T value): value(value){
    count = 1;
    height = 1;

    left = nullptr;
    right = nullptr;
}

template <class T>
void wordItem<T>::updateValues(){
    count = (left != nullptr ? left->count : 0) + (right != nullptr ? right->count : 0) + 1;

    height = std::max(left != nullptr ? left->height : 0,
                      right != nullptr ? right->height : 0) + 1;
}

template <class T>
int wordItem<T>::balanceFactor(){
    return (left != nullptr ? left->height : 0) - (right != nullptr ? right->height: 0);
}

template <class T>
wordItem<T>* wordItem<T>::left_rotate(){
    wordItem* R = right;
    right = right->left;
    R->left = this;

    this->updateValues();  // the order is important
    R->updateValues();

    return R;
}

template <class T>
wordItem<T>* wordItem<T>::right_rotate(){
    wordItem* L = left;
    left = left->right;
    L->right = this;

    this->updateValues();  // the order is important
    L->updateValues();

    return L;
}


template <class T>
class AVLTree {
    int _size;
    wordItem<T> *root;

    void balance(std::vector<wordItem<T>**> path);
    void display(wordItem<T>* cur, int depth=0, int state=0);

public:
    AVLTree();
    ~AVLTree();

    void insert(T value);
    void remove(T value);

    void clear();
    bool empty() const;
    int size() const;

    int find(T value) const;

    wordItem<T>* operator[](std::size_t idx) const;


    void display();
};

template <class T>
AVLTree<T>::AVLTree(){
    root = nullptr;
    _size = 0;
}

template <class T>
AVLTree<T>::~AVLTree(){
    clear();
}

template <class T>
void AVLTree<T>::clear(){
    std::vector<wordItem<T>*> stack;

    if (root != nullptr)
        stack.push_back(root);

    while (!stack.empty()){
        wordItem<T> *node = stack.back();
        stack.pop_back();

        if (node->left != nullptr)
            stack.push_back(node->left);

        if (node->right != nullptr)
            stack.push_back(node->right);

        _size--;
        delete node;
    }

    root = nullptr;
}

template <class T>
void AVLTree<T>::insert(T value){
    wordItem<T> **indirect = &root;  // to generalize insertion
    std::vector<wordItem<T>**> path;  // to update height values

    // Search for the value in the tree
    while (*indirect != nullptr){
        path.push_back(indirect);

        if ((*indirect)->value == value)
            return;  // Value already exists, so return without insertion
        else if ((*indirect)->value > value)
            indirect = &((*indirect)->left);
        else
            indirect = &((*indirect)->right);
    }

    // Value does not exist, so insert it
    *indirect = new wordItem<T>(value);
    path.push_back(indirect);

    // Perform balancing
    balance(path);
    _size++;
}


template <class T>
void AVLTree<T>::remove(T value){
    wordItem<T> **indirect = &root;  // to generalize insertion
    std::vector<wordItem<T>**> path;  // to update height values

    while (*indirect != nullptr and (*indirect)->value != value){
        path.push_back(indirect);

        if ((*indirect)->value > value)
            indirect = &((*indirect)->left);
        else
            indirect = &((*indirect)->right);
    }

    if (*indirect == nullptr)  // the value does not exist in tree
        return;  // may be raising an Exception is more elegant

    else
        path.push_back(indirect);

    std::size_t index = path.size();

    if ((*indirect)->left == nullptr and (*indirect)->right == nullptr){  // the node is leaf
        delete *indirect;  // just delete node
        *indirect = nullptr;
        path.pop_back();  // pop the deleted node from path
    }

    else if ((*indirect)->right == nullptr){  // only left child exists
        wordItem<T> *toRemove = *indirect;

        (*indirect) = (*indirect)->left;
        delete toRemove;

        path.pop_back();
    }

    else{  // right child exists
        wordItem<T> **successor = &((*indirect)->right);

        while ((*successor)->left != nullptr){
            path.push_back(successor);
            successor = &((*successor)->left);
        }

        if (*successor == (*indirect)->right){
            (*successor)->left = (*indirect)->left;

            wordItem<T> *toRemove = *indirect;
            *indirect = *successor;
            delete toRemove;
        }

        else{
            wordItem<T> *tmp = *path.back(), *suc = *successor;

            tmp->left = (*successor)->right;
            suc->left = (*indirect)->left;
            suc->right = (*indirect)->right;

            delete *indirect;
            *indirect = suc;
            path[index] = &(suc->right);
        }
    }

    balance(path);
    _size--;
}

template <class T>
void AVLTree<T>::balance(std::vector<wordItem<T> **> path){  // starting from root
    std::reverse(path.begin(), path.end());

    for (auto indirect: path){
        (*indirect)->updateValues();

        if ((*indirect)->balanceFactor() >= 2 and (*indirect)->left->balanceFactor() >= 1)   // left - left
            *indirect = (*indirect)->right_rotate();

        else if ((*indirect)->balanceFactor() >= 2){  // left - right
            (*indirect)->left = (*indirect)->left->left_rotate();
            *indirect = (*indirect)->right_rotate();
        }

        else if ((*indirect)->balanceFactor() <= -2 and (*indirect)->right->balanceFactor() <= -1)  // right - right
            *indirect = (*indirect)->left_rotate();

        else if ((*indirect)->balanceFactor() <= -2){  // right - left
            (*indirect)->right = ((*indirect)->right)->right_rotate();
            *indirect = (*indirect)->left_rotate();
        }
    }
}

template <class T>
bool AVLTree<T>::empty() const{
    return _size == 0;
}

template <class T>
int AVLTree<T>::size() const{
    return _size;
}

template <class T>
int AVLTree<T>::find(T value) const{
    wordItem<T> *direct = root;  // to generalize insertion
    int idx = 0;

    while (direct != nullptr and direct->value != value){
        if (direct->value > value)
            direct = direct->left;
        else{
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }

    if (direct == nullptr)
        return -1;

    else
        return idx + (direct->left ? direct->left->count : 0);
}


template <class T>
wordItem<T>* AVLTree<T>::operator[](std::size_t idx) const {
    wordItem<T>* cur = root;
    int left = cur->left != nullptr ? cur->left->count : 0;

    while (left != idx) {
        if (left < idx) {
            idx -= left + 1;
            cur = cur->right;
            left = cur->left != nullptr ? cur->left->count : 0;
        } else {
            cur = cur->left;
            left = cur->left != nullptr ? cur->left->count : 0;
        }
    }

    return cur;
}

template <class T>
void AVLTree<T>::display(){
    printf("\n");
    if (root != nullptr)
        display(root);
    else
        printf("Empty");
    printf("\n");
}

template <class T>
void AVLTree<T>::display(wordItem<T> *cur, int depth, int state){  // state: 1 -> left, 2 -> right , 0 -> root
    if (cur->left)
        display(cur->left, depth + 1, 1);

    for (int i=0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("┌───");
    else if (state == 2)  // right
        printf("└───");

    std::cout << "[" << cur->value << "] - (" << cur->count << ", " << cur->height << ")" << std::endl;

    if (cur->right)
        display(cur->right, depth + 1, 2);
}

// Function to check if a string contains only alphabetical characters
bool is_alpha(const std::string& str) {
    for (char c : str) {
        if (!std::isalpha(c)) {
            return false;
        }
    }
    return true;
}



int main() {

    // Get the number of files from the user
    int num_files;
    std::cout << "Enter number of input files: ";
    std::cin >> num_files;

    // Vector of file names and their corresponding words
    std::vector<std::string> file_names(num_files);
    std::vector<std::vector<std::string>> words_per_file(num_files);

    // Get file names from the user
    for (int i = 0; i < num_files; ++i) {
        std::cout << "Enter " << i + 1 <<". file name: " ;
        std::cin >> file_names[i];
    }

    // Iterate over each file
    for (int i = 0; i < num_files; ++i) {
        std::ifstream file(file_names[i]);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << file_names[i] << std::endl;
            continue;
        }

        std::string word;
        while (file >> word) {
            // Convert the word to lowercase
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (is_alpha(word)) {
                words_per_file[i].push_back(word);
            }
        }
    }

    AVLTree<std::string> tree;

    // Print words for each file
    for (int i = 0; i < num_files; ++i) {
        for (const std::string& word : words_per_file[i]) {
            if(tree.find(word) == -1){
                documentItem doc_temp;
                doc_temp.count=1;
                doc_temp.documentName=file_names[i];
                tree.insert(word);
                int temp=tree.find(word);
                tree.operator[](temp)->documentInfo.push_back(doc_temp);
            }
            else{
                bool check=true;
                int temp_else= tree.find(word);

                for (int k=0;k<tree.operator[](temp_else)->documentInfo.size();k++){
                    if(tree.operator[](temp_else)->documentInfo[k].documentName==file_names[i]){
                        tree.operator[](temp_else)->documentInfo[k].count++;
                        check= false;
                    }
                }
                if (check){
                    documentItem doc_t;
                    doc_t.documentName=file_names[i];
                    doc_t.count=1;
                    tree.operator[](temp_else)->documentInfo.push_back(doc_t);

                }
            }
        }
    }
    cout<<endl;
    //TREE IS CREATED

    std::vector<word_se> words_searching;
    bool firstIteration = true;
    while (true){
        bool check_for_fail= true;
        words_searching.clear();
        cout<<"Enter queried words in one line: ";
        bool lastcheck=true;
        string sentence;
        if (firstIteration){
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            firstIteration=false;
        }

        std::getline(std::cin, sentence);
        string query;
        std::istringstream iss(sentence);
        std::vector<std::string> queries;

        while (iss >> query) {
            if(query!="REMOVE" && query!="ENDOFINPUT"){
                std::transform(query.begin(), query.end(), query.begin(), ::tolower);
            }
            queries.push_back(query);
        }

        if(queries[0]=="REMOVE"){
            tree.remove(queries[1]);
            cout<<queries[1]<<" has been REMOVED"<<endl;
            cout<<endl;
        }
        else if (queries[0]=="ENDOFINPUT"){
            return 0;
        }
        else {
            for (int j = 0; j < queries.size(); j++) {

                if (tree.find(queries[j]) != -1) {
                    int ss = tree.find(queries[j]);
                    word_se temp_word;
                    temp_word.name = tree.operator[](ss)->value;

                    temp_word.documents.assign(tree.operator[](ss)->documentInfo.begin(),
                                               tree.operator[](ss)->documentInfo.end());
                    words_searching.push_back(temp_word);
                    //---------------------------------------------
                }
                else {
                    cout << "No document contains the given query" << endl;
                    cout<<endl;
                    lastcheck = false;
                    queries.clear();
                }
            }

            if (lastcheck){
                for (int t = 0; t < file_names.size(); t++) { //changing file name
                    int printable = 0;
                    for (int p = 0; p < words_searching.size(); p++) { // changing searching words
                        for (int b = 0;b < words_searching[p].documents.size(); b++) { //changing searching words doc names
                            if (file_names[t] == words_searching[p].documents[b].documentName) {
                                printable++;
                            }
                        }
                    }

                    if (printable == words_searching.size()&& printable!=0) {
                        cout << "in Document " << file_names[t] << ", ";
                        check_for_fail=false;
                        for (int p = 0; p < words_searching.size(); p++) { // changing searching words
                            cout << words_searching[p].name;
                            for (int b = 0 ;b < words_searching[p].documents.size(); b++) { //changing searching words doc names
                                if (file_names[t] == words_searching[p].documents[b].documentName) {
                                    cout << " found " << words_searching[p].documents[b].count << " times";
                                }
                            }
                            if(p<words_searching.size()-1) {
                                cout << ", ";
                            }
                        }
                        cout << "." << endl;
                        cout<<endl;
                    }
                }
            }
            if(lastcheck && check_for_fail){
                cout << "No document contains the given query" << endl;
                cout<<endl;
            }
        }
    }
}
