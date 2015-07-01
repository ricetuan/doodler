//
//  JSONPacker.cpp
//  Doodler
//
//  Created by Liang Fan on 6/30/15.
//
//

#include "JSONPacker.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace cocos2d;

namespace JSONPacker
{

    std::string packLineData(std::vector<LineData> lineDataList)
    {
        rapidjson::Document document;
        document.SetArray();
        
        for (int i=0;i<lineDataList.size(); ++i) {
            rapidjson::Value startPt(rapidjson::kObjectType);
            startPt.AddMember("x", lineDataList[i].startPoint.x, document.GetAllocator());
            startPt.AddMember("y", lineDataList[i].startPoint.y, document.GetAllocator());
            
            rapidjson::Value endPt(rapidjson::kObjectType);
            endPt.AddMember("x", lineDataList[i].endPoint.x, document.GetAllocator());
            endPt.AddMember("y", lineDataList[i].endPoint.y, document.GetAllocator());
            
            rapidjson::Value lineColor(rapidjson::kObjectType);
            lineColor.AddMember("r", lineDataList[i].color.r, document.GetAllocator());
            lineColor.AddMember("g", lineDataList[i].color.g, document.GetAllocator());
            lineColor.AddMember("b", lineDataList[i].color.b, document.GetAllocator());
            lineColor.AddMember("a", lineDataList[i].color.a, document.GetAllocator());
            
            rapidjson::Value linedata(rapidjson::kObjectType);
            linedata.AddMember("startPoint", startPt, document.GetAllocator());
            linedata.AddMember("endPoint", endPt, document.GetAllocator());
            linedata.AddMember("radius", lineDataList[i].radius, document.GetAllocator());
            linedata.AddMember("color", lineColor, document.GetAllocator());

            document.PushBack(linedata,document.GetAllocator());
    }
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        
        CCLOG("%s",buffer.GetString());
        
        return std::string(buffer.GetString(), buffer.Size());
    }
    
    std::vector<LineData> unpackLineDataJSON(std::string json)
    {
        rapidjson::Document document;
        document.Parse<0>(json.c_str());
        std::vector<LineData> resultList;
        for (int i =0; i < document.Capacity();++i)
        {
            const rapidjson::Value& startDoc = document[i]["startPoint"];
            const rapidjson::Value& endDoc = document[i]["endPoint"];
            const rapidjson::Value& colorDoc = document[i]["color"];
            Vec2 start = Vec2(startDoc["x"].GetDouble(), startDoc["y"].GetDouble());
            Vec2 end = Vec2(endDoc["x"].GetDouble(), endDoc["y"].GetDouble());
            float radius = document[i]["radius"].GetDouble();
            
            float r = colorDoc["r"].GetDouble();
            float g = colorDoc["g"].GetDouble();
            float b = colorDoc["b"].GetDouble();
            float a = colorDoc["a"].GetDouble();
            Color4F color = Color4F(r, g, b, a);
            
            LineData result;
            result.startPoint = start;
            result.endPoint = end;
            result.radius = radius;
            result.color = color;
            resultList.push_back(result);
        }

        return resultList;

    }

    
    
}