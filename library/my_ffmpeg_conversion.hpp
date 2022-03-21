#ifndef FFMPEG_CONVERSION_H
#define FFMPEG_CONVERSION_H

#include <vector>
#include <string>

using namespace std;

void mergeAllAudioWithVideo(const int &, const string&, const string&, const string&, const int &);
void generateAllMedia(const vector<string> &, const vector<vector<string>> &, vector<int> &, const int&, const int&, const vector<string> &, const string &, const string &);
void generatePreviewGif(const int &, const int &, const int &, const int &fps);
void addEmptyTrackAudioToVideos();

#endif
