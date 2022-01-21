#include <iostream>
#include <string>
#include "../library/utils.hpp"
#include "../library/metadataFunction.hpp"

using namespace std;

int main()
{
    const string name = "wow nice work";
    const string description = "CeloPunks Animated Edition is a collection of 1500 unique special animated punks. Not affiliated with LarvaLabs.";
    const string image = "https://ipfs.io/ipfs/QmawoM7cYWBtxaUQNE5Sk5TCmYjQ7dFr4Bpi3SfHwD22pp";
    const int numbOfPunkToGen = 100;
    const int nLayers = 5;
    const vector<pair<string,string>> &extraMetadata = 
    {
    {"Compiler", "CeloPunks Algorithm"}
    };
    updateInfo(name, description, image, extraMetadata, numbOfPunkToGen, nLayers);
    generateMetadataJson(numbOfPunkToGen);
 return 0;
}
