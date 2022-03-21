#include "dna_generator.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

static bool wasAlrGen(const string &mediaDna, const vector<string> &dnaOfAllMedia, const int &currGen)
{
    clog << "Entered the function: 'wasAlrGen'" << endl;
    bool flag = false;
    for(int i=0;i<=currGen&&!flag;++i)
        if(mediaDna==dnaOfAllMedia[i]){
            flag = true;
            clog << "Dna already exists" << endl;
        }
    clog << "Exiting the function: 'wasAlrGen'" << endl;
    return flag;
}

void genAndSaveDnaRandomly(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const vector<int> &collectionSize, const int& collIndex, vector<string> &dnaOfAllMedia, const bool &unique)
{
    clog << "Entered the function: 'genAndSaveDnaRandomly'" << endl;
    ofstream fout("./tmp/media_dna.txt", ios::app);
    string mediaDna;
    int layerInDir = -1, nLayers = (int)singleLayer.size(), randNum;
    
    if(fout.fail()){
        cerr << "in function 'genAndSaveDnaRandomly': error while opening in writing the file ./tmp/media_dna.txt" << endl;
        exit(2);
    }
    clog << "Randomly generating dna ..." << endl;
    for(int currGen=collectionSize[collIndex-1];currGen<collectionSize[collIndex];++currGen){
        do{
            mediaDna.clear();
            for(int i=0;i<nLayers;++i){
                layerInDir = (int)singleLayer[i].size();
                randNum = rand() % layerInDir;
                mediaDna = mediaDna + " " + layerDir[i] + " " + to_string(randNum);
            }
        }while(wasAlrGen(mediaDna, dnaOfAllMedia, currGen)&&unique);
        dnaOfAllMedia[currGen] = mediaDna;
        fout << mediaDna << endl;
    }
    clog << "Generation completed successfully" << endl;
    fout.close();
    clog << "Exiting the function: 'genAndSaveDnaRandomly'" << endl;
    return;
}


void genAndSaveDnaWithRarity(const vector<string> &layerDir,const vector<vector<string>> &singleLayer, const vector<vector<int>> &rarityWeight, const vector<int> &collectionSize, const int& collIndex, vector<string> &dnaOfAllMedia, const bool &unique)
{
    clog << "Entered the function: 'genAndSaveDnaWithRarity'" << endl;
    string mediaDna;
    int random = -1, totWeight = -1;
    ofstream fout("./tmp/media_dna.txt", ios::app);
    
    if(fout.fail()){
        cerr << "in function 'genAndSaveDnaWithRarity': error while opening in writing the file ./tmp/media_dna.txt" << endl;
        exit(2);
    }
    
    clog << "Generating dna with rarity ..." << endl;
    for(int i=collectionSize[collIndex-1];i<collectionSize[collIndex];){
        for(int j=0;j<(int)rarityWeight.size();++j){
            totWeight = 0;
            for(int k=0;k<(int)rarityWeight[j].size();++k)
                totWeight += rarityWeight[j][k];
            random = rand() % totWeight;
            for(int k=0;random>=0&&k<(int)rarityWeight[j].size();++k){
                random -= rarityWeight[j][k];
                if(random<0)
                    mediaDna = mediaDna + " " + layerDir[j] + " " + to_string(k);
            }
        }
        if(!wasAlrGen(mediaDna, dnaOfAllMedia, i-1) || !unique){
            dnaOfAllMedia[i] = mediaDna;
            fout << mediaDna << endl;
            ++i;
        }
        mediaDna.clear();
    }
    clog << "Generation completed successfully" << endl;
    
    fout.close();
    clog << "Exiting the function: 'genAndSaveDnaWithRarity'" << endl;
    return;
}
