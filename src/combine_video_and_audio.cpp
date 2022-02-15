#include <iostream>
#include "../library/myFFmpegConversion.hpp"

using namespace std;

int main()
{
    const int numbOfVideoAndAudio = 1;
    const string inputVideoForm = "mp4";
    const string inputAudioForm = "mp3";
    const string outputVideoForm = "mp4";
    mergeAllAudioWithVideo(inputVideoForm, inputAudioForm, outputVideoForm, numbOfVideoAndAudio);
 return 0;
}
