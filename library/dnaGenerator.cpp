#include "dnaGenerator.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <utility>
#include <algorithm>
#include <random>

using namespace std;

static bool wasAlrGen(string &punkDna, vector<string> &dnaOfAllPunks, int currGen) //è O(n) si potrebbe miglioare a O(logn)
{
    bool flag = false;
    for(int i=0;i<=currGen&&!flag;++i)
        if(punkDna==dnaOfAllPunks[i])
            flag = true;
    return flag;
}

void genAndSaveDnaRandomly(string nameFile, const vector<vector<string>> &singleLayer, const int numbOfPunkToGen)
{
    ofstream fout(nameFile);
    string punkDna;
    int layerInDir = -1, nLayers = (int)singleLayer.size();
    vector<string> dnaOfAllPunks(numbOfPunkToGen);
    
    if(fout.fail()){
        cerr << "errore durante l'apertura del file dei dna" << endl;
        exit(2);
    }
    
    for(int currGen=0;currGen<numbOfPunkToGen;++currGen){
        do{
            punkDna.clear();
            for(int i=0;i<nLayers;++i){
                layerInDir = (int)singleLayer[i].size();
                punkDna = punkDna + " " + to_string(rand() % layerInDir);
            }
        }while(wasAlrGen(punkDna, dnaOfAllPunks, currGen));
        dnaOfAllPunks[currGen] = punkDna;
        fout << punkDna << endl;
    }
    
    fout.close();
    return;
}

/*static pair<int,int> contSingleTreeAndWriteSomeDna(vector<vector<int>> &rarityValue, vector<string> &dnaOfAllPunks, vector<pair<int,int>> &indici, int &result, int i, int j)
{
    pair<int,int> nodeBack(-1,-1);
    if(i+1>=(int)rarityValue.size()){
        indici[i] = make_pair(i, j);
        dnaOfAllPunks[result].clear();
        for(int k=(int)indici.size()-1;k>=0;--k){
            --rarityValue[indici[k].first][indici[k].second];
            if(rarityValue[indici[k].first][indici[k].second]<=0)
                nodeBack = make_pair(indici[k].first, indici[k].second);
        }
        for(int k=0;k<(int)indici.size();++k)
            dnaOfAllPunks[result] = dnaOfAllPunks[result] + " " + to_string(indici[k].second);
        clog << "dna generated: " << result << endl;
        ++result;
        return nodeBack;
    }
    indici[i] = make_pair(i, j);
    
    for(int k=0;k<(int)rarityValue[i+1].size()&&rarityValue[i][j]>0;++k){
        if(rarityValue[i+1][k]>0&&rarityValue[i][j]>0){
            nodeBack = contSingleTreeAndWriteSomeDna(rarityValue, dnaOfAllPunks, indici, result, i+1, k);
            if(nodeBack.first!=-1&&nodeBack.second!=-1){
                if(nodeBack.first==i&&nodeBack.second==j)
                    return make_pair(-1,-1);
                return nodeBack;
            }
        }
    }

    return nodeBack;
}

static bool custome_compare(const pair<int, int> &p1, const pair<int, int> &p2)
{
    return p1.first > p2.first;
}

static bool generateSingleDnaRemained(vector<vector<int>> &rarityValue, vector<vector<pair<int,int>>> &indexedRarityValue, vector<string> &dnaOfAllPunks, vector<pair<int,int>> &indici, int &result, int i, int j)
{
    bool flag = false;
    if(i+1>=(int)rarityValue.size()){
        indici[i] = make_pair(i, j);
        dnaOfAllPunks[result].clear();
        
        for(int k=0;k<(int)indici.size();++k)
            dnaOfAllPunks[result] = dnaOfAllPunks[result] + " " + to_string(indexedRarityValue[indici[k].first][indici[k].second].second);
            
        if(!wasAlrGen(dnaOfAllPunks[result], dnaOfAllPunks, result-1)){
            for(int k=(int)indici.size()-1;k>=0;--k){
                --indexedRarityValue[indici[k].first][indici[k].second].first;
                if(max_element(indexedRarityValue[indici[k].first].begin(),indexedRarityValue[indici[k].first].end()) - indexedRarityValue[indici[k].first].begin()!=indici[k].second)
                    flag = true;
            }
            clog << "dna generated: " << result << endl;
            ++result;
        }
            
        return flag;
    }
    
    indici[i] = make_pair(i, j);
    
    for(int k=0;k<(int)indexedRarityValue[i+1].size()&&!flag;++k)
        flag=generateSingleDnaRemained(rarityValue, indexedRarityValue, dnaOfAllPunks, indici, result, i+1, k);
    
    return flag;
}

static void generateWithAproxRarity(vector<vector<int>> &rarityValue, vector<string> &dnaOfAllPunks, const int &numbOfPunkToGen)
{
    vector<pair<int,int>> indici((int)rarityValue.size());
    vector<vector<pair<int,int>>> indexedRarityValue((int)rarityValue.size());
    int result = 0;
    for(int i=0;i<(int)rarityValue[0].size();++i)
        contSingleTreeAndWriteSomeDna(rarityValue, dnaOfAllPunks, indici, result, 0, i);
    
    cout << "A) result: " << result << endl;
    for(int i=0;i<(int)indexedRarityValue.size();++i){
        indexedRarityValue[i].resize((int)rarityValue[i].size());
        for(int j=0;j<(int)indexedRarityValue[i].size();++j){
            indexedRarityValue[i][j].first = rarityValue[i][j];
            indexedRarityValue[i][j].second = j;
        }
    }
    
    while(result<numbOfPunkToGen){
        for(int i=0;i<(int)indexedRarityValue.size();++i)
            sort(indexedRarityValue[i].begin(), indexedRarityValue[i].end(), custome_compare);
        generateSingleDnaRemained(rarityValue, indexedRarityValue, dnaOfAllPunks, indici, result, 0, 0);
    }
    cout << "B) result: " << result << endl;
    return;
}

void genAndSaveDnaWithRarityInefficient(string nameFile, const vector<vector<string>> &singleLayer, const vector<vector<int>> &rarityList, const int numbOfPunkToGen)
{
    ofstream fout;
    string punkDna;
    vector<int> punkDnaInt((int)singleLayer.size());
    vector<string> dnaOfAllPunks(numbOfPunkToGen);
    vector<vector<int>> rarityValue((int)singleLayer.size());
    
    //100:rarityList[i][j]=numbOfPunkToGen:x x=numbOfPunkToGen*rarityList[i][j]/100
    for(int i=0;i<(int)rarityValue.size();++i){
        int cont = 0;
        rarityValue[i].resize((int)rarityList[i].size());
        for(int j=0;j<(int)rarityValue[i].size();++j){
            rarityValue[i][j] = numbOfPunkToGen * rarityList[i][j] / 100;
            cont += rarityValue[i][j];
        }
        if(cont<numbOfPunkToGen){
            for(int j=cont, k=0;j<numbOfPunkToGen;++j, ++k)
                ++rarityValue[i][k%i];
        }
    }
    
    generateWithAproxRarity(rarityValue, dnaOfAllPunks, numbOfPunkToGen);
    
    fout.open(nameFile);
    
    if(fout.fail()){
        cerr << "errore durante l'apertura del file dei dna" << endl;
        exit(2);
    }
    auto rng = default_random_engine {};
    shuffle(begin(dnaOfAllPunks), end(dnaOfAllPunks), rng);
    
    for(int i=0;i<numbOfPunkToGen;++i)
        fout << dnaOfAllPunks[i] << endl;
    fout.close();
    return;
}*/

