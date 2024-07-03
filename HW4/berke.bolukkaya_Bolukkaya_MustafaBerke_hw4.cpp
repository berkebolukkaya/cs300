// Mustafa Berke Bölükkaya 32683
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cctype>
#include <cstdlib>
#include <cassert>
#include <sstream>


using namespace std;

// THESE ARE TAKEN FROM UTIL CLASS
void ToUpper(string & s)
// postcondition: s all lower case
{
    int len = s.length();
    for(int k=0; k < len; k++)
    {
        s[k] = toupper(s[k]);
    }
}

string UpperString(const string & s)
{
    string word = s;
    ToUpper(word);
    return word;
}


int atoi(const string & s)
// precondition: s is a sequence of digits
// postcondition: returns integer equivalent of s
// exception: if s is not valid integer, 0 is returned
{
    return atoi(s.c_str());
}

double atof(const string & s)
// precondition: s is a sequence of digits
// postcondition: returns double equivalent of s
// exception: if s is not valid double, 0 is returned
{
    return atof(s.c_str());
}

struct contact_struct{
    string name, surname, phone_no, country;

    contact_struct()= default;

    contact_struct(std::string n, std::string s, std::string num, std::string c)
    : name(std::move(n)), surname(std::move(s)), phone_no(std::move(num)), country(std::move(c)) {
    }

    bool operator==(const contact_struct& c) const{
        return (name == c.name && surname == c.surname && phone_no == c.phone_no && country == c.country);
    }

    bool operator!=(const contact_struct& c) const{
        return (phone_no != c.phone_no);
    }

    bool operator<(const contact_struct& c) const{
        if(name == c.name){
            return (surname<c.surname);
        }
        else{
            return (name < c.name);
        }
    }

    bool operator<=(const contact_struct& c) const{
        if(name == c.name){
            return (surname<c.surname);
        }
        else{
            return (name < c.name);
        }
    }

    bool operator>(const contact_struct& c) const{
        if(name == c.name){
            return (surname>c.surname);
        }
        else{
            return (name > c.name);
        }
    }
};

template <class Comparable>
inline void swapping(Comparable & obj1, Comparable & obj2 ){
    Comparable tmp = obj1;
    obj1 = obj2;
    obj2 = tmp;
}


template <class Comparable>
void insertionSort (vector<Comparable>& a){
    int j;
    for (unsigned int p = 1;  p < a.size(); p++){ // loop over the passes
        Comparable tmp = a[p];
        for (j = p; j > 0 && tmp < a[j-1]; j--) // loop over the elements
            a[j] = a[j-1];
        a[j] = tmp;
    }
}

/////////////////////////////////////////////////////////////////////////////////

inline int next_member(int i){
    return 2*i+1;
}

template <class Comparable>
// for deleteMax
void iterating_before_elems(vector<Comparable> & a, int i, int n ){
    int child;
    Comparable tmp;

    for (tmp=a[i]; next_member(i) < n; i = child ){
        child = next_member(i);
        if ( child != n-1 && a[ child  ] < a[ child+1 ] )
            child++;
        if ( a[child ] > tmp )
            a[i] = a[ child ];
        else
            break;
    }
    a[ i ] = tmp;
}

template <class Comparable>
void heapsort(vector<Comparable> & a){
    // buildHeap
    for (int i = a.size()/2; i >=0; i--)
        iterating_before_elems(a, i, a.size());

    // sort
    for (int j = a.size()-1; j >0; j--){
        swapping(a[0], a[j]);    // swap max to the last pos.
        iterating_before_elems(a, 0, j); // re-form the heap
    }
}

/////////////////////////////////////////////////////////////////////////////////

