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
    
    //GESTIONE LAYER 
    const int numbOfPunkToGen = 100;
    const int scale = 240;
    const bool randomized = true;
    //vettore di stringhe che contiene il nome della cartella in cui sono contenuti i singoli layer
    const vector<string> layerDir = {"1-background", "2-character", "3-lip", "4-head", "5-eyes"};
    //singoli layer specificati
    const vector<vector<string>> singleLayer = 
    {
    {"Celo_Dark1.gif", "Celo_Dark2.gif", "Celo_Dark3.gif"}, 
    
    {"Ape_Male_Alien1.gif", "Ape_Male_Alien2.gif", "Ape_Male_Alien3.gif"},
    
    {"Burgundy_Lipstick1.gif", "Burgundy_Lipstick2.gif", "Burgundy_Lipstick3.gif"},
    
    {"Bandana1.gif", "Bandana2.gif", "Bandana3.gif"},
    
    {"3D_Glasses1.gif", "3D_Glasses2.gif", "3D_Glasses3.gif"}
    };
    
    //GESTIONE METADATA
    const vector<vector<string>> singleLayerName = 
    {
    {"Celo_Dark1.gif", "Celo_Dark2.gif", "Celo_Dark3.gif"}, 
    
    {"Ape_Male_Alien1.gif", "Ape_Male_Alien2.gif", "Ape_Male_Alien3.gif"},
    
    {"Burgundy_Lipstick1.gif", "Burgundy_Lipstick2.gif", "Burgundy_Lipstick3.gif"},
    
    {"Bandana1.gif", "Bandana2.gif", "Bandana3.gif"},
    
    {"3D_Glasses1.gif", "3D_Glasses2.gif", "3D_Glasses3.gif"}
    };
    
    const string name = "CeloPunk Animated Edition";
    const string description = "CeloPunks Animated Edition is a collection of 1500 unique special animated punks. Not affiliated with LarvaLabs.";
    const string image = "";
    const vector<pair<string,string>> extraMetadata = 
    {
    {"Compiler", "CeloPunks Algorithm"}
    };
    
    //GESTIONE RARITÀ
    const vector<vector<int>> rarityList = 
    {
    {20, 30, 50}, //3
    {20, 30, 50}, //3
    {20, 30, 50}, //3
    {20, 30, 50}, //3
    {20, 30, 50}, //3
    };
    
    if(randomized)
        genAndSaveDnaRandomly("../tmp/punksDna", singleLayer, numbOfPunkToGen);
    else
        genAndSaveDnaWithRarity("../tmp/punksDna", singleLayer, rarityList, numbOfPunkToGen);
    generateAllPunksMetadata(layerDir, singleLayerName, name, description, image, extraMetadata, numbOfPunkToGen);
    createRarityFile(layerDir, singleLayerName, numbOfPunkToGen);
    generateAllPunks(layerDir, singleLayer, numbOfPunkToGen, scale);
    
 return 0;
}