void genAndSaveDnaWithRarity(string nameFile, const vector<vector<string>> &singleLayer, const vector<vector<int>> &rarityList, const int numbOfPunkToGen)
{
    vector<vector<int>> rarityValue((int)singleLayer.size());
    vector<string> dnaOfAllPunks(numbOfPunkToGen);
    string punkDna;
    ofstream fout;
    int random = -1;

    //100:rarityList[i][j]=numbOfPunkToGen:x x=numbOfPunkToGen*rarityList[i][j]/100
    for(int i=0;i<(int)rarityValue.size();++i){
        int cont = 0;
        rarityValue[i].resize((int)rarityList[i].size());
        for(int j=0;j<(int)rarityValue[i].size();++j){
            rarityValue[i][j] = numbOfPunkToGen * rarityList[i][j] / 100;
            cont += rarityValue[i][j];
        }
        if(cont<numbOfPunkToGen){
            for(int j=cont, k=0;j<numbOfPunkToGen;++j, ++k)
                ++rarityValue[i][k%i];
        }
    }
    
    fout.open(nameFile);
    
    if(fout.fail()){
        cerr << "errore durante l'apertura del file dei dna" << endl;
        exit(2);
    }
    
    for(int i=0;i<numbOfPunkToGen;){
        for(int j=0;j<(int)rarityValue.size();++j){
            random = rand() % numbOfPunkToGen;
            for(int k=0;random>=0&&k<(int)rarityValue[j].size();++k){
                random -= rarityValue[j][k];
                if(random<0)
                    punkDna = punkDna + " " + to_string(k);
            }
        }
        if(!wasAlrGen(punkDna, dnaOfAllPunks, i-1)){
            dnaOfAllPunks[i] = punkDna;
            fout << punkDna << endl;
            ++i;
        }
        punkDna.clear();
    }
    
    fout.close();
    return;
}