template <class Comparable>
void merge( vector<Comparable> & a, vector<Comparable> & tmpArray, int leftPos, int rightPos, int rightEnd ){
    int leftEnd = rightPos - 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;

    while ( leftPos <= leftEnd && rightPos <= rightEnd ) // Main loop
        if ( a[ leftPos ] <= a[ rightPos ] )
            tmpArray[ tmpPos++ ] = a[ leftPos++ ];
        else
            tmpArray[ tmpPos++ ] = a[ rightPos++ ];

    while ( leftPos <= leftEnd ) // Copy rest of first half
        tmpArray[ tmpPos++ ] = a[ leftPos++ ];

    while ( rightPos <= rightEnd ) // Copy rest of right half
        tmpArray[ tmpPos++ ] = a[ rightPos++ ];

    for ( int i = 0; i < numElements; i++, rightEnd-- ) // Copy tmpArray back
        a[ rightEnd ] = tmpArray[ rightEnd ];
}

template <class Comparable>
void mergeSort( vector<Comparable> & a, vector<Comparable> & tmpArray, int left, int right ){
    if ( left < right ){
        int center = ( left + right ) / 2;
        mergeSort( a, tmpArray, left, center );
        mergeSort( a, tmpArray, center + 1, right );
        merge( a, tmpArray, left, center + 1, right );
    }
}

template <class Comparable>
void helper_merge_sort(vector<Comparable> & a ){
    vector<Comparable> tmpArray( a.size( ) );
    mergeSort( a, tmpArray, 0, a.size( ) - 1 );
}

/////////////////////////////////////////////////////////////////////////////////

template <class Comparable>
void qInsertionSort( vector<Comparable> & a, int left, int right ){
    for ( int p = left + 1; p <= right; p++ ){
        Comparable tmp = a[ p ];
        int j;

        for ( j = p; j > left && tmp < a[j-1]; j-- )
            a[j] = a[j-1];
        a[j] = tmp;
    }
}

template <class Comparable>
const Comparable & middle_one(vector<Comparable> & a, int left, int right ){
    int center = ( left + right ) / 2;
    if ( a[ center ] < a[ left ] )
        swapping(a[left], a[center]);
    if ( a[ right ] < a[ left ] )
        swapping(a[left], a[right]);
    if ( a[ right ] < a[ center ] )
        swapping(a[center], a[right]);

    swapping(a[center], a[right - 1]); // Place pivot at position right - 1
    return a[ right - 1 ];
}

template <class Comparable>
void quicksort( vector<Comparable> & a, int left, int right ){

    if ( left + 10 <= right ){
        Comparable pivot = middle_one(a, left, right);
        // Begin partitioning
        int i = left, j = right - 1;
        for ( ; ; )
        {
            while ( a[ ++i ] < pivot ) { }

            while ( pivot < a[ --j ] ) { }

            if ( i < j )
                swapping(a[i], a[j]);
            else
                break;
        }

        swapping(a[i], a[right - 1]); // keeping the pivot elem

        quicksort( a, left, i - 1 ); // sorting first part
        quicksort( a, i + 1, right ); // sorting second part
    }
    else // sorting the subarray
        qInsertionSort( a, left, right );
}

template <class Comparable>
void helper_quick_sort(vector<Comparable> & a ){
    quicksort( a, 0, a.size( ) - 1 );
}

