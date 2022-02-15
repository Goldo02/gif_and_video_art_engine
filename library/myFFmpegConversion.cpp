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
    
    //cout << endl << systemCall << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        cerr << "command executed: " + systemCall << endl;
        exit(1);
    }
    return;
}

static void convertMp4ToGif(const string pathMp4, const string pathGif, const int &scale)
{
    string systemCall = "ffmpeg -i " + pathMp4 + " -vf \"fps=30,scale=" + to_string(scale) + ":-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\" -loop 0 " + pathGif + " -y";
    
    //cout << endl << systemCall << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        cerr << "command executed: " + systemCall << endl;
        exit(1);
    }
    return;
}

static void convertVideo(string inputPath, string outputPath, string format, const int &scale)
{
    string systemCall = "ffmpeg -i " + inputPath + " -vf scale=" + to_string(scale) + ":-1 -f " + format + " " + outputPath + " -y";

    //cout << endl << systemCall << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        cerr << "command executed: " + systemCall << endl;
        exit(1);
    }
    return;
}

static void convertVideo(const string inputPath, const string outputPath, const string format)
{
    string systemCall = "ffmpeg -i " + inputPath + " -f " + format + " " + outputPath + " -y";

    //cout << endl << systemCall << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        cerr << "command executed: " + systemCall << endl;
        exit(1);
    }
    return;
}

static void overlapTwoMediaInAVideo(const string pathTopFrame, const string pathUnderFrame, const string pathWhereToSave)
{
    string systemCall = "ffmpeg -i " + pathTopFrame + " -i " + pathUnderFrame + " -filter_complex \"[1]format=rgba,colorchannelmixer=aa=1[front];[0][front]overlay=x=(W-w)/2:y=H-h,format=yuv420p\" " + pathWhereToSave + " -y";
    
    //cout << endl << systemCall << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        cerr << "command executed: " + systemCall << endl;
        exit(1);
    }
    return;
}

static void overlapTwoAudio(const string firstAudioPath, const string seconAudioPath, const string outputAudioPath)
{
    string systemCall = "ffmpeg -i " + firstAudioPath + " -i " + seconAudioPath + " -filter_complex amix=inputs=2:duration=longest " + outputAudioPath + " -y";
    
    //cout << endl << systemCall << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        cerr << "command executed: " + systemCall << endl;
        exit(1);
    }
    return;
}

static void convertGifToMp4(const int &n)
{
    string systemCall = "ffmpeg -i ../output/media/"+ to_string(n) +".gif -vf \"scale=trunc(iw/2)*2:trunc(ih/2)*2\" ../tmp/" + to_string(n) + ".mp4" + " -y";
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        cerr << "command executed: " + systemCall << endl;
        exit(1);
    }
    return;
}

void mergeAllAudioWithVideo(const string &inputVideoForm, const string &inputAudioForm, const string &outputVideoForm, const int &numbOfMediaToGen)
{
    for(int i=1;i<=numbOfMediaToGen;++i){
        mergeAudioWithVideo("../tmp/" + to_string(i) + "." + inputVideoForm, "../tmp/" + to_string(i) + "." + inputAudioForm, "../tmp/" + to_string(i) + "_.mp4");
        convertVideo("../tmp/" + to_string(i) + "_.mp4", "../output/video_with_audio/" + to_string(i) + "." + outputVideoForm, outputVideoForm);
    }
    return;
}

void generatePreviewGif(const int &numbOfMediaToGen, const int &scale)
{
    ofstream fout;
    string systemCall = "ffmpeg -f concat -safe 0 -i ../tmp/listOfVideo.txt -c copy ../tmp/preview_gif.mp4 -y";
    
    for(int i=1;i<=numbOfMediaToGen;++i)
        convertGifToMp4(i);
    
    fout.open("../tmp/listOfVideo.txt");
    if(fout.fail()){
        cerr << "errore durante l'apertura del file listOfVideo" << endl;
        exit(2);
    }
    
    for(int i=1;i<=numbOfMediaToGen;++i)
        fout << "file " + to_string(i) + ".mp4" << endl; 
    
    fout.close();
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed..." << endl;
        cerr << "command executed: " + systemCall << endl;
        exit(1);
    }
    
    convertMp4ToGif("../tmp/preview_gif.mp4", "../output/preview_gif.gif", scale);
    return;
}

void generateAllMedia(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, vector<int> &collectionSize, const int& collIndex, const int &scale, const vector<string> &dnaOfAllMedia, const string &outputFormat)
{
    vector<int> mediaDna((int)layerDir.size());
    if(layerDir.size()<=1){
        cerr << "hai solo una cartella cosa vuoi fare?" << endl;
        exit(3);
    }
    
    for(int k=collectionSize[collIndex-1]+1;k<=collectionSize[collIndex];++k){
        extractInteger(dnaOfAllMedia[k-1], mediaDna);
        if(outputFormat=="mp3" || outputFormat=="wav"){
            overlapTwoAudio("../layers/" + layerDir[0] + "/" + singleLayer[0][mediaDna[0]], "../layers/" + layerDir[1] + "/" + singleLayer[1][mediaDna[1]], "../tmp/1." + outputFormat);
            for(int i=2;i<(int)layerDir.size()-1;++i)
                overlapTwoAudio("../tmp/" + to_string(i-1) + "." + outputFormat, "../layers/" + layerDir[i] + "/" + singleLayer[i][mediaDna[i]], "../tmp/" + to_string(i) + "." + outputFormat);
            overlapTwoAudio("../tmp/" + to_string((int)layerDir.size()-2) + "." + outputFormat, "../layers/" + layerDir[(int)layerDir.size()-1] + "/" + singleLayer[(int)layerDir.size()-1][mediaDna[(int)layerDir.size()-1]], "../output/media/" + to_string(k) + "." + outputFormat);
        }
        else{
            overlapTwoMediaInAVideo("../layers/" + layerDir[0] + "/" + singleLayer[0][mediaDna[0]], "../layers/" + layerDir[1] + "/" + singleLayer[1][mediaDna[1]], "../tmp/1.mp4");
            for(int i=2;i<(int)layerDir.size();++i){
                cout << "../tmp/" + to_string(i-1) + ".mp4" << endl;
                cout << "../layers/" + layerDir[i] + "/" + singleLayer[i][mediaDna[i]] << endl;
                cout << layerDir[i] << endl;
                cout << singleLayer[i][mediaDna[i]] << endl;
                overlapTwoMediaInAVideo("../tmp/" + to_string(i-1) + ".mp4", "../layers/" + layerDir[i] + "/" + singleLayer[i][mediaDna[i]], "../tmp/" + to_string(i) + ".mp4");
            }
            if(outputFormat=="gif")
                convertMp4ToGif("../tmp/" + to_string((int)layerDir.size()-1) + ".mp4", "../output/media/" + to_string(k) + ".gif", scale);
            else
                convertVideo("../tmp/" + to_string((int)layerDir.size()-1) + ".mp4", "../output/media/" + to_string(k) + "." + outputFormat, outputFormat, scale);
        }
    }
    
    return;
}

/*
system("ffmpeg -i 1.gif -i 2.gif -filter_complex \"[1]format=rgba,colorchannelmixer=aa=1[front];[0][front]overlay=x=(W-w)/2:y=H-h,format=yuv420p\" output.mp4") -> prende la seconda gif e la mette sopra la prima gif e crea un video

system(ffmpeg -i output.mp4 -vf \"fps=30,scale=384:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\" -loop 0 output.gif) -> prende il video precedentemente creato e lo ritrasforma in una gif

*/
