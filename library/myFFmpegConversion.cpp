#include "myFFmpegConversion.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>

using namespace std;

static void convertMp4ToGif(string pathMp4, string pathGif, const int scale)
{
    string systemCall = "ffmpeg -i " + pathMp4 + " -vf \"fps=30,scale=" + to_string(scale) + ":-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\" -loop 0 " + pathGif + " -y";
    
    //cout << endl << systemCall << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        exit(1);
    }
    return;
}

static void overlapTwoGifsInAVideo(string pathTopFrame, string pathUnderFrame, string pathWhereToSave)
{
    string systemCall = "ffmpeg -i " + pathTopFrame + " -i " + pathUnderFrame + " -filter_complex \"[1]format=rgba,colorchannelmixer=aa=1[front];[0][front]overlay=x=(W-w)/2:y=H-h,format=yuv420p\" " + pathWhereToSave + " -y";
    
    //cout << endl << systemCall << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        exit(1);
    }
    return;
}

static void convertGifToMp4(const int &n)
{
    string systemCall = "ffmpeg -i ../output/images/"+ to_string(n) +".gif -movflags faststart -pix_fmt yuv420p -vf \"scale=trunc(iw/2)*2:trunc(ih/2)*2\" ../tmp/" + to_string(n) + ".mp4" + " -y";
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        exit(1);
    }
    return;
}

void generatePreviewGif(const int &numbOfPunkToGen)
{
    ofstream fout;
    string systemCall = "ffmpeg -f concat -safe 0 -i ../tmp/listOfVideo.txt -c copy ../tmp/preview_gif.mp4 -y";
    
    for(int i=1;i<=numbOfPunkToGen;++i)
        convertGifToMp4(i);
    
    fout.open("../tmp/listOfVideo.txt");
    if(fout.fail()){
        cerr << "errore durante l'apertura del file listOfVideo" << endl;
        exit(2);
    }
    
    for(int i=1;i<=numbOfPunkToGen;++i)
        fout << "file " + to_string(i) + ".mp4" << endl; 
    
    fout.close();
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        exit(1);
    }
    
    convertMp4ToGif("../tmp/preview_gif.mp4", "../output/preview_gif.gif", 240);
    return;
}

void generateAllPunks(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const int numbOfPunkToGen, const int scale)
{
    vector<string> punkDna((int)layerDir.size());
    ifstream fin("../tmp/punksDna");
    if(fin.fail()){
        cerr << "errore durante l'apertura del file punksDna" << endl;
        exit(2);
    }
    if(layerDir.size()<=1){
        cerr << "hai solo una cartella cosa vuoi fare?" << endl;
        exit(3);
    }
    int currGen = 1;
    
    while(currGen<=numbOfPunkToGen){
        for(int i=0;i<(int)layerDir.size();++i)
            fin >> punkDna[i];
        overlapTwoGifsInAVideo("../layers/" + layerDir[0] + "/" + singleLayer[0][atoi(punkDna[0].c_str())], "../layers/" + layerDir[1] + "/" + singleLayer[1][atoi(punkDna[1].c_str())], "../tmp/1.mp4");
        for(int i=2;i<(int)layerDir.size();++i)
            overlapTwoGifsInAVideo("../tmp/" + to_string(i-1) + ".mp4", "../layers/" + layerDir[i] + "/" + singleLayer[i][atoi(punkDna[i].c_str())], "../tmp/" + to_string(i) + ".mp4");
        convertMp4ToGif("../tmp/" + to_string((int)layerDir.size()-1) + ".mp4", "../output/images/" + to_string(currGen) + ".gif", scale);
        ++currGen;
    }
    fin.close();
    return;
}

/*
system("ffmpeg -i 1.gif -i 2.gif -filter_complex \"[1]format=rgba,colorchannelmixer=aa=1[front];[0][front]overlay=x=(W-w)/2:y=H-h,format=yuv420p\" output.mp4") -> prende la seconda gif e la mette sopra la prima gif e crea un video

system(ffmpeg -i output.mp4 -vf \"fps=30,scale=384:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\" -loop 0 output.gif) -> prende il video precedentemente creato e lo ritrasforma in una gif

*/
