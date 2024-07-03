#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <cctype>
#include <sstream>
#include <chrono>

using namespace std;

//NEARLY ALL OF BST AND HASH CLASS ARE COPIED FROM LECTURE SLIDES, FUNCTIONS ARE TAKEN FROM THE BST PART, DELETED SOME PARTS AS NOT USING



template <typename T>
class binary_search_tree;

template <typename T>
class bst_node //avlNode class
{
    T 	element;
    bst_node *left = nullptr;
    bst_node *right = nullptr;
    int height;

    bst_node(const T & theElement, bst_node *lt, bst_node *rt, int h = 0 ) : element(theElement ), left(lt ), right(rt ), height(h ) { } //constructor
    bst_node() : element(nullptr), left(nullptr ), right(nullptr ), height(0 ){}
    friend class binary_search_tree<T>;
};

template <typename T>
class binary_search_tree { //our main class
public:


    explicit binary_search_tree(const T &notFound) : ITEM_NOT_FOUND(notFound), root(nullptr){} //explicit constructor from the lecture slides

    binary_search_tree(): root(nullptr) {}


    binary_search_tree(const binary_search_tree<T> & rhs): root(nullptr), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND){ //coppy constructor

        *this = rhs;

    }

    ~binary_search_tree(){
        makeEmpty(root); //destructor for memory

    }

    const binary_search_tree & operator=(const binary_search_tree & rhs ){ // = operator overload

        if (this != &rhs)
        {
            makeEmpty(); //free the memory
            root = rhs.clone(rhs.root);
        }
        return *this;

    }

    bool operator < (const binary_search_tree<T> &rhs) const { //insertion uniqiue one
        return root->element < rhs.root->element;
    }

    const T &find(const T &x) const{ //public find

        return elementAt( find( x, root ) );


    }


    void makeEmpty(){

        makeEmpty( root );


    }

    void insert(const T &x){ //just copied from the lecture slides

        insert( x, root );

    }

    void remove(const T &x){ //just copied from the lecture slides BST part

        remove( x, root );

    }



private:

    string title;
    bst_node<T> *root;
    const T ITEM_NOT_FOUND;

    const T &elementAt(bst_node<T> *t) const{ //copied from the lecture slides

        return t == nullptr ? ITEM_NOT_FOUND : t->element;

    }

    void insert(const T &x, bst_node<T> *&t) const{ //copied from the lecture slides


        if ( t == nullptr )
            t = new bst_node<T>(x, nullptr, nullptr );

        else if ( x < t->element ) {

            insert( x, t->left );

            if ( height( t->left ) - height( t->right ) == 2 )
                if ( x < t->left->element )
                    rotateWithLeftChild( t );
                else
                    doubleWithLeftChild( t );
        }
        else if( t->element < x )
        {    // Otherwise X is inserted to the right subtree
            insert( x, t->right );
            if ( height( t->right ) - height( t->left ) == 2 )
                // height of the right subtree increased
                if ( t->right->element < x )
                    // X was inserted to right-right subtree
                    rotateWithRightChild( t );
                else // X was inserted to right-left subtree
                    doubleWithRightChild( t );
        }
        else
            ;  // Duplicate; do nothing

        // update the height the node
        t->height = max( height( t->left ), height( t->right ) ) + 1;
    }


    void remove(const T &x, bst_node<T> *&t) { //the first part of this function is copied directly from the BST tree function of lecture slides
        if (t == nullptr)
            return; // Item not found; do nothing

        if (x < t->element)
            remove(x, t->left);
        else if (t->element < x)
            remove(x, t->right);
        else if (t->left != nullptr && t->right != nullptr) // Two children
        {
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        }
        else // One or no children
        {
            bst_node<T> *oldNode = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete oldNode;
        }

    }

    bst_node<T> *findMin(bst_node<T> *t) const{ //copied from the slides

        if( t == nullptr )
            return nullptr;
        if( t->left == nullptr )
            return t;
        return findMin( t->left );


    }

    bst_node<T> *find(const T &x, bst_node<T> *t) //copied from the slides
    const{

        if ( t == nullptr )
            return nullptr;
        else if( x < t->element )
            return find( x, t->left );
        else if( t->element < x )
            return find( x, t->right );
        else
            return t;    // Match


    }


    void makeEmpty(bst_node<T> *&t) const{ //copied from the slides

        if( t != nullptr )
        {
            makeEmpty( t->left );
            makeEmpty( t->right );
            delete t;
        }
        t = nullptr;


    }

    void printTree(bst_node<T> *t) const{ //copied from the slides

        if ( t != NULL )
        {
            printTree( t->left );
            cout << t->element << endl;
            printTree( t->right );
        }


    }

    bst_node<T> *clone(bst_node<T> *t) const{ //copied from the slides

        if ( t == NULL )
            return NULL;
        else
            return new bst_node<T>
                    ( t->element,
                      clone( t->left ),
                      clone( t->right ) );


    }

    // Avl manipulations
    int height(bst_node<T> *t) const{ //copied from the slides

        if (t == nullptr)
            return -1;

        return t->height;


    }

    int max(int lhs, int rhs) const{ //copied from the slides

        if (lhs > rhs)
            return lhs;

        return rhs;


    }

    void rotateWithLeftChild(bst_node<T> *&k2) const{ //copied from the slides

        bst_node<T> *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
        k1->height = max( height( k1->left ), k2->height ) + 1;
        k2 = k1;


    }

    void rotateWithRightChild(bst_node<T> *&k1) const{ //copied from the slides

        bst_node<T> *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
        k2->height = max( height( k2->right ), k1->height ) + 1;
        k1 = k2;


    }

    void doubleWithLeftChild(bst_node<T> *&k3) const{ //copied from the slides

        rotateWithRightChild( k3->left );
        rotateWithLeftChild( k3 );


    }

    void doubleWithRightChild(bst_node<T> *&k1) const{ //copied from the slides

        rotateWithLeftChild( k1->right );
        rotateWithRightChild( k1 );


    }
};

