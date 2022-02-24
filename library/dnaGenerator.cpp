#include "dnaGenerator.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <utility>
#include <algorithm>
#include <iterator>
#include <random>

using namespace std;

static bool wasAlrGen(const string &mediaDna, const vector<string> &dnaOfAllMedia, const int &currGen)
{
    bool flag = false;
    for(int i=0;i<=currGen&&!flag;++i)
        if(mediaDna==dnaOfAllMedia[i])
            flag = true;
    return flag;
}

void genAndSaveDnaRandomly(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const vector<int> &collectionSize, const int& collIndex, vector<string> &dnaOfAllMedia, const bool &unique)
{
    ofstream fout("./tmp/media_dna.txt", ios::app);
    string mediaDna;
    int layerInDir = -1, nLayers = (int)singleLayer.size(), randNum;
    
    if(fout.fail()){
        cerr << "in function 'genAndSaveDnaRandomly': error while opening in writing the file ./tmp/media_dna.txt" << endl;
        exit(2);
    }
    
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
    
    fout.close();
    return;
}


void genAndSaveDnaWithRarity(const vector<string> &layerDir,const vector<vector<string>> &singleLayer, const vector<vector<int>> &rarityList, const vector<int> &collectionSize, const int& collIndex, vector<string> &dnaOfAllMedia, const bool &unique)
{
    vector<vector<int>> rarityValue((int)singleLayer.size());
    string mediaDna;
    ofstream fout;
    int random = -1;

    //100:rarityList[i][j]=numbOfMediaToGen:x x=numbOfMediaToGen*rarityList[i][j]/100 CAPIRE MEGLIO RIGA 72/74 non mi ricordo più
    for(int i=0;i<(int)rarityValue.size();++i){
        int cont = 0;
        rarityValue[i].resize((int)rarityList[i].size());
        for(int j=0;j<(int)rarityValue[i].size();++j){
            rarityValue[i][j] = (int)(collectionSize[collIndex]-collectionSize[collIndex-1]) * rarityList[i][j] / 100;
            cont += rarityValue[i][j];
        }
        if(cont<(collectionSize[collIndex]-collectionSize[collIndex-1])){
            for(int j=cont, k=0;j<(collectionSize[collIndex]-collectionSize[collIndex-1]);++j, ++k){
                if(i!=0)
                    ++rarityValue[i][k%i];
                else
                    ++rarityValue[i][k];
            }
        }
    }
    
    fout.open("./tmp/media_dna.txt", ios::app);
    
    if(fout.fail()){
        cerr << "in function 'genAndSaveDnaWithRarity': error while opening in writing the file ./tmp/media_dna.txt" << endl;
        exit(2);
    }
    
    for(int i=collectionSize[collIndex-1];i<collectionSize[collIndex];){
        for(int j=0;j<(int)rarityValue.size();++j){
            random = rand() % (collectionSize[collIndex]-collectionSize[collIndex-1]);
            for(int k=0;random>=0&&k<(int)rarityValue[j].size();++k){
                random -= rarityValue[j][k];
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
    
    fout.close();
    return;
}
