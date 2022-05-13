#include "my_ffmpeg_conversion.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

static void mergeAudioWithVideo(string videoPath, string audioPath, string outputPath)
{
    clog << "Entered the function: 'mergeAudioWithVideo'" << endl;
    
    string systemCall = "ffmpeg -i " + videoPath + " -i " + audioPath + " -c:v copy -c:a aac " + outputPath + " -y";
    
    clog << "Executing the system call: " + systemCall << " ..." << endl;
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'mergeAudioWithVideo': the command executed was " + systemCall << endl;
        exit(1);
    }
    clog << "Done" << endl;
    clog << "Exiting the function: 'mergeAudioWithVideo'" << endl;
    return;
}

static void convertMp4ToGif(const string pathMp4, const string pathGif, const int &scale, const int &fps)
{
    clog << "Entered the function: 'convertMp4ToGif'" << endl;
    
    string systemCall = "ffmpeg -i " + pathMp4 + " -vf \"fps=" + to_string(fps) + ",scale=" + to_string(scale) + ":-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\" -loop 0 " + pathGif + " -y";
    clog << "Executing the system call: " + systemCall << " ..." << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'convertMp4ToGif': the command executed was " + systemCall << endl;
        exit(1);
    }
    
    clog << "Done" << endl;
    clog << "Exiting the function: 'convertMp4ToGif'" << endl;
    return;
}

static void convertVideo(const string inputPath, const string outputPath, const string format)
{
    clog << "Entered the function: 'convertVideo'" << endl;
    
    string systemCall = "ffmpeg -i " + inputPath + " -f " + format + " " + outputPath + " -y";

    clog << "Executing the system call: " + systemCall << " ..." << endl;
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'convertVideo': the command executed was " + systemCall << endl;
        exit(1);
    }
    
    clog << "Done" << endl;
    clog << "Exiting the function: 'convertVideo'" << endl;
    return;
}

static void convertGifToMp4(const int &n)
{
    clog << "Entered the function: 'convertGifToMp4'" << endl;
    
    string systemCall = "ffmpeg -i ./output/media/"+ to_string(n) +".gif -vf \"scale=trunc(iw/2)*2:trunc(ih/2)*2\" ./tmp/" + to_string(n) + ".mp4" + " -y";
    clog << "Executing the system call: " + systemCall << " ..." << endl;
    
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'convertGifToMp4': the command executed was " + systemCall << endl;
        exit(1);
    }
    
    clog << "Done" << endl;
    clog << "Exiting the function: 'convertGifToMp4'" << endl;
    return;
}

void mergeAllAudioWithVideo(const int &firstMedia, const string &inputVideoForm, const string &inputAudioForm, const string &outputVideoForm, const int &numbOfMediaToGen)
{
    clog << "Entered the function: 'mergeAllAudioWithVideo'" << endl;
    for(int i=firstMedia;i<firstMedia+numbOfMediaToGen;++i){
        mergeAudioWithVideo("./tmp/" + to_string(i) + "." + inputVideoForm, "./tmp/" + to_string(i) + "." + inputAudioForm, "./tmp/" + to_string(i) + "_.mp4");
        convertVideo("./tmp/" + to_string(i) + "_.mp4", "./output/video_with_audio/" + to_string(i) + "." + outputVideoForm, outputVideoForm);
    }
    clog << "Exiting the function: 'mergeAllAudioWithVideo'" << endl;
    return;
}

void generatePreviewGif(const int &firstMedia, const int &numbOfMediaToGen, const int &scale, const int &fps)
{
    clog << "Entered the function: 'generatePreviewGif'" << endl;
    
    ofstream fout;
    string systemCall = "ffmpeg -f concat -safe 0 -i ./tmp/listOfVideo.txt -c copy ./tmp/preview_gif.mp4 -y";
    
    clog << "Conversion of " + to_string(numbOfMediaToGen) + "GIFs to MP4 started" << endl;
    for(int i=firstMedia;i<firstMedia+numbOfMediaToGen;++i)
        convertGifToMp4(i);
    clog << "Conversion done" << endl;
    
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
    
    clog << "Exiting the function: 'generatePreviewGif'" << endl;
    return;
}

