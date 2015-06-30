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
#include "Constants.h"

namespace JSONPacker
{
    std::vector<LineData> unpackLineDataJSON(std::string json);
    std::string packLineData(std::vector<LineData> lineDatalist);
}

#endif /* defined(__Doodler__JSONPacker__) */
