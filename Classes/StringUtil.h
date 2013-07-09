//
//  StringUtil.h
//  GameDemo
//
//  Created by qiuhelin on 13-5-12.
//
//

#ifndef GameDemo_StringUtil_h
#define GameDemo_StringUtil_h
#include <string>
#include <vector>
 class CStringUtil
{
public:
    static std::string& ltrim(std::string& str);
    static std::string& rtrim(std::string& str);
    static std::string& trim(std::string& str);
    static int split(const std::string &split, std::string &str, std::vector<std::string>& v);

};
#endif
