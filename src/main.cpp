#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
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
    const int numbOfPunkToGen = 1500;
    const int scale = 240;
    const bool randomized = false;
    //vettore di stringhe che contiene il nome della cartella in cui sono contenuti i singoli layer
    const vector<string> layerDir = {"1-background", "2-character", "3-lip", "4-head", "5-eyes"};
    //singoli layer specificati
    const vector<vector<string>> singleLayer = 
    {
    {"Celo_Dark.gif", "Celo_Gray.gif", "Faint_Gold.gif"}, 
    
    {"Ape_Male_Alien.gif", "Blue_Cool_Woman.gif", "Dark_Female_Human.gif", "Dark_Male_Human.gif", "Darker_Male_Human.gif", "Female_Alien.gif", "Female_Ape.gif", "Female_Orc.gif", "Female_Zombie.gif", "Goddess_Night_Woman.gif", "Light_Male_Human.gif", "Lighter_Female_Human.gif", "Lighter_Male_Human.gif", "Male_Alien.gif", "Male_Ape.gif", "Male_Mummy.gif", "Male_Orc.gif", "Male_Robot.gif", "Male_Vampire.gif", "Male_Zombie_Ape.gif", "Male_Zombie.gif", "Red_Burning_Woman.gif"},
    
    {"Burgundy_Lipstick.gif", "Flashy_Blue_Lipstick.gif", "Gold_Lipstick.gif", "Party_Pink_Lipstick.gif", "Passion_Red_Lipstick.gif", "Purple_Lipstick.gif", "Space_Lipstick.gif", "Pipe.gif"},
    
    {"Bandana.gif", "Beanie.gif", "Blue_Cap.gif", "Cap_Forward.gif", "Cowboy_Hat.gif", "Crazy_Hair.gif", "Fedora.gif", "Green_Cap.gif", "Knitted_Cap.gif", "Male_Wild_Hair.gif", "Top_Hat.gif", "Wallstreetbets_Dark_Crazy_Hair.gif", "Wallstreetbets_Hair.gif"},
    
    {"3D_Glasses.gif", "Big_Shades.gif", "Classic_Shades.gif", "Empty.gif", "Eyepatch.gif", "Male_Earring.gif", "Nerd_Glasses.gif", "Regular_Shades.gif"}
    };
    
    //GESTIONE METADATA
    const vector<vector<string>> singleLayerName = 
    {
    {"Celo Dark", "Celo Gray", "Faint Gold"}, 
    
    {"Ape Male Alien", "Blue Cool Woman", "Dark Female Human", "Dark Male Human", "Darker Male Human", "Female Alien", "Female Ape", "Female Orc", "Female Zombie", "Goddess Night Woman", "Light Male Human", "Lighter Female Human", "Lighter Male Human", "Male Alien", "Male Ape", "Male Mummy", "Male Orc", "Male Robot", "Male Vampire", "Male Zombie Ape", "Male Zombie", "Red Burning Woman"},
    
    {"Burgundy Lipstick", "Flashy Blue Lipstick", "Gold Lipstick", "Party Pink Lipstick", "Passion Red Lipstick", "Purple Lipstick", "Space Lipstick", "Pipe"},
    
    {"Bandana", "Beanie", "Blue Cap", "Cap Forward", "Cowboy Hat", "Crazy Hair", "Fedora", "Green Cap", "Knitted Cap", "Male Wild Hair", "Top Hat", "Wallstreetbets Dark Crazy Hair", "Wallstreetbets Hair"},
    
    {"3D Glasses", "Big Shades", "Classic Shades", "Empty", "Eyepatch", "Male Earring", "Nerd Glasses", "Regular Shades"}
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
    {1, 7, 7, 7, 7, 4, 4, 4, 4, 6, 9, 9, 9, 2, 2, 2, 3, 2, 2, 2, 3, 4}, //22
    {18, 16, 5, 20, 5, 17, 16, 3}, //8
    {11, 1, 13, 11, 3, 1, 4, 13, 11, 3, 13, 13, 3}, //13
    {6, 6, 12, 20, 12, 20, 9, 15} //8
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
