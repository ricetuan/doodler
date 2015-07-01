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
#include "Constants.h"
#include "zlib.h"

class DrawingCanvas;
class Compressor
{
public:
    Compressor();
    ~Compressor();
    
    std::string compress_string(const std::string& str,int compressionlevel = Z_BEST_SPEED);
    std::string decompress_string(const std::string& str);
    
    static Compressor* getInstance();
    void compressData(std::vector<LineData> dataList, std::string &data);
    std::vector<LineData> unCompressData(std::string);
    
    
private:
    std::vector<LineData> bufferList;

};
#endif /* defined(__Doodler__Compressor__) */
