#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

static void createLsFile(const string inputPath, const string outputPath)
{
    string systemCall = "ls " + inputPath + " > " + outputPath;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ls failed.." << endl;
        cerr << "in function 'createLsFile': the command executed was " + systemCall << endl;
        exit(1);
    }
    return;
}

void deleteFile(string fileToDelete)
{
    if(remove(fileToDelete.c_str())!=0)
        cerr << "in function 'deleteFile': error while deleting the file / file not found " + fileToDelete << endl;
    else
        clog << fileToDelete + " successfully deleted" << endl;
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
    
    createLsFile("./layers", "./tmp/list_layers.txt");
    
    fin.open("./tmp/list_layers.txt");
    if(fin.fail()){
        cerr << "in function 'createRarityFile': error while opening in reading the file ./tmp/list_layers.txt" << endl;
        exit(2);
    }
    
    while(fin>>buffer)
        allDir.push_back(buffer);
    fin.close();
    allLayers.resize((int)allDir.size());
    trueRarity.resize((int)allDir.size());

    for(int i=0;i<(int)allDir.size();++i){
        createLsFile("./layers/" + allDir[i], "./tmp/list_layers.txt");
        fin.open("./tmp/list_layers.txt");
        if(fin.fail()){
            cerr << "in function 'createRarityFile': error while opening in reading the file ./tmp/list_layers.txt" << endl;
            exit(2);
        }
        
        while(fin>>buffer)
            allLayers[i].push_back(buffer);
        
        trueRarity[i].resize((int)allLayers[i].size(), 0);
        fin.close();
    }
    
    fin.open("./tmp/media_dna.txt");
    if(fin.fail()){
        cerr << "in function 'createRarityFile': error while opening in reading the file ./tmp/media_dna.txt" << endl;
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
        cerr << "in function 'createRarityFile': error while opening in writing the file " + pathOutput << endl;
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

void readLayersAndRaritys(const vector<string> &layerDir, vector<vector<string>> &singleLayer, vector<vector<string>> &metadataSingleLayerName, vector<vector<int>> &rarityList)
{
    string buffer;
    ifstream fin;
    
    singleLayer.resize((int)layerDir.size());
    metadataSingleLayerName.resize((int)layerDir.size());
    rarityList.resize((int)layerDir.size());
    
    for(int i=0;i<(int)layerDir.size();++i){
        createLsFile("./layers/" + layerDir[i], "./tmp/list_layers.txt");
        fin.open("./tmp/list_layers.txt");
        if(fin.fail()){
            cerr << "in function 'readLayersAndRaritys': error while opening in reading the file ./tmp/list_layers.txt" << endl;
            exit(2);
        }
        
        for(int j=0;fin>>buffer;++j){
            singleLayer[i].push_back(buffer);
            metadataSingleLayerName[i].push_back(singleLayer[i][j].substr(0, singleLayer[i][j].find("#")));
            replace(metadataSingleLayerName[i][j].begin(), metadataSingleLayerName[i][j].end(), '_', ' ');
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

void deleteAllFilesOfFolder(string pathToDelete)
{
    ifstream fin;
    string buffer;
    createLsFile(pathToDelete, pathToDelete + "/delete_all.txt");
    
    fin.open(pathToDelete + "/delete_all.txt");
    if(fin.fail()){
        cerr << "in function 'deleteAllFilesOfFolder': error while opening in reading the file " + pathToDelete + "/delete_all.txt" << endl;
        exit(2);
    }
    while(fin>>buffer)
        deleteFile(pathToDelete + "/" + buffer);
    fin.close();
    
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

void readDnaFromFile(vector<string> &dnaOfAllMedia)
{
    ifstream fin("./tmp/media_dna.txt");
    if(fin.fail()){
        cerr << "in function 'readDnaFromFile': error while opening in reading the file ./tmp/media_dna.txt" << endl;
        exit(2);
    }
    
    for(int i=0;i<(int)dnaOfAllMedia.size();++i)
        getline(fin, dnaOfAllMedia[i]);
    
    fin.close();
    return;
}

void createShuffleFile(const int &numOfMedia)
{
    srand(time(NULL));
    ofstream fout("./tmp/shuffle_code.txt");
    if(fout.fail()){
        cerr << "in function 'createShuffleFile': error while opening in writing the file ./tmp/shuffle_code.txt" << endl;
        exit(2);
    }
    
    for(int i=1;i<=numOfMedia;++i)
        fout << (rand() % numOfMedia) + 1<< endl;
    
    fout.close();
    return;
}

void reversePrevShuffle(const string &chain, const int &numOfMedia, const vector<int> &randomIndex, const string &outputFormat)
{
    int adjustForChain = 0, i = numOfMedia;

    if(chain=="SOL"){
        i=numOfMedia-1;
        adjustForChain = 1;
    }
    for(;i>=1-adjustForChain;--i){
        if(chain=="SOL"){
            //swap media
            rename(string("./output/media/" + to_string(i) + "." + outputFormat).c_str(), string("./output/media/tmp." + outputFormat).c_str());
            rename(string("./output/media/" + to_string(randomIndex[i]) + "." + outputFormat).c_str(), string("./output/media/" + to_string(i) + "." + outputFormat).c_str());
            rename(string("./output/media/tmp." + outputFormat).c_str(), string("./output/media/" + to_string(randomIndex[i]) + "." + outputFormat).c_str());
        }
        else{
            //swap media
            rename(string("./output/media/" + to_string(i) + "." + outputFormat).c_str(), string("./output/media/tmp." + outputFormat).c_str());
            rename(string("./output/media/" + to_string(randomIndex[i-1]) + "." + outputFormat).c_str(), string("./output/media/" + to_string(i) + "." + outputFormat).c_str());
            rename(string("./output/media/tmp." + outputFormat).c_str(), string("./output/media/" + to_string(randomIndex[i-1]) + "." + outputFormat).c_str());
        }
    }
    return;
}

//*image si può togliere penso
void swapTwoJsonContentOnSolana(const string &name, const string &image, const string &outputFormat, const int &indexA, const int &indexB)
{
    string buffer, fileA, fileB, category;
    ifstream fin("./output/json/" + to_string(indexA) + ".json");
    ofstream fout;
    
    if(fin.fail()){
        cerr << "tokerrore durante l'apertura del file json n" + to_string(indexA) << endl;
        exit(2);
    }

    if(outputFormat=="gif")
        category = "image";
    else
        category = "video";
        
    getline(fin, buffer);
    fileA += buffer + "\n";
    getline(fin, buffer);
    fileA += "\t \"name\":\"" + name + " #" + to_string(indexB + 1) + "\", \n";
    for(int i=1;i<=4;++i){
        getline(fin, buffer);
        fileA += buffer + "\n";
    }
    getline(fin, buffer);
    fileA += "\t \"image\":\"" + to_string(indexB) + "." + outputFormat + "\", \n";
    while(buffer.find("file")==-1){
        getline(fin, buffer);
        if(buffer.find("file")==-1)
            fileA += buffer + "\n";
    }
    
    fileA += "\t \t \"files\": [{\"uri\": \"" + to_string(indexB) + "." + outputFormat +"\", \"type\": \"" + category + "/" + outputFormat + "\"}] \n";
    
    while(getline(fin, buffer))
        fileA += buffer + "\n";
    fin.close();
    
    fin.open("./output/json/" + to_string(indexB) + ".json");
    if(fin.fail()){
        cerr << "tokerrore durante l'apertura del file json n" + to_string(indexA) << endl;
        exit(2);
    }
    
    getline(fin, buffer);
    fileB += buffer + "\n";
    getline(fin, buffer);
    fileB += "\t \"name\":\"" + name + " #" + to_string(indexA + 1) + "\", \n";
    for(int i=1;i<=4;++i){
        getline(fin, buffer);
        fileB += buffer + "\n";
    }
    getline(fin, buffer);
    fileB += "\t \"image\":\"" + to_string(indexA) + "." + outputFormat + "\", \n";
    while(buffer.find("file")==-1){
        getline(fin, buffer);
        if(buffer.find("file")==-1)
            fileB += buffer + "\n";
    }
    
    fileB += "\t \t \"files\": [{\"uri\": \"" + to_string(indexA) + "." + outputFormat +"\", \"type\": \"" + category + "/" + outputFormat + "\"}] \n";
    
    while(getline(fin, buffer))
        fileB += buffer + "\n";
    
    fin.close();
    
    fout.open("./output/json/" + to_string(indexB) + ".json");
    if(fout.fail()){
        cerr << "errore durante l'apertura in scrittura del json n" + to_string(indexA) << endl;
        exit(2);
    }
    
    fout << fileA;
    fout.close();
    
    fout.open("./output/json/" + to_string(indexA) + ".json");
    if(fout.fail()){
        cerr << "errore durante l'apertura in scrittura del json n" + to_string(indexA) << endl;
        exit(2);
    }

    fout << fileB;
    fout.close();
    return;
}

static void swapTwoJsonContent(const string &name, const string &image, const string &outputFormat, const int &indexA, const int &indexB)
{
    string buffer, fileA, fileB;
    ifstream fin("./output/json/" + to_string(indexA) + ".json");
    ofstream fout;
    if(fin.fail()){
        cerr << "in function 'swapTwoJsonContent': error while opening in reading the file ./output/json/" + to_string(indexA) + ".json" << endl;
        exit(2);
    }
    getline(fin, buffer); //{
    fileA += buffer + "\n";
    getline(fin, buffer); //name
    fileA += "\t \"name\":\"" + name + " #" + to_string(indexB) + "\", \n";
    getline(fin, buffer); //description
    fileA += buffer + "\n";
    getline(fin, buffer); //image
    fileA += "\t \"image\":\"" + image +"/" + to_string(indexB) + "." + outputFormat + "\", \n";
    getline(fin, buffer); //dna
    fileA += buffer + "\n";
    getline(fin, buffer); //edition
    fileA += "\t \"edition\":\"" + to_string(indexB) +"\", \n";
    while(getline(fin, buffer))
        fileA += buffer + "\n";
    fin.close();
    
    fin.open("./output/json/" + to_string(indexB) + ".json");
    if(fin.fail()){
        cerr << "in function 'swapTwoJsonContent': error while opening in reading the file ./output/json/" + to_string(indexB) + ".json" << endl;
        exit(2);
    }
    getline(fin, buffer); //{
    fileB += buffer + "\n";
    getline(fin, buffer); //name
    fileB += "\t \"name\":\"" + name + " #" + to_string(indexA) + "\", \n";
    getline(fin, buffer); //description
    fileB += buffer + "\n";
    getline(fin, buffer); //image
    fileB += "\t \"image\":\"" + image +"/" + to_string(indexA) + "." + outputFormat + "\", \n";
    getline(fin, buffer); //dna
    fileB += buffer + "\n";
    getline(fin, buffer); //edition
    fileB += "\t \"edition\":\"" + to_string(indexA) +"\", \n";
    while(getline(fin, buffer))
        fileB += buffer + "\n";
    fin.close();
    
    fout.open("./output/json/" + to_string(indexB) + ".json");
    if(fout.fail()){
        cerr << "in function 'swapTwoJsonContent': error while opening in writing the file ./output/json/" + to_string(indexB) + ".json" << endl;
        exit(2);
    }
    
    fout << fileA;
    fout.close();
    
    fout.open("./output/json/" + to_string(indexA) + ".json");
    if(fout.fail()){
        cerr << "in function 'swapTwoJsonContent': error while opening in writing the file ./output/json/" + to_string(indexA) + ".json" << endl;
        exit(2);
    }

    fout << fileB;
    fout.close();
    
    return;
}

void readRandomIndex(vector<int> &randomIndex, const string &chain)
{
    ifstream fin("./tmp/shuffle_code.txt");
    if(fin.fail()){
        cerr << "in function 'readRandomIndex': error while opening in reading the file ./tmp/shuffle_code.txt" << endl;
        exit(2);
    }
    
    for(int i=0;i<(int)randomIndex.size();++i){
        fin >> randomIndex[i];
        if(chain=="SOL")
            --randomIndex[i];
    }
    
    fin.close();
    return;
}

void shuffleCollection(vector<int> &randomIndex, const string &name, const string &image, const string &outputFormat, const string &chain, const int &numOfMedia)
{
    int adjustForChain = 0, i = 1;

    if(chain=="SOL"){
        i=0;
        adjustForChain = 1;
    }
    for(;i<=numOfMedia-adjustForChain;++i){
        if(chain=="SOL"){
            swapTwoJsonContentOnSolana(name, image, outputFormat, i, randomIndex[i]);
            //swap media
            rename(string("./output/media/" + to_string(i) + "." + outputFormat).c_str(), string("./output/media/tmp." + outputFormat).c_str());
            rename(string("./output/media/" + to_string(randomIndex[i]) + "." + outputFormat).c_str(), string("./output/media/" + to_string(i) + "." + outputFormat).c_str());
            rename(string("./output/media/tmp." + outputFormat).c_str(), string("./output/media/" + to_string(randomIndex[i]) + "." + outputFormat).c_str());
        }
        else{
            swapTwoJsonContent(name, image, outputFormat, i, randomIndex[i-1]);
            //swap media
            rename(string("./output/media/" + to_string(i) + "." + outputFormat).c_str(), string("./output/media/tmp." + outputFormat).c_str());
            rename(string("./output/media/" + to_string(randomIndex[i-1]) + "." + outputFormat).c_str(), string("./output/media/" + to_string(i) + "." + outputFormat).c_str());
            rename(string("./output/media/tmp." + outputFormat).c_str(), string("./output/media/" + to_string(randomIndex[i-1]) + "." + outputFormat).c_str());
        }
    }
    
    return;
}
