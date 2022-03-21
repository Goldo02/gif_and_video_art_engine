#ifndef METADATA_FUNCTION_H
#define METADATA_FUNCTION_H

#include <string>
#include <vector>

using namespace std;

void generateAllMediaMetadata(const vector<string> &, const vector<vector<string>> &, const string &, const string &, const string &, const vector<pair<string,string>> &, const vector<int> &, const int &, vector<string> &, const string &);
void generateAllMediaSolanaMetadata(const vector<string> &, const vector<vector<string>> &, const string &, const string &, const string &, const string &, const string &, const string &, const vector<string> &, const vector<string> &, const vector<pair<string,string>> &, const vector<int> &, const int &, vector<string> &, const string &);
void generateMetadataJson(const int &, const string &);

#endif
