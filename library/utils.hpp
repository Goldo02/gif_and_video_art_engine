#include <string>
#include <vector>

using namespace std;

void createRarityFile(string, const int &);
void updateInfo(const string &, const string &, const string &, const vector<pair<string,string>> &, vector<int> &, const int &, const int &, const string &);
void readLayersAndRaritys(const vector<string> &, vector<vector<string>> &, vector<vector<string>> &, vector<vector<int>> &);
void deleteAllTmpFiles();
void convertCollectionSize(vector<int> &);
void extractInteger(const string &, vector<int> &);
void deleteCharactersFromDnas(vector<string> &);
