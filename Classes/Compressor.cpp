//
//  Compressor.cpp
//  Doodler
//
//  Created by Liang Fan on 7/1/15.
//
//


#include "Compressor.h"
#include "SceneManager.h"

static Compressor* sharedCompressor = nullptr;

Compressor* Compressor::getInstance()
{
    if (! sharedCompressor)
    {
        sharedCompressor = new Compressor();
    }
    return sharedCompressor;
}

std::string Compressor::compressData(std::vector<LineData> dataList)
{
    return JSONPacker::packLineData(dataList);
}

std::vector<LineData> Compressor::unCompressData(std::string json)
{
    return JSONPacker::unpackLineDataJSON(json);
}

Compressor::Compressor()
{
    
}

Compressor::~Compressor()
{

}