#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

static void createLsFile(const string inputPath, const string outputPath)
{
    string systemCall = "ls " + inputPath + " > " + outputPath;
    
    //cout << endl << systemCall << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ls failed..." << endl;
        exit(1);
    }
    return;
}

static void deleteFile(string fileToDelete)
{
    fileToDelete = "../tmp/" + fileToDelete;
    if(remove(fileToDelete.c_str())!=0)
        cerr << "error deleting media_dna.txt or file not found" << endl;
    else
        clog << "file successfully deleted" << endl;
    return;
}

void createRarityFile(string pathOutput, const int &numOfMedia)
{
    vector<string> allDir;
    vector<vector<string>> allLayers;
    vector<vector<int>> trueRarity;
    int index;
    ifstream fin;
    ofstream fout;
    string buffer;
    
    createLsFile("../layers", "../tmp/list_layers.txt");
    
    fin.open("../tmp/list_layers.txt");
    if(fin.fail()){
        cerr << "errore durante l'apertura del file list_layers.txt" << endl;
        exit(2);
    }
    
    while(fin>>buffer)
        allDir.push_back(buffer);
    fin.close();
    allLayers.resize((int)allDir.size());
    trueRarity.resize((int)allDir.size());

    for(int i=0;i<(int)allDir.size();++i){
        createLsFile("../layers/" + allDir[i], "../tmp/list_layers.txt");
        fin.open("../tmp/list_layers.txt");
        if(fin.fail()){
            cerr << "errore durante l'apertura del file list_layers.txt" << endl;
            exit(2);
        }
        
        while(fin>>buffer)
            allLayers[i].push_back(buffer);
        
        trueRarity[i].resize((int)allLayers[i].size(), 0);
        fin.close();
    }
    
    fin.open("../tmp/media_dna.txt");
    if(fin.fail()){
        cerr << "errore durante l'apertura del file media_dna.txt" << endl;
        exit(2);
    }
    while(fin>>buffer>>index){
        int pos;
        for(pos=0;allDir[pos]!=buffer&&pos<(int)allDir.size();++pos);
        ++trueRarity[pos][index];
    }
    
    fin.close();
    fout.open(pathOutput);
    if(fout.fail()){
        cerr << "errore durante l'apertura del file rarity_list.txt" << endl;
        exit(2);
    }
    for(int i=0;i<(int)allLayers.size();++i){
        fout << allDir[i] << endl;
        for(int j=0;j<(int)allLayers[i].size();++j)
            fout << allLayers[i][j] << ": " << (double)trueRarity[i][j] * 100 / (double)numOfMedia << endl;
        fout << endl;
    }
    
    fout.close();
    return;
}

void updateInfo(const string &name, const string &description, const string &image, const vector<pair<string,string>> &extraMetadata, vector<int> &collectionSize, const int &collIndex, const int &nLayers, const string &format)
{
    string buffer, dna, edition, date, attributes;
    fstream file;
    
    for(int i=collectionSize[collIndex-1]+1;i<=collectionSize[collIndex];++i){
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
        file << "\t \"image\":\"" + image + "/" + to_string(i) + "." + format + "\"," << endl;
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

void readLayersAndRaritys(const vector<string> &layerDir, vector<vector<string>> &singleLayer, vector<vector<string>> &metadataSingleLayerName, vector<vector<int>> &rarityList)
{
    string buffer;
    ifstream fin;
    
    singleLayer.resize((int)layerDir.size());
    metadataSingleLayerName.resize((int)layerDir.size());
    rarityList.resize((int)layerDir.size());
    
    for(int i=0;i<(int)layerDir.size();++i){
        createLsFile("../layers/" + layerDir[i], "../tmp/list_layers.txt");
        fin.open("../tmp/list_layers.txt");
        if(fin.fail()){
            cerr << "errore durante l'apertura del file list_layers.txt" << endl;
            exit(2);
        }
        
        for(int j=0;fin>>buffer;++j){
            singleLayer[i].push_back(buffer);
            metadataSingleLayerName[i].push_back(singleLayer[i][j].substr(0, singleLayer[i][j].find("#")));
            try{
                rarityList[i].push_back(stoi(singleLayer[i][j].substr(singleLayer[i][j].find("#")+1)));
            }
            catch(invalid_argument const &exc){
                clog << "invalid_argument exception thrown, caused by stoi in readAllLayersAndRaritys" << endl;
                rarityList[i].push_back(0);
            }
        }
        
        fin.close();
    }
    return;
}

void convertCollectionSize(vector<int> &collectionSize)
{
    collectionSize.insert(collectionSize.begin(), 0);
    for(int i=1;i<(int)collectionSize.size();++i)
        collectionSize[i] += collectionSize[i-1];
    return;
}

void deleteAllTmpFiles()
{
    ifstream fin;
    string buffer;
    createLsFile("../tmp", "../tmp/delete_all.txt");
    
    fin.open("../tmp/delete_all.txt");
    if(fin.fail()){
        cerr << "errore durante l'apertura del file delete_all.txt" << endl;
        exit(2);
    }
    while(fin>>buffer)
        deleteFile(buffer);
    fin.close();
    
    deleteFile("../tmp/delete_all.txt");
    return;
}

void extractInteger(const string &str, vector<int> &mediaDna)
{
    int i = 0;
    stringstream ss;
    
    ss << str;
    
    string temp;
    int found;
    while(!ss.eof()){
    
        ss >> temp;
        
        if(stringstream(temp) >> found){
            mediaDna[i] = found;
            ++i;
        }
        
        temp = "";
    }
    return;
}

void deleteCharactersFromDnas(vector<string> &dnaOfAllMedia)
{
    bool copyChar;
    string buffer;
    for(int i=0;i<(int)dnaOfAllMedia.size();++i){
        copyChar = false;
        buffer.clear();
        for(int j=1;j<(int)dnaOfAllMedia[i].length();++j){
            if(copyChar)
                buffer.push_back(dnaOfAllMedia[i][j]);
            if(copyChar && dnaOfAllMedia[i][j]==' ')
                copyChar = false;
            else if(!copyChar && dnaOfAllMedia[i][j]==' ')
                copyChar = true;
        }
        dnaOfAllMedia[i] = buffer;
    }
    return;
}

