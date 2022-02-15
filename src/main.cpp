#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>

#include "../library/myFFmpegConversion.hpp"
#include "../library/metadataFunction.hpp"
#include "../library/dnaGenerator.hpp"
#include "../library/utils.hpp"

using namespace std;

int main()
{
    srand(time(NULL));
    const int scale = 240;
    const string outputFormat = "gif";
    const bool randomized = true;
    const bool unique = true;
    vector<int> collectionSize = {5, 6};
    const vector<vector<string>> layerDir = 
    {
    {"1-background", "2-character", "3-head", "4-eyes", "5-lip"},
    {"1-background", "2-character", "3-head"}
    };
    
    const string name = "CeloPunks YouTube test";
    const string description = "i love CeloPunks";
    const string image = "https://twitter.com/elonmusk";
    const vector<pair<string,string>> extraMetadata = 
    {
    {"Compiler", "CeloPunks compiler"},
    {"Github Author", "Goldo02"}
    };
    
    vector<vector<vector<string>>> singleLayer((int)collectionSize.size());
    vector<vector<vector<string>>> metadataSingleLayerName((int)collectionSize.size());
    vector<vector<vector<int>>> rarityList((int)collectionSize.size());
    vector<string> dnaOfAllMedia;
    
    if((int)collectionSize.size()>=1){
        deleteAllTmpFiles();
        convertCollectionSize(collectionSize); dnaOfAllMedia.resize(collectionSize[(int)collectionSize.size()-1]);
        for(int i=1;i<(int)collectionSize.size();++i){
            readLayersAndRaritys(layerDir[i-1], singleLayer[i-1], metadataSingleLayerName[i-1], rarityList[i-1]);

            if(randomized)
                genAndSaveDnaRandomly("../tmp/media_dna.txt", layerDir[i-1], singleLayer[i-1], collectionSize, i, dnaOfAllMedia, unique);
            else
                genAndSaveDnaWithRarity("../tmp/media_dna.txt", layerDir[i-1], singleLayer[i-1], rarityList[i-1], collectionSize, i, dnaOfAllMedia, unique);
        }
         createRarityFile("../output/rarity_list.txt", collectionSize[(int)collectionSize.size()-1]);
          deleteCharactersFromDnas(dnaOfAllMedia);
        for(int i=1;i<(int)collectionSize.size();++i){
            generateAllMediaMetadata(layerDir[i-1], metadataSingleLayerName[i-1], name, description, image, extraMetadata, collectionSize, i, dnaOfAllMedia, outputFormat);
            generateAllMedia(layerDir[i-1], singleLayer[i-1], collectionSize, i, scale, dnaOfAllMedia, outputFormat);
        }
        generateMetadataJson(collectionSize[(int)collectionSize.size()-1]);
    }
 return 0;
}
