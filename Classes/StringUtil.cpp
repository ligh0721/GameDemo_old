//
//  StringUtil.cpp
//  GameDemo
//
//  Created by qiuhelin on 13-5-12.
//
//

#include "StringUtil.h"
std::string& CStringUtil::ltrim(std::string& str)
{
    return str.erase(0, str.find_first_not_of(" "));

}

std::string& CStringUtil::rtrim(std::string& str)
{
    return str.erase(str.find_last_not_of(" ") + 1);
}

std::string& CStringUtil::trim(std::string& str)
{
    return rtrim(ltrim(str));
}
int CStringUtil::split(const std::string &split, std::string &str, std::vector<std::string>& v)
{
    std::string::size_type pos1 = 0 ;
    std::string::size_type pos2 =  str.find(split.c_str());
    while (pos2 != std::string::npos)
    {
        std::string i = str.substr(pos1, pos2 - pos1);
        v.push_back(i);
        pos1 = pos2 + 1;
        pos2 = str.find(split.c_str(), pos1);
    }
    v.push_back(str.substr(pos1));
    return v.size();
}