vector<contact_struct> binarySearch(const vector<contact_struct> & mainVec, const string& queryWord){
    vector<contact_struct> includedContacts;
    string name, surname;
    if(queryWord.find(' ') != string::npos){ //full name search
        istringstream ss(queryWord);
        ss >> name >> surname;
        int low = 0; // starting
        int high = mainVec.size()-1; //ending
        int mid;
        while (low <= high){
            mid = (low + high)/2;
            if (mainVec[mid].name == name && mainVec[mid].surname == surname){ // found elem
                contact_struct foundedContact(name, surname, mainVec[mid].phone_no, mainVec[mid].country);
                includedContacts.push_back(foundedContact);
                return includedContacts;
            }
            else if (mainVec[mid].name < name) // elem for higher part
                low = mid + 1;
            else // elem for lower part
                high = mid - 1;
        }
    }
    else{ //partial name searching
        int low = 0; // starting
        int high = mainVec.size()-1; // ending
        int mid;
        while (low <= high){
            mid = (low + high)/2;
            if (mainVec[mid].name.find(queryWord) != string::npos){
                contact_struct foundedContact(mainVec[mid].name, mainVec[mid].surname, mainVec[mid].phone_no, mainVec[mid].country);
                includedContacts.push_back(foundedContact);
                int backIndex = mid;
                while(backIndex >= 1 && mainVec[backIndex-1].name.find(queryWord) != string::npos){
                    contact_struct exist_contact(mainVec[backIndex - 1].name, mainVec[backIndex - 1].surname, mainVec[backIndex - 1].phone_no, mainVec[backIndex - 1].country);
                    includedContacts.push_back(exist_contact);
                    backIndex--;
                }

                unsigned int frontIndex = mid;
                while(frontIndex < mainVec.size()-1 && mainVec[frontIndex+1].name.find(queryWord) != string::npos){ //checking for other possible wanted elems
                    contact_struct exist_contact(mainVec[frontIndex + 1].name, mainVec[frontIndex + 1].surname, mainVec[frontIndex + 1].phone_no, mainVec[frontIndex + 1].country);
                    includedContacts.push_back(exist_contact);
                    frontIndex++;
                }
                return includedContacts;
            }
            else if (mainVec[mid].name < queryWord) // key in upper half
                low = mid + 1;
            else // key in lower half
                high = mid - 1;
        }
    }
    return includedContacts;
}

vector<contact_struct> sequentialSearch(const vector<contact_struct> & mainVec, const string& queryWord){
    vector<contact_struct> includedContacts;
    string name, surname;
    if(queryWord.find(' ') != string::npos){ //fullname
        istringstream ss(queryWord);
        ss >> name >> surname;
        for(const auto & i : mainVec){
            if(i.name == name && i.surname == surname){
                contact_struct foundedContact(name, surname, i.phone_no, i.country);
                includedContacts.push_back(foundedContact);
            }
        }
    }
    else{ //partial name
        for(const auto & i : mainVec){
            if(i.name.find(queryWord) != string::npos){
                contact_struct foundedContact(i.name, i.surname, i.phone_no, i.country);
                includedContacts.push_back(foundedContact);
            }
        }
    }
    return includedContacts;
}

