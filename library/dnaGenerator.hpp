#include <vector>
#include <string>

using namespace std;

static bool wasAlrGen(string &, vector<string> &, int);
void genAndSaveDnaRandomly(string, const vector<vector<string>> &, const int);
void genAndSaveDnaWithRarity(string, const vector<vector<string>> &, const vector<vector<int>> &, const int);

/*
void genAndSaveDnaWithRarityInefficient(string, const vector<vector<string>> &, const vector<vector<int>> &, const int);
static pair<int,int> contSingleTreeAndGenSomeDna(vector<vector<int>> &, vector<string> &, vector<pair<int,int>> &, int &, int, int);
static bool generateSingleDnaRemained(vector<vector<int>> &, vector<string> &, vector<pair<int,int>> &, int &, int, int);
static void generateWithAproxRarity(vector<vector<int>> &, vector<string> &);
static bool custome_compare(const pair<int, int> &, const pair<int, int> &);
*/

