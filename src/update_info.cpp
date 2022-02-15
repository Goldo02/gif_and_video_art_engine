#include <iostream>
#include <string>
#include "../library/utils.hpp"
#include "../library/metadataFunction.hpp"

using namespace std;

int main()
{
    const string name = "wow nice work";
    const string description = "CeloPunks Animated Edition is a collection of 1500 unique special animated punks. Not affiliated with LarvaLabs.";
    const string image = "https://ipfs.io/ipfs/QmawoM7cYGBtxaUQNE5Sk5TCmYjQ7dFr4Bpi3SfHwD22pp";
    const string format = "gif";
    vector<int> collectionSize = {5, 6};
    const vector<int> nLayers = {5, 3};
    const vector<pair<string,string>> &extraMetadata = 
    {
    {"Compiler", "CeloPunks compiler"},
    {"Github Author", "Goldo02"}
    };
    
    convertCollectionSize(collectionSize);
    for(int i=1;i<(int)collectionSize.size();++i)
        updateInfo(name, description, image, extraMetadata, collectionSize, i, nLayers[i-1], format);
    generateMetadataJson(collectionSize[(int)collectionSize.size()-1]);
 return 0;
}