static void generateSingleMedia(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, const vector<int> &mediaDna, const string &outputFormat, const int &scale, const int &index)
{
    clog << "Entered the function: 'generateSingleMedia'" << endl;
    int i;
    string systemCall = "ffmpeg -i ./layers/" + layerDir[0] + "/" + singleLayer[0][mediaDna[0]] + " -i ./layers/" + layerDir[1] + "/" + singleLayer[1][mediaDna[1]];
    
    for(i=2;i<(int)layerDir.size();++i)
        systemCall += " -i ./layers/" + layerDir[i] + "/" + singleLayer[i][mediaDna[i]];
    
    systemCall += " -filter_complex \"";
    systemCall += "[0][1]overlay=format=auto";
	if((int)layerDir.size()-2<=0)
		systemCall += ",scale=" + to_string(scale) + ":-1,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse";
	else{
		systemCall += "[i1];";
		for(i=1;i<=(int)layerDir.size()-3;++i)
			systemCall += "[i" + to_string(i) + "][" + to_string(i+1) + "]overlay=format=auto[i" + to_string(i+1) + "];";
		systemCall += "[i" + to_string(i) + "][" + to_string(i+1) + "]overlay=format=auto,scale=" + to_string(scale) + ":-1,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse";
    }
	
    if(outputFormat=="gif" || outputFormat=="mp3" || outputFormat=="wav" || outputFormat=="png" || outputFormat == "jpg")
        systemCall += "\" ";
    else
        systemCall += ";amix=inputs=" + to_string((int)layerDir.size()) + ":duration=longest\" ";
    systemCall += "./output/media/" + to_string(index) + "." + outputFormat + " -y";
        
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'generateSingleMedia': the command executed was " + systemCall << endl;
        exit(1);
    }
    clog << "Exiting the function: 'generateSingleMedia'" << endl;
    return;
}

void generateAllMedia(const vector<string> &layerDir, const vector<vector<string>> &singleLayer, vector<int> &collectionSize, const int& collIndex, const int &scale, const vector<string> &dnaOfAllMedia, const string &outputFormat, const string &chain)
{
    clog << "Exiting the function: 'generateAllMedia'" << endl;
    vector<int> mediaDna((int)layerDir.size());
    int adjustForChain = 0;
    if(layerDir.size()<=1){
        cerr << "in function 'generateAllMedia': the numbers of layers is too low" << endl;
        exit(3);
    }
    
    if(chain=="SOL")
        adjustForChain = 1;
    
    for(int k=collectionSize[collIndex-1]+1-adjustForChain;k<=collectionSize[collIndex]-adjustForChain;++k){
        extractInteger(dnaOfAllMedia[k-1+adjustForChain], mediaDna);
        generateSingleMedia(layerDir, singleLayer, mediaDna, outputFormat, scale, k);
    }
    clog << "Exiting the function: 'generateAllMedia'" << endl;
    return;
}

static void addEmptyAudioTraceIfThereIsNoOne(string pathInput, string pathOutput)
{
    clog << "Exiting the function: 'addEmptyAudioTraceIfThereIsNoOne'" << endl;
    string systemCall = "ffmpeg -f lavfi -i anullsrc=channel_layout=stereo:sample_rate=44100 -i " + pathInput + " -c:v copy -c:a aac -shortest " + pathOutput + " -y";
    if(system(systemCall.c_str())!=0){
        cerr << "ffmpeg failed.." << endl;
        cerr << "in function 'addEmptyAudioTraceIfThereIsNoOne': the command executed was " + systemCall << endl;
        exit(1);
    }
    clog << "Exiting the function: 'addEmptyAudioTraceIfThereIsNoOne'" << endl;
    return;
}

void addEmptyTrackAudioToVideos()
{
    clog << "Exiting the function: 'addEmptyTrackAudioToVideos'" << endl;
    ifstream fin;
    vector<string> layerDir;
    string buffer;
    
    createLsFile("./layers/", "./tmp/list_layers.txt");
    fin.open("./tmp/list_layers.txt");
    if(fin.fail()){
        cerr << "in function 'addEmptyTrackToVideos': error while opening in reading the file ./tmp/shuffle_code.txt" << endl;
        exit(2);
    }
    
    while(fin>>buffer)
        layerDir.push_back(buffer);
    fin.close();
    
    for(int i=0;i<(int)layerDir.size();++i){
        createLsFile("./layers/" + layerDir[i], "./tmp/list_layers.txt");
        fin.open("./tmp/list_layers.txt");
        if(fin.fail()){
            cerr << "in function 'addEmptyTrackToVideos': error while opening in reading the file ./tmp/shuffle_code.txt" << endl;
            exit(2);
        }
        while(fin>>buffer){
            addEmptyAudioTraceIfThereIsNoOne("./layers/" + layerDir[i] + "/" + buffer, "./layers/" + layerDir[i] + "/_" + buffer);
            deleteFile("./layers/" + layerDir[i] + "/" + buffer);
            rename(string("./layers/" + layerDir[i] + "/_" + buffer).c_str(), string("./layers/" + layerDir[i] + "/" + buffer).c_str());
        }
        
        fin.close();
    }
    clog << "Exiting the function: 'addEmptyTrackAudioToVideos'" << endl;
    
    return;
}
