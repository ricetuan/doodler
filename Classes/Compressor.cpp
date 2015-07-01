//
//  Compressor.cpp
//  Doodler
//
//  Created by Liang Fan on 7/1/15.
//
//


#include "Compressor.h"
#include "SceneManager.h"
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <zlib.h>

static Compressor* sharedCompressor = nullptr;

Compressor* Compressor::getInstance()
{
    if (! sharedCompressor)
    {
        sharedCompressor = new Compressor();
    }
    return sharedCompressor;
}

void Compressor::compressData(std::vector<LineData> dataList, std::string &data)
{
    std::string rawData = JSONPacker::packLineData(dataList);
    CCLOG("data size before compress is %lu",rawData.size());
    data = compress_string(rawData);
    CCLOG("data size after compress is %lu",data.size());
}

std::vector<LineData> Compressor::unCompressData(std::string json)
{
    return JSONPacker::unpackLineDataJSON(decompress_string(json));
}

std::string Compressor::compress_string(const std::string& str,int compressionlevel)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));
    
    if (deflateInit(&zs, compressionlevel) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));
    
    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();           // set the z_stream's input
    
    int ret;
    char outbuffer[32768];
    std::string outstring;
    
    // retrieve the compressed bytes blockwise
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        
        ret = deflate(&zs, Z_FINISH);
        
        if (outstring.size() < zs.total_out) {
            // append the block to the output string
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);
    
    deflateEnd(&zs);
    
    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }
    
    return outstring;
}

/** Decompress an STL string using zlib and return the original data. */
std::string Compressor::decompress_string(const std::string& str)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));
    
    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));
    
    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();
    
    int ret;
    char outbuffer[32768];
    std::string outstring;
    
    // get the decompressed bytes blockwise using repeated calls to inflate
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        
        ret = inflate(&zs, 0);
        
        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }
        
    } while (ret == Z_OK);
    
    inflateEnd(&zs);
    
    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") "
        << zs.msg;
        throw(std::runtime_error(oss.str()));
    }
    
    return outstring;
}

Compressor::Compressor()
{
    
}

Compressor::~Compressor()
{

}