int main(){

    string input_File;
    cout << "Please enter the contact file name:" << endl;
    cin >> input_File;
    cin.clear();
    cin.get();
    string input_word;
    cout << "Please enter the word to be queried:" << endl;
    getline(cin, input_word);

    ifstream file_opened;
    file_opened.open(input_File.c_str());
    string line;


    vector<contact_struct> insertionSortVector;
    vector<contact_struct> quickSortVector;
    vector<contact_struct> mergeSortVector;
    vector<contact_struct> heapSortVector;

    while(getline(file_opened, line)){
        istringstream ss(line);
        string nameEntry, surnameEntry, numberEntry, countryEntry;
        ss >> nameEntry >> surnameEntry >> numberEntry >> countryEntry;
        contact_struct contact_new(nameEntry, surnameEntry, numberEntry, countryEntry);
        insertionSortVector.push_back(contact_new);
        quickSortVector.push_back(contact_new);
        mergeSortVector.push_back(contact_new);
        heapSortVector.push_back(contact_new);
    }

    cout << endl << "Sorting the vector copies" << endl << "======================================" << endl;

    const int k = 100;
    vector<contact_struct> tempForSort;
    auto quickStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++){
        tempForSort = quickSortVector;
        helper_quick_sort(tempForSort);
    }
    auto quickSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - quickStart);
    helper_quick_sort(quickSortVector);

    cout << "Quick Sort Time: " << quickSortTime.count() / k << " Nanoseconds" << endl;

    auto insertStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++){
        tempForSort = insertionSortVector;
        insertionSort(tempForSort);
    }
    auto insertionSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - insertStart);
    insertionSort(insertionSortVector);

    cout << "Insertion Sort Time: " << insertionSortTime.count() / k << " Nanoseconds" << endl;

    auto mergeStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++){
        tempForSort = mergeSortVector;
        helper_merge_sort(tempForSort);
    }
    auto mergeSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mergeStart);
    helper_merge_sort(mergeSortVector);

    cout << "Merge Sort Time: " << mergeSortTime.count() / k << " Nanoseconds" << endl;

    auto heapStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++){
        tempForSort = heapSortVector;
        heapsort(tempForSort);
    }
    auto heapSortTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - heapStart);
    heapsort(heapSortVector);

    cout << "Heap Sort Time: " << heapSortTime.count() / k << " Nanoseconds" << endl;

    cout << endl << "Searching for " << input_word << endl << "======================================" << endl;

    const int j = 100;
    auto binaryStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < j; i++){
        vector<contact_struct> bin_sort_vec = binarySearch(insertionSortVector, input_word);
    }
    auto binarySearchTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - binaryStart);

    auto sequentialStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < j; i++){
        vector<contact_struct> seq_sort_vec = sequentialSearch(insertionSortVector, input_word);
    }
    auto sequentialSearchTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - sequentialStart);

    vector<contact_struct> checked_bin = binarySearch(insertionSortVector, input_word);
    helper_quick_sort(checked_bin);
    if(checked_bin.empty()){
        cout << UpperString(input_word) << " does NOT exist in the dataset" << endl;
    }
    else{
        for(auto & i : checked_bin){
            cout << UpperString(i.name) << " " << UpperString(i.surname)
                 << " " << i.phone_no << " " << i.country << endl;
        }
    }
    cout<<endl;
    cout << "Binary Search Time: " << binarySearchTime.count()/j << " Nanoseconds" << endl << endl;

    cout << "Search results for Sequential Search:" << endl;
    vector<contact_struct> checked_seq = sequentialSearch(insertionSortVector, input_word);
    if(checked_seq.empty()){
        cout << UpperString(input_word) << " does NOT exist in the dataset" << endl;
    }
    else{
        for(auto & i : checked_seq){
            cout << UpperString(i.name) << " " << UpperString(i.surname)
                 << " " << i.phone_no << " " << i.country << endl;
        }
    }
    cout<<endl;
    cout << "Sequential Search Time: " << sequentialSearchTime.count()/j << " Nanoseconds" << endl << endl;

    cout << "SpeedUp between Search Algorithms" << endl << "======================================" << endl;
    if(binarySearchTime.count()/j < sequentialSearchTime.count()/j)
        cout << "(Sequential Search/ Binary Search) SpeedUp = " << (double)sequentialSearchTime.count()/binarySearchTime.count() << endl << endl;
    else
        cout << "(Binary Search/ Sequential Search) SpeedUp = " << (double)binarySearchTime.count()/sequentialSearchTime.count() << endl << endl;

    cout << "SpeedUps between Sorting Algorithms" << endl << "======================================" << endl;

    if(quickSortTime.count()/k < insertionSortTime.count()/k)
        cout << "(Insertion Sort/ Quick Sort) SpeedUp = " << (double)insertionSortTime.count()/quickSortTime.count() << endl;
    else
        cout << "(Quick Sort/ Insertion Sort) SpeedUp = " << (double)quickSortTime.count()/insertionSortTime.count() << endl;
    if(quickSortTime.count()/k < mergeSortTime.count()/k)
        cout << "(Merge Sort/ Quick Sort) SpeedUp = " << (double)mergeSortTime.count()/quickSortTime.count() << endl;
    else
        cout << "(Quick Sort/ Merge Sort) SpeedUp = " << (double)quickSortTime.count()/mergeSortTime.count() << endl;
    if(quickSortTime.count()/k < heapSortTime.count()/k)
        cout << "(Heap Sort/ Quick Sort) SpeedUp = " << (double)heapSortTime.count()/quickSortTime.count() << endl;
    else
        cout << "(Quick Sort/ Heap Sort) SpeedUp = " << (double)quickSortTime.count()/heapSortTime.count() << endl;

    return 0;
}