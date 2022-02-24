#include <string>
#include <vector>

using namespace std;

void createRarityFile(string, const int &);
void readLayersAndRaritys(const vector<string> &, vector<vector<string>> &, vector<vector<string>> &, vector<vector<int>> &);

void deleteAllFilesOfFolder(string); 
void deleteFile(string);

void convertCollectionSize(vector<int> &);
void extractInteger(const string &, vector<int> &);

void readDnaFromFile(vector<string> &);
void deleteCharactersFromDnas(vector<string> &);

void createShuffleFile(const int &);
void reversePrevShuffle(const string &, const int &, const vector<int> &, const string &);
void shuffleCollection(vector<int> &, const string &, const string &, const string &, const string &, const int &);
void readRandomIndex(vector<int> &, const string &);
