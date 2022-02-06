#include "metadataFunction.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>

using namespace std;

static void generateSingleMetadata(const vector<string> &punkDna, int currGen, const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const string &name, const string &description, const string &image, const vector<pair<string,string>> &extraMetadata)
{
    string dna = "";
    ofstream fout("../output/json/" + to_string(currGen) + ".json");
    if(fout.fail()){
        cerr << " errore durante l'apertura del file in scrittura (metadata n: " + to_string(currGen) +")" << endl;
        exit(2);
    }
    
    for(int i=0;i<(int)punkDna.size();++i)
        dna = dna + " " + punkDna[i];
        
    fout << "{" << endl;
    fout << "\t \"name\":\"" + name + " #" + to_string(currGen) + "\"," << endl;
    fout << "\t \"description\":\"" + description + "\"," << endl;
    fout << "\t \"image\":\"" + image + "/" + to_string(currGen) + ".gif\"," << endl;
    fout << "\t \"dna\": \"" + dna + " \"," << endl;
    fout << "\t \"edition\":\"" + to_string(currGen) + "\"," << endl;
    fout << "\t \"date\":\"" + to_string(time(NULL)) + "\"," << endl;
    fout << "\t \"attributes\": [" << endl;
    for(int i=0;i<(int)layerDir.size();++i){
        fout << "\t \t {" << endl;
        fout << "\t \t \t \"trait_type\": \"" + layerDir[i] + "\"," << endl;
        fout << "\t \t \t \"value\": \"" + singleLayer[i][atoi(punkDna[i].c_str())] + "\"" << endl;
        if(i!=(int)layerDir.size()-1)
            fout << "\t \t }," << endl;
        else
            fout << "\t \t }" << endl;
    }
    fout << "\t ]," << endl;
    for(int i=0;i<(int)extraMetadata.size();++i){
        fout << "\t \"" + extraMetadata[i].first + "\": \"" + extraMetadata[i].second + "\"";
        if(i!=(int)extraMetadata.size()-1)
            fout << "," << endl;
        else
            fout << endl;
    }
    fout << "}" << endl;
    fout.close();
    return;
}

void generateMetadataJson(const int &numbOfPunkToGen)
{
    ofstream fout("../output/json/_metadata.json");
    ifstream fin;
    string buffer;
    if(fout.fail()){
        cerr << "errore durante l'apertura in scrittura di _metadata.json" << endl;
        exit(2);
    }
    
    fout << "[" << endl;
    for(int i=1;i<=numbOfPunkToGen;++i){
        fin.open("../output/json/" + to_string(i) + ".json");
        if(fin.fail()){
            cerr << "errore durante l'apertura del json n: " << to_string(i) << endl;
            exit(2);
        }
        while(getline(fin, buffer))
            fout << "\t" << buffer << endl;
        if(i!=numbOfPunkToGen)
            fout << "\t ," << endl;
        else
            fout << endl;
        fin.close();
    }
    fout << "]" << endl;
    
    fout.close();
    return;
}

void generateAllPunksMetadata(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const string &name, const string &description, const string &image, const vector<pair<string,string>> &extraMetadata, const int &numbOfPunkToGen)
{
    vector<string> punkDna((int)layerDir.size());
    ifstream fin("../tmp/punksDna");
    
    if(fin.fail()){
        cerr << "errore durante l'apertura di punksDna" << endl;
        exit(2);
    }
    
    for(int i=1;i<=numbOfPunkToGen;++i){
        for(int j=0;j<(int)layerDir.size();++j)
            fin >> punkDna[j];
        generateSingleMetadata(punkDna, i, layerDir, singleLayer, name, description, image, extraMetadata);
    }
    fin.close();
    
    generateMetadataJson(numbOfPunkToGen);
    return;
}