//-----------------------------------------------------------------

struct DocumentItem { //to keep track of total count and file name

    string documentName;
    int count;

};

using namespace std;

int nextPrime(int n) {
    if (n <= 1) return 2; // The smallest prime number

    auto isPrime = [](int number) -> bool {
        if (number <= 3) return number > 1;
        if (number % 2 == 0 || number % 3 == 0) return false;
        for (int i = 5; i * i <= number; i += 6) {
            if (number % i == 0 || number % (i + 2) == 0) return false;
        }
        return true;
    };

    // Start from the next number if n is even, otherwise start from n itself
    if (n % 2 == 0) ++n;
    else n += 2;

    // Only check odd numbers
    for (; !isPrime(n); n += 2);

    return n;
}

int hashFunc (const string& key, int tableSize) //copied from the lecture slides, changed the value
{
    int hashVal = 0;

    for (char i : key)
        hashVal = 47 * hashVal + i;

    hashVal = hashVal % tableSize;

    if (hashVal < 0) //for checking negative values
        hashVal = hashVal + tableSize;

    return(hashVal);
}

template <class HashedObj>
class HashTable
{
public:
    explicit HashTable( const HashedObj & notFound, int size = 79 ); //constructor

    HashTable( const HashTable & rhs ) //copy constructor
            : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
              array( rhs.array ), currentSize( rhs.currentSize ) { }

    const HashedObj & find( const HashedObj & x ) const{ //public find method, gives the element on the wanted position
        int currentPos = findPos( x );

        if (isActive( currentPos ))
            return array[ currentPos ].element;

        return ITEM_NOT_FOUND;
    }


    HashedObj & update(const HashedObj & x, int j){ //updating the count
        int position = findPos(x);

        if (array[position].element == x) {
            array[position].element.documentList[j].count++;
        }
        return array[position].element;

    }

    void update2(HashedObj& wordItem, const std::string& documentName) { //updating the count
        int position = findPos(wordItem);

        if (array[position].element == wordItem) {

            DocumentItem newDocumentItem;
            newDocumentItem.documentName = documentName;
            newDocumentItem.count = 1;

            array[position].element.documentList.push_back(newDocumentItem);
        } else {

        }
    }
    void makeEmpty( ){ //makes all the cells empty

        for( int i = 0; i < array.size( ); i++ )
            array[ i ].info = EMPTY;

        currentSize = 0;

    }
    int wordCountHash(){ //returns the current size

        return currentSize;
    }

    double loadRatioHash(){ //returns the load ratio

        float loadFactor= 1.0 * currentSize/array.size();

        return loadFactor;
    }


    void insert( const HashedObj & x ){ //insert method

        // Insert x
        int currentPos = findPos( x );
        if ( isActive( currentPos ) )
            return;
        array[ currentPos ] = HashEntry( x, ACTIVE );

        // make it bigger
        if ( ++currentSize >= array.size( ) * 0.55 ) {
            rehash();
        }
    }


    const HashTable & operator=( const HashTable & rhs ){ //operator overload for the table to work correctly

        if (this != &rhs) {
            array = rhs.array;
            currentSize = rhs.currentSize;
        }
        return *this;

    }

