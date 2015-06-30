//
//  Compressor.h
//  Doodler
//
//  Created by Liang Fan on 7/1/15.
//
//

#ifndef __Doodler__Compressor__
#define __Doodler__Compressor__

#include "JSONPacker.h"
#include "zlib.h"
#include "Constants.h"

class DrawingCanvas;
class Compressor
{
public:
    Compressor();
    ~Compressor();
    
    static Compressor* getInstance();
    std::string compressData(std::vector<LineData> dataList);
    std::vector<LineData> unCompressData(std::string);
    
    
private:
    std::vector<LineData> bufferList;

};
#endif /* defined(__Doodler__Compressor__) */
