#include <vector>
#include <string>

using namespace std;

static void convertMp4ToGif(string, string, const int);
static void convertGifToMp4(const int&);
static void overlapTwoGifsInAVideo(string, string, string);
void generateAllPunks(const vector<string> &, const vector<vector<string>> &, const int, const int);
void generatePreviewGif(const int &);