    enum EntryType { ACTIVE, EMPTY };

private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( HashedObj  e = HashedObj( ),EntryType i = EMPTY ): element(std::move( e )), info( i ) {}


    };



    vector<HashEntry> array;
    int currentSize;
    const HashedObj ITEM_NOT_FOUND;

    int findPos( const HashedObj & x ) const{ //location finding

        int collisionNum = 0;
        int currentPos = hashFunc(x.word, array.size());
        while ( array[ currentPos ].info != EMPTY &&
                array[ currentPos ].element != x )
        {
            currentPos += pow(++collisionNum, 2) ;

            if ( currentPos >= array.size( ) )
                currentPos -= array.size( );
        }
        return currentPos;
    }

    bool isActive( int currentPos ) const{ //check if empty

        return array[ currentPos ].info == ACTIVE;

    }

    void rehash();
};

template<class HashedObj>
void HashTable<HashedObj>::rehash() { //rehashing

    {
        vector<HashEntry> oldArray = array;
        int sizeofarray = oldArray.size();

        // Create triple-sized, empty table
        array.resize( nextPrime( 3 * sizeofarray ) );
        for ( int j = 0; j < array.size( ); j++ )
            array[ j ].info = EMPTY;

        // cc table
        currentSize = 0;
        for ( int i = 0; i < sizeofarray; i++ ){

            if ( oldArray[ i ].info == ACTIVE ){
                insert( oldArray[ i ].element );
            }
        }
        float loadFactor= 1.0 * currentSize/array.size();
        cout<<"rehashed..."<<endl;
        cout<<"previous table size:"<<sizeofarray<<", new table size: "<<array.size()<<", current unique word count "<<currentSize<<", current load factor: "<<loadFactor<<endl;
    }
}

template<class HashedObj> //the constructor
HashTable<HashedObj>::HashTable(const HashedObj &notFound, int size): array(nextPrime(size)), ITEM_NOT_FOUND(notFound) {
    makeEmpty();
}

struct WordItem { //WordItem struct to hold each word in the tree,
    string word;
    vector<DocumentItem> documentList;
    WordItem() = default; //default constructor

    WordItem(string newWord, string docName) : word(std::move(newWord)), documentList(1){//paramaterized constructor

        documentList[0].documentName = std::move(docName);
        documentList[0].count = 1;
    }

};

void correctString(string &theWord) { //function to delete non-alphabetical and make all characters lowercase
    for (char & i : theWord) {
        if (isalpha(i)) {
            i = tolower(i); // converts uppercase to lowercase
        } else {
            i = ' '; // Treat non-alphabetic characters as separators
        }
    }
}

bool operator<(const WordItem& lhs, const WordItem& rhs) { //the operator overload for the < to work in methods like find and insert
    return lhs.word < rhs.word;
}

bool operator == (const WordItem& lhs, const WordItem& rhs) { //the operator overload for the == to work in methods like find and insert
    return lhs.word == rhs.word;
}


bool operator != (const WordItem& lhs, const WordItem& rhs) { //the operator overload for the != to work in methods like find and insert
    return lhs.word != rhs.word;
}

