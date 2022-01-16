#include "utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

void createRarityFile(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const int &numbOfPunkToGen)
{
    vector<vector<int>> numberOfLayers((int)layerDir.size());
    ifstream fin("../tmp/punksDna");
    ofstream fout;
    if(fin.fail()){
        cerr << "errore durante l'apertura del file dei dna" << endl;
        exit(2);
    }
    
    for(int i=0;i<(int)layerDir.size();++i)
        numberOfLayers[i].resize((int)singleLayer[i].size());
    
    for(int i=1;i<=numbOfPunkToGen;++i)
        for(int j=0;j<(int)layerDir.size();++j){
            int tmp;
            fin >> tmp;
            ++numberOfLayers[j][tmp];
        }
    
    fin.close();
    
    fout.open("../output/rarity_list");
    if(fout.fail()){
        cout << "errore apertura file rarità" << endl;
        exit(2);
    }
    
    for(int i=0;i<(int)layerDir.size();++i){
        fout << layerDir[i] << endl;
        for(int j=0;j<(int)singleLayer[i].size();++j)
            fout << singleLayer[i][j] << ": " << (double) numberOfLayers[i][j] * 100 / numbOfPunkToGen << endl;
        fout << endl;
    }
    
    return;
}

void updateInfo(const string &name, const string &description, const string &image, const vector<pair<string,string>> &extraMetadata, const int &numbOfPunkToGen, const int &nLayers)
{
    string buffer, dna, edition, date, attributes;
    fstream file;
    
    for(int i=1;i<=numbOfPunkToGen;++i){
        file.open("../output/json/" + to_string(i) + ".json", ios::in);
        if(file.fail()){
            cerr << "errore durante l'apertura del file in lettura (metadata n: " + to_string(i) +")" << endl;
            exit(2);
        }
        
        //leggo le informazioni utili
        for(int j=1;j<=4;++j)
            getline(file, buffer);
        getline(file, dna);
        getline(file, edition);
        getline(file, date);
        buffer.clear();
        getline(file, attributes);
        attributes += "\n";
        for(int j=1;j<=nLayers;++j){
            getline(file, buffer);
            buffer += "\n";
            attributes += buffer;
            getline(file, buffer);
            buffer += "\n";
            attributes += buffer;
            getline(file, buffer);
            buffer += "\n";
            attributes += buffer;
            getline(file, buffer);
            buffer += "\n";
            attributes += buffer;
        }
        getline(file, buffer);
        attributes += buffer;
        file.close();
        
        file.open("../output/json/" + to_string(i) + ".json", ios::out);
        if(file.fail()){
            cerr << " errore durante l'apertura del file in scrittura (metadata n: " + to_string(i) +")" << endl;
            exit(2);
        }
        file << "{" << endl;
        file << "\t \"name\":\"" + name + " #" + to_string(i) + "\"," << endl;
        file << "\t \"description\":\"" + description + "\"," << endl;
        file << "\t \"image\":\"" + image + "/" + to_string(i) + ".gif\"," << endl;
        file << dna << endl;
        file << edition << endl;
        file << date << endl;
        file << attributes << endl;
        for(int i=0;i<(int)extraMetadata.size();++i){
            file << "\t \"" + extraMetadata[i].first + "\": \"" + extraMetadata[i].second + "\"";
            if(i!=(int)extraMetadata.size()-1)
                file << "," << endl;
            else
                file << endl;
        }
        file << "}" << endl;
        
        file.close();
    }
    return;
}
