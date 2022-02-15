#include <iostream>
#include "../library/myFFmpegConversion.hpp"

using namespace std;

int main()
{
    const int numbOfMediaToGen = 150;
    const int scale = 240;
    generatePreviewGif(numbOfMediaToGen, scale);
    
 return 0;
}