int main() {
    int k = 100;
    int fileAmount;
    cout<<"Enter number of input files: ";
    cin>>fileAmount;

    vector<string> fileNames; //i am holding all the fileNames in a vector of strings.

    for (int j = 0; j < fileAmount; j++) {

        string fileName;
        cout << "Enter " << j + 1 << ". file name: ";
        cin >> fileName;
        cin.ignore();
        fileNames.push_back(fileName); //pushing each fileName into the fileNames vector.

    }

    //binary search tree starts

    binary_search_tree<WordItem> BSTREE(WordItem("", {}));//forming a blank bst tree

    for (const auto & fileName : fileNames) {

        ifstream file(fileName);

        string dummy; //for the each word of the document
        string test_dummy;
        while (getline(file, dummy)) {
            correctString(dummy); //making each word lowercase and discarding non-alphabetic characters.
            istringstream ss(dummy);
            while (ss >> test_dummy) {

                WordItem newItem(test_dummy, fileName); //forming a new WordItem object, the word is test_dummy
                WordItem foundWord = BSTREE.find(WordItem{test_dummy, {}});
                if (test_dummy != foundWord.word) { //unless it can't be found
                    DocumentItem doc; //forming a new DocumentItem object for the new word.
                    doc.count = 1; //making its count 1.
                    doc.documentName = fileName;

                    newItem.documentList.push_back(doc); //pushing back.

                    BSTREE.insert(newItem); //inserting
                }
                else {

                    bool DocumentIsNotExisting = false; //suppose that the word exist but on the same file that we are on.

                    for (auto & j : foundWord.documentList) {//iterating that words possible documents

                        if (j.documentName == fileName) { //if the document of the word is the same as the document we are currently iterating
                            DocumentIsNotExisting = true; //now we know that we are on the same document
                            j.count++; //if so we just need to update its count
                        }

                    }
                    if (!DocumentIsNotExisting) { //if the word exists but not on our file,
                        DocumentItem newDocumentItemOfTheWord; //we need to push another documentItem object to the WordItem, so we create another one.
                        newDocumentItemOfTheWord.documentName = fileName; //its file is the file that we have found,
                        newDocumentItemOfTheWord.count = 1; //and again its count is one
                        foundWord.documentList.push_back(newDocumentItemOfTheWord); //pushing back to foundWord

                    }

                    BSTREE.remove(foundWord); // remove the existing word
                    BSTREE.insert(foundWord); // insert the updated word back

                }
            }
        }
        file.close();
    }
    //Hash table starts
    HashTable<WordItem> myTable(WordItem("",{})); //constructing the hash table
    for (const auto & fileName : fileNames) {
        ifstream file(fileName);
        string line; //for every word in the doc
        string word;
        while (getline(file, line)) {
            correctString(line);
            istringstream ss(line);
            while (ss>>word) {

                WordItem WordItemObject; //control input
                WordItemObject.word = word;

                DocumentItem DocumentItemObject; //store the current
                DocumentItemObject.documentName = fileName;

                WordItem foundItemHash = myTable.find(WordItemObject);

                if (foundItemHash == WordItem("", {})) { //if the word is not present

                    DocumentItemObject.count = 1;

                    WordItemObject.documentList.push_back(DocumentItemObject);

                    myTable.insert(WordItemObject);

                }

                else { //if the word is present

                    bool DocumentIsNotExisting = false; //file name checker

                    for (int j = 0; j < foundItemHash.documentList.size(); j++) {//iterating word on other docs

                        if (foundItemHash.documentList[j].documentName == fileName) {
                            DocumentIsNotExisting = true; //now we know that we are on the same document
                            foundItemHash.documentList[j].count++; //if so we just need to update its count
                            myTable.update(foundItemHash,j);

                        }
                    }
                    if (!DocumentIsNotExisting) { //if the word exists but not on our file,
                        myTable.update2(WordItemObject,fileName);

                    }

                }
            }
        }
        file.close();
    }
    cout<<endl;
    cout<<"After preprocessing, the unique word count is "<< myTable.wordCountHash()<<". Current load ratio is "<<myTable.loadRatioHash()<<endl;
    cout << "Enter queried words in one line: ";
    string line;
    getline(cin, line); //getline
    istringstream iss(line);
    string word;
    vector<string> quarryWords; //vector for every words
    int count = 0; //to count the num of word
    while (iss >> word) {
        correctString(word);
        stringstream wordStream(word);
        string segment;
        while (getline(wordStream, segment, ' ')) { //parsing the line by the blanks
            if (!segment.empty()) {
                quarryWords.push_back(segment);
                count++;
            }
        }
    }
    auto start = std::chrono::high_resolution_clock::now(); //timer for bst
    for (int i = 0; i < k; i++)
    {
        for (int y = 0; y < fileNames.size(); ++y) { //bst query

            int countForCheck = 0; //a counter to check if the all query words are in the same file we are currently on

            for (int z = 0; z < count ; ++z) {

                WordItem WordItemObjectToFind; //forming a word item object
                WordItemObjectToFind.word = quarryWords[z];

                DocumentItem DocumentItemObjectToFind; //forming a document item object
                DocumentItemObjectToFind.documentName = fileNames[y];

                WordItem foundItemBSTForQuery = BSTREE.find(WordItemObjectToFind); //finding the word without a file

                for (const auto & docItem : foundItemBSTForQuery.documentList) {

                    //creating a document item object and assign it to the document list of the wanted word
                    if (docItem.documentName == fileNames[y]) { //if the files are the same
                        countForCheck++; //increment the count for one
                        break;
                    }
                }
            }

            if ((count != countForCheck) &&  y==0){ //no document contains the words

                if(i==0){ //to make it print only for once
                    cout<<"No document contains the given query"<<endl;
                }
            }

            else if(count == 1 && countForCheck == 1 ){ //we have only one word and document contains the query

                WordItem WordItemObjectToFind; //again forming a worditem to find the word in the tree
                WordItemObjectToFind.word = quarryWords[i];

                DocumentItem DocumentItemObjectToFind;
                DocumentItemObjectToFind.documentName = fileNames[y];

                WordItem foundItemBSTForQuery = BSTREE.find(WordItemObjectToFind);

                if(i==0){
                    cout<<"in Document "<<fileNames[y]<<", "<< foundItemBSTForQuery.word<<" found "<<foundItemBSTForQuery.documentList[y+1].count<<" times."<<endl; //the print formation
                }


            }
            else if(count > 1 && countForCheck > 1 &&  count == countForCheck){ //if there are more than 1 word and if the document contains all of them

                int ifInSameDocument = 0;

                if(i==0){
                    cout<<"in Document "<<fileNames[y]<<", "; //prints the document name
                }

                for (int j = 0; j < quarryWords.size() ; ++j) {

                    WordItem WordItemObjectToFind; //creating a word item object to search
                    WordItemObjectToFind.word = quarryWords[j];

                    WordItem foundItemBSTForQuery = BSTREE.find(WordItemObjectToFind);



                    for (auto & l : foundItemBSTForQuery.documentList) { //this counter is created before but is not used, it holds the data of the words that are on the same document

                        if (l.documentName == fileNames[y] ){
                            ifInSameDocument++;
                        }

                    }
                    if(i==0){
                        cout<<foundItemBSTForQuery.word<<" found "<<foundItemBSTForQuery.documentList[y+1].count<<" times"; //output formation
                    }


                    if (j < quarryWords.size() -1){
                        if(i==0){
                            cout<<", ";
                        }

                    }
                    else
                    if(i==0){
                        cout<<".";
                    }
                }
                if (i==0){
                    cout<<endl;
                }

            }

        }

    }

    auto binary_time = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now(); //timer for hash
    for (int i = 0; i < k; i++)
    {
        for (int y = 0; y < fileNames.size(); ++y) { //hash query

            int counter_checker = 0; //a counter to check if the all query words are in the same file we are currently on

            for (int q = 0; q < count ; ++q) {

                WordItem WordItemObjectToFind; //forming a word item object
                WordItemObjectToFind.word = quarryWords[q];

                DocumentItem DocumentItemObjectToFind; //forming a document item object
                DocumentItemObjectToFind.documentName = fileNames[y];

                WordItem foundItemHashForQuery = myTable.find(WordItemObjectToFind); //finding the word without a file

                if ( fileNames[y] == foundItemHashForQuery.documentList[y].documentName){ //if the files are the same
                    counter_checker++; //increment the count by one
                }

            }

            if ((count != counter_checker) && y == 0){ //no document contains the words
                if(i==0){ //to print only once
                    cout<<"No document contains the given query"<<endl;
                }
            }
            else if(count == 1 && counter_checker == 1 ){
                WordItem word_struct_looking_for; //creating a word item object
                word_struct_looking_for.word = quarryWords[i];
                DocumentItem DocumentItemObjectToFind;
                DocumentItemObjectToFind.documentName = fileNames[y];
                WordItem foundItemHashForQuery = myTable.find(word_struct_looking_for);
                if(i==0){
                    cout<<"in Document "<<fileNames[y]<<", "<< foundItemHashForQuery.word<<" found "<<foundItemHashForQuery.documentList[y].count<<" times."<<endl;
                }

            }
            else if(count > 1 && counter_checker > 1 && count == counter_checker){
                int ifInSameDocument = 0;
                if(i==0){
                    cout<<"in Document "<<fileNames[y]<<", ";
                }
                for (int j = 0; j < quarryWords.size() ; ++j) {
                    WordItem WordItemObjectToFind; //creating a worditem object
                    WordItemObjectToFind.word = quarryWords[j];
                    WordItem foundItemHashForQuery = myTable.find(WordItemObjectToFind);
                    for (auto & l : foundItemHashForQuery.documentList) {

                        if (l.documentName == fileNames[y] ){
                            ifInSameDocument++;
                        }
                    }
                    if(i==0){
                        cout<<foundItemHashForQuery.word<<" found "<<foundItemHashForQuery.documentList[y].count<<" times"; //output formation
                    }
                    if (j < quarryWords.size() -1){
                        if(i==0){
                            cout<<", ";
                        }
                    }
                    else
                    if(i==0){
                        cout<<".";
                    }
                }
                if(i==0){
                    cout<<endl;
                }
            }
        }
    }
    auto hash_time = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);
    cout << "\nTime: " << binary_time.count() / k << "\n"; //time of bst
    cout << "\nTime: " << hash_time.count() / k << "\n"; //time of hash

    cout<<"Speed Up: "<< (binary_time.count() * 1.0) / (hash_time.count() * 1.0); //the speed up time

    return 0;
}

