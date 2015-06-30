//
//  JSONPacker.h
//  Doodler
//
//  Created by Liang Fan on 6/30/15.
//
//

#ifndef __Doodler__JSONPacker__
#define __Doodler__JSONPacker__

#include "cocos2d.h"
namespace JSONPacker
{
    struct LineData
    {
        cocos2d::Vec2 startPoint;
        cocos2d::Vec2 endPoint;
        cocos2d::Color4F color;
        float radius;
    };
    
    LineData unpackLineDataJSON(std::string json);
    std::string packLineData(LineData lineData);
}

#endif /* defined(__Doodler__JSONPacker__) */
