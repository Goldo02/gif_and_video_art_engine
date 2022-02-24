#include "myFFmpegConversion.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>

using namespace std;

static void mergeAudioWithVideo(string videoPath, string audioPath, string outputPath)
{
    string systemCall = "ffmpeg -i " + videoPath + " -i " + audioPath + " -c:v copy -c:a aac " + outputPath + " -y";
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'mergeAudioWithVideo': the command executed was " + systemCall << endl;
        exit(1);
    }
    return;
}

static void convertMp4ToGif(const string pathMp4, const string pathGif, const int &scale, const int &fps)
{
    string systemCall = "ffmpeg -i " + pathMp4 + " -vf \"fps=" + to_string(fps) + ",scale=" + to_string(scale) + ":-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\" -loop 0 " + pathGif + " -y";
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'convertMp4ToGif': the command executed was " + systemCall << endl;
        exit(1);
    }
    return;
}

static void convertVideo(const string inputPath, const string outputPath, const string format)
{
    string systemCall = "ffmpeg -i " + inputPath + " -f " + format + " " + outputPath + " -y";
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'convertVideo': the command executed was " + systemCall << endl;
        exit(1);
    }
    return;
}

static void convertGifToMp4(const int &n)
{
    string systemCall = "ffmpeg -i ./output/media/"+ to_string(n) +".gif -vf \"scale=trunc(iw/2)*2:trunc(ih/2)*2\" ./tmp/" + to_string(n) + ".mp4" + " -y";
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'convertGifToMp4': the command executed was " + systemCall << endl;
        exit(1);
    }
    return;
}

void mergeAllAudioWithVideo(const int &firstMedia, const string &inputVideoForm, const string &inputAudioForm, const string &outputVideoForm, const int &numbOfMediaToGen)
{
    for(int i=firstMedia;i<firstMedia+numbOfMediaToGen;++i){
        mergeAudioWithVideo("./tmp/" + to_string(i) + "." + inputVideoForm, "./tmp/" + to_string(i) + "." + inputAudioForm, "./tmp/" + to_string(i) + "_.mp4");
        convertVideo("./tmp/" + to_string(i) + "_.mp4", "./output/video_with_audio/" + to_string(i) + "." + outputVideoForm, outputVideoForm);
    }
    return;
}

void generatePreviewGif(const int &firstMedia, const int &numbOfMediaToGen, const int &scale, const int &fps)
{
    ofstream fout;
    string systemCall = "ffmpeg -f concat -safe 0 -i ./tmp/listOfVideo.txt -c copy ./tmp/preview_gif.mp4 -y";
    
    for(int i=firstMedia;i<firstMedia+numbOfMediaToGen;++i)
        convertGifToMp4(i);
    
    fout.open("./tmp/listOfVideo.txt");
    if(fout.fail()){
        cerr << "in function 'generatePreviewGif': error while opening in writing the file ./tmp/listOfVideo.txt" << endl;
        exit(2);
    }
    
    for(int i=firstMedia;i<firstMedia+numbOfMediaToGen;++i)
        fout << "file " + to_string(i) + ".mp4" << endl; 
    
    fout.close();
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'generatePreviewGif': the command executed was " + systemCall << endl;
        exit(1);
    }
    
    convertMp4ToGif("./tmp/preview_gif.mp4", "./output/preview_gif.gif", scale, fps);
    return;
}

static void generateSingleMedia(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const vector<int> &mediaDna, const string &outputFormat, const int &scale, const int &index)
{
    int i;
    string systemCall = "ffmpeg -ss 0 -i ./layers/" + layerDir[0] + "/" + singleLayer[0][mediaDna[0]] + " -i ./layers/" + layerDir[1] + "/" + singleLayer[1][mediaDna[1]];
    
    for(i=2;i<(int)layerDir.size();++i)
        systemCall += " -i ./layers/" + layerDir[i] + "/" + singleLayer[i][mediaDna[i]];
    
    systemCall += " -filter_complex \"";
    systemCall += "[0][1]overlay=format=auto[i1];";
    for(i=1;i<=(int)layerDir.size()-3;++i)
        systemCall += "[i" + to_string(i) + "][" + to_string(i+1) + "]overlay=format=auto[i" + to_string(i+1) + "];";
    systemCall += "[i" + to_string(i) + "][" + to_string(i+1) + "]overlay=format=auto,scale=" + to_string(scale) + ":-1,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse";
    
    if(outputFormat=="gif" || outputFormat=="mp3" || outputFormat=="wav")
        systemCall += "\" ";
    else
        systemCall += ";amix=inputs=" + to_string((int)layerDir.size()) + ":duration=longest\" ";
    systemCall += "./output/media/" + to_string(index) + "." + outputFormat + " -y";
    
    cout << systemCall << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'generateSingleMedia': the command executed was " + systemCall << endl;
        exit(1);
    }
    return;
}

void generateAllMedia(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, vector<int> &collectionSize, const int& collIndex, const int &scale, const vector<string> &dnaOfAllMedia, const string &outputFormat, const string &chain)
{
    vector<int> mediaDna((int)layerDir.size());
    int adjustForChain = 0;
    if(layerDir.size()<=1){
        cerr << "in function 'generateAllMedia': the numbers of layers is too low" << endl;
        exit(3);
    }
    
    if(chain=="SOL")
        adjustForChain = 1;
    
    for(int k=collectionSize[collIndex-1]+1-adjustForChain;k<=collectionSize[collIndex]-adjustForChain;++k){
        extractInteger(dnaOfAllMedia[k-1], mediaDna);
        generateSingleMedia(layerDir, singleLayer, mediaDna, outputFormat, scale, k);
    }
    
    return;
}
