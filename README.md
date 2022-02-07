# GIF Art Engine For NFT 

## Introduction
A GIF art engine that will allow you to generate multi-layer GIFs from single GIFs as layers. All the code in this repository has been written by **me** in c++, inspired by the generative art engine of HashLips that does not support GIFs as layers. The problem arose from my and my teamleader's need to generate animated images and then GIFs, in the same way as HashLips generated static images.

If you found my work useful for any of your projects, please make some donations to support me.

MetaMask address: 0x58d55348e16C1241D1b7e9238c7753e9c7BF19CB

For help contact me on discord: luffy2023 il cane fifone#2742

## What does the code allow you to do? 
When you have downloaded the code you will be able to generate your own GIFs using GIFs as layers. What the code will do is overlay all the GIFs you have chosen in the desired order and generate _n_ final gifs. This is an example of generating a gif with five separate layers:

|Num of layer|Layer|Name|Gif|
|------------|-----|----|---|
|1|Background|f8f9f9.gif|![Alt Text](https://github.com/Goldo02/gif_art_engine_for_nft/blob/main/example/f8f9f9.gif)|
|2|Character |zombie-male.gif|![Alt Text](https://github.com/Goldo02/gif_art_engine_for_nft/blob/main/example/zombie-male.gif)|
|3|Head      |tophat.gif|![Alt Text](https://github.com/Goldo02/gif_art_engine_for_nft/blob/main/example/tophat.gif)|
|4|Eyes      |bigshades.gif|![Alt Text](https://github.com/Goldo02/gif_art_engine_for_nft/blob/main/example/bigshades.gif)|
|5|Mouth     |lipstick-party_pink.gif|![Alt Text](https://github.com/Goldo02/gif_art_engine_for_nft/blob/main/example/lipstick-party_pink.gif)|

**Output:**

![Alt Text](https://github.com/Goldo02/gif_art_engine_for_nft/blob/main/example/20.gif)

### What functionality does it offer?

* Generate GIFs uniquely randomly or by selecting a rarity for each individual layer.

* Generation of a preview GIF containing the first _n_ GIFs generated.

* Modification of JSON (Metadata) after GIF generation.

* After generating a collection you will automatically find in the output folder a file called _rarity-list_ that will tell you the percentage of presence of each layer in the collection.

## Project Structure
```
├── layers
│   ├── 1-background
│   │        ├── 2e3338.gif
│   │        ├── f8f9f9.gif
│   │        ...
│   ├── 2-character
│   │       ├── alien-male_180.gif
│   │       ├── ape-alien-male.gif
│   │       ...
│   ├── 3-eyes
│   │     ├── 3dglasses.gif
│   │     ├── bigshades.gif
│   │     ...
│   ├── 4-head
│   │     ├── bandana.gif
│   │     ├── beanie.gif
│   │     ...
│   ├── 5-lip
│   │     ├── lipstick-burgundy.gif
│   │     ├── lipstick-flashy_blue.gif
│   ...   ...
├── library 
│      ├── dnaGenerator.cpp
│      ├── dnaGenerator.hpp
│      ├── metadataFunction.cpp
│      ├── metadataFunction.hpp
│      ├── myFFmpegConversion.cpp
│      ├── myFFmpegConversion.hpp
│      ├── utils.cpp
│      └── utils.hpp
├── output
│     ├── images
│     │     ├── 1.gif
│     │     ├── 2.gif
│     │     ...
│     ├── json
│     │    ├── _metadata.json
│     │    ├── 1.json
│     │    ├── 2.json
│     │    ...
│     ├── preview_gif.gif
│     └── rarity_list
├── src
│    ├── main.cpp
│    ├── preview_gif.cpp
│    └── update_info.cpp
├── tmp
│    ├── 1.mp4
│    ├── 2.mp4
│    ├── 3.mp4
│    ├── 4.mp4
│    ...
│    ├── punksDna
│    └── listOfVideo.txt
├── example
├── .gitignore 
├── LICENSE
└── README.md
```
* example: folder that contains the example shown in the introduction.

* layers: folder where the subfolders of each animated layer will be placed..

* library: folder that contains some libraries written by me. The .hpp files contain the declarations, while the .cpp files contain the definitions. Explanation of the files:
    * dnaGenerator: contains all the functions to generate the dna of your NFTs. Either randomly or based on rarity.
    * metadataFunction: contains all the functions used to generate the metadata of your NFTs.
    * myFFmpegConversion: contains useful functions for overlaying layers and converting videos to gifs and vice versa.
    * utils: contains additional features to the standard ones.

* output: will contain the result of the program execution and therefore all the generated gifs and JSONs. There will also be the rarity-list file and the preview-gif.gif. file.

* src: contains the main files to compile together with the libraries.

* tmp: folder where temporary files are stored.

## Installation, compilation and execution

Requirements for linux: FFmpeg installed

Requirements for windows: FFmpeg and MinGW installed

On linux if you haven't installed FFmpeg yet, install it with the following command in the terminal:
```
sudo apt update & sudo apt install ffmpeg -y
```
On windows to install FFmpeg and MinGW watch these videos:

1) FFmpeg: https://www.youtube.com/watch?v=r1AtmY-RMyQ

2) MinGW: https://www.youtube.com/watch?v=guM4XS43m4I&t=305s

The code was tested with FFmpeg version 4.2.4.

If you want to clone the project, run this command:
```
git clone https://github.com/Goldo02/gif_art_engine_for_nft.git
```
After doing so, move to the folder where you cloned the project and go to the _src_ folder.

To compile the _main.cpp_ file use the command: 
```
g++ ../library/dnaGenerator.cpp ../library/metadataFunction.cpp ../library/myFFmpegConversion.cpp ../library/utils.cpp main.cpp -o main
```

To execute:
```
./main
```

To compile the _update-info.cpp_ file, use the command:
```
g++ ../library/utils.cpp ../library/metadataFunction.cpp update_info.cpp -o update_info
```

To execute:
```
./update_info
```

To compile the file _preview-gif.cpp_ use the command: 
```
g++ ../library/myFFmpegConversion.cpp preview_gif.cpp -o preview_gif
```

To execute:
```
./preview_gif
```

Note: remember that every time you modify one of the three files you must also recompile it before running it, otherwise you will run the program out of date with the changes you made.

## Usage
To use the algorithm and modify it according to your needs you must open the file _main.cpp_. Inside it you can distinguish three different sections: layer management, metadata management, rarity management.
Description of variables:

* Layer Management:
    * numbOfPunkToGen: contains the total number of gifs you want to generate
    
    * scale: contains the size in px of the images

    * randomized: set to true if you want to generate a collection randomly, regardless of rarity. Set to false generates the collection according to the rarity set later.

    * layerDir: vector containing the name of the folders of each layer

    * singleLayer: multidimensional vector that contains the name of every single sprite-sheet (in gif format) in every single layer folder. The name of these must match the name of the file in the folders. The use of spaces is not allowed.
    
* Metadata management:
    * singleLayerName: name you want to appear in the metadata regarding the specified sprite-sheet..
    
    * name: collection name.
    
    * description: collection description. 
    
    * image: links where gifs and JSONs will be hosted.
    
    * extraMetadata: vector that contains some metadata you want to add to each JSON.
* Rarity Management:
    * rarityList: multidimensional vector of numbers that contains the probability that character has of being selected. The sum of each probability entered for a specific layer must make 100.
    
This is an example with a collection of mine:
```c++
//LAYER MANAGEMENT
const int numbOfPunkToGen = 1500;
const int scale = 240;
const bool randomized = false;
const vector<string> layerDir = {"1-background", "2-character", "4-head", "3-eyes", "5-lip"};
const vector<vector<string>> singleLayer = 
{
{"Celo_Dark.gif", "Celo_Gray.gif", "Faint_Gold.gif"}, 

{"Ape_Male_Alien.gif", "Blue_Cool_Woman.gif", "Dark_Female_Human.gif", "Dark_Male_Human.gif", "Darker_Male_Human.gif", "Female_Alien.gif", "Female_Ape.gif", "Female_Orc.gif", "Female_Zombie.gif", "Goddess_Night_Woman.gif", "Light_Male_Human.gif", "Lighter_Female_Human.gif", "Lighter_Male_Human.gif", "Male_Alien.gif", "Male_Ape.gif", "Male_Mummy.gif", "Male_Orc.gif", "Male_Robot.gif", "Male_Vampire.gif", "Male_Zombie_Ape.gif", "Male_Zombie.gif", "Red_Burning_Woman.gif"},

{"Bandana.gif", "Beanie.gif", "Blue_Cap.gif", "Cap_Forward.gif", "Cowboy_Hat.gif", "Crazy_Hair.gif", "Fedora.gif", "Green_Cap.gif", "Knitted_Cap.gif", "Male_Wild_Hair.gif", "Top_Hat.gif", "Wallstreetbets_Dark_Crazy_Hair.gif", "Wallstreetbets_Hair.gif"},

{"3D_Glasses.gif", "Big_Shades.gif", "Classic_Shades.gif", "Empty.gif", "Eyepatch.gif", "Male_Earring.gif", "Nerd_Glasses.gif", "Regular_Shades.gif"},

{"Burgundy_Lipstick.gif", "Flashy_Blue_Lipstick.gif", "Gold_Lipstick.gif", "Party_Pink_Lipstick.gif", "Passion_Red_Lipstick.gif", "Purple_Lipstick.gif", "Space_Lipstick.gif", "Pipe.gif"}
};

//METADATA MANAGEMENT
const vector<vector<string>> singleLayerName = 
{
{"Celo Dark", "Celo Gray", "Faint Gold"}, 

{"Ape Male Alien", "Blue Cool Woman", "Dark Female Human", "Dark Male Human", "Darker Male Human", "Female Alien", "Female Ape", "Female Orc", "Female Zombie", "Goddess Night Woman", "Light Male Human", "Lighter Female Human", "Lighter Male Human", "Male Alien", "Male Ape", "Male Mummy", "Male Orc", "Male Robot", "Male Vampire", "Male Zombie Ape", "Male Zombie", "Red Burning Woman"},

{"Bandana", "Beanie", "Blue Cap", "Cap Forward", "Cowboy Hat", "Crazy Hair", "Fedora", "Green Cap", "Knitted Cap", "Male Wild Hair", "Top Hat", "Wallstreetbets Dark Crazy Hair", "Wallstreetbets Hair"},

{"3D Glasses", "Big Shades", "Classic Shades", "Empty", "Eyepatch", "Male Earring", "Nerd Glasses", "Regular Shades"},

{"Burgundy Lipstick", "Flashy Blue Lipstick", "Gold Lipstick", "Party Pink Lipstick", "Passion Red Lipstick", "Purple Lipstick", "Space Lipstick", "Pipe"}
};
const string name = "CeloPunk Animated Edition";
const string description = "CeloPunks Animated Edition is a collection of 1500 unique special animated punks. Not affiliated with LarvaLabs.";
const string image = "";
const vector<pair<string,string>> extraMetadata = 
{
{"Compiler", "CeloPunks Algorithm"}
};

//RARITY MANAGEMENT
const vector<vector<int>> rarityList = 
{
{20, 30, 50}, //3
{1, 7, 7, 7, 7, 4, 4, 4, 4, 6, 9, 9, 9, 2, 2, 2, 3, 2, 2, 2, 3, 4}, //22
{11, 1, 13, 11, 3, 1, 4, 13, 11, 3, 13, 13, 3}, //13
{6, 6, 12, 20, 12, 20, 9, 15}, //8
{18, 16, 5, 20, 5, 17, 16, 3}, //8
};
```

### Update metadata
Open the _update-info.cpp_ file located in the _src_ folder. Here too you will find a number of variables that you can modify. Modify them according to your needs and then run the code to update the metadata. 

Example:
```c++
const string name = "CeloPunk Animated Edition";
const string description = "CeloPunks Animated Edition is a collection of 1500 unique special animated punks. Not affiliated with LarvaLabs.";
const string image = "";
const int numbOfPunkToGen = 1500;
const int nLayers = 5;
const vector<pair<string,string>> &extraMetadata = 
{
{"Compiler:", "CeloPunks Algorithm"}
};
```

### Creating a gif preview
Open the _preview-gif.cpp_ file located in the _src_ folder. In this file the only parameter present is _numbOfPunkToGen_ which is a number that indicates the first _n_ GIFs that must be taken from the collection to generate the preview.

Example:
```c++
const int numbOfPunkToGen = 350;
```

This way the first 350 GIFs in our collection will be used to generate a preview.
