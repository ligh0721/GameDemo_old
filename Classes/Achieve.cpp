//
//  Achieve.cpp
//  GameDemo
//
//  Created by qiuhelin on 13-5-11.
//
//
#include "CommInc.h"

#include "Achieve.h"
#include "StringUtil.h"
#include <iostream>
#include <fstream>
#include <strstream>
#include <cctype>
#include <algorithm>
#include "Displayer.h"

#define MAX_BUFF_LENGTH 1024
//成就完成逻辑类型
enum ACHIEVE_LOGIC_TYPE
{
    LOGIC_TYPE_OR = 1, //即时成就
    LOGIC_TYPE_MIX,    //累积成就
    LOGIC_TYPE_ORDER,  //累积成就，有先后顺序
    LOGIC_TYPE_SEQ,    //累积成就，严格的序列方式
};
CAchieve::CAchieve()
{
    m_bReach = false;
}

bool CAchieve::init(const string title, const map<string, string> &entries)
{
    fromEntry(title, entries);
    return true;
}
CAchieve::~CAchieve()
{
    
}
void CAchieve::fromEntry(const string title, const map<string, string>& entries)
{
    m_sName = title;
    map<string, string>::const_iterator it = entries.begin();
    m_bReach = false;
    while (it != entries.end())
    {
        if (it->first == "type")
        {
            m_sType = it->second;
        }
        else if(it->first == "desc")
        {
            m_sDesc = it->second;
        }
        else if(it->first == "pic")
        {
            m_sPic = it->second;
        }
        else if(it->first == "reward")
        {
            string reward = it->second;
            vector<string> vecReward;
            CStringUtil::split(",", reward, vecReward);
            for (size_t i=0; i < vecReward.size(); i++)
            {
                vector<string> item;
                CStringUtil::split(":", vecReward[i], item);
                if(item.size() < 2)
                    continue;
                m_mapReward.insert(make_pair(atoi(item[0].c_str()), atoi(item[1].c_str())));
            }
        }
        else if(it->first == "case")
        {
            string::size_type left = it->second.find('(');
            string::size_type right = it->second.find(')');
            string events = "";
            if (left != string::npos && right != string::npos)
            {
                events =  it->second.substr(left + 1, right - left);
                string logicType =  it->second.substr(0, left);
                transform(logicType.begin(), logicType.end(), logicType.begin(), ::tolower);
               if( logicType == "or")
               {
                   m_wLogicType = LOGIC_TYPE_OR;
                   
               }
                else if(logicType == "mix")
                {
                    m_wLogicType = LOGIC_TYPE_MIX;
                    
                }
                else if(logicType == "order")
                {
                    m_wLogicType = LOGIC_TYPE_ORDER;
                }
                else if(logicType == "seq")
                {
                    m_wLogicType = LOGIC_TYPE_SEQ;
                }
            }
            else if(left != string::npos && right != string::npos)
            {
                continue;
            }
            else
            {
                m_sType = LOGIC_TYPE_OR;
                events = it->second;
            }
           
            vector<string> vecEvent;
            CStringUtil::split(",", events, vecEvent);
            for (size_t i = 0; i < vecEvent.size(); i++)
            {
                vector<string> item;
                CStringUtil::split(":", vecEvent[i], item);
                if(item.size() < 2)
                    continue;
                size_t num = atoi(item[1].c_str());
                for (size_t j = 0; j < num;  j++)
                {
                    m_vecEvent.push_back(atoi(item[0].c_str()));
                    m_vecFinish.push_back(false);
                }
            }
        }
        it++;
    }
    
}
void CAchieve::display(ostream &os, int level)
{
    Displayer dis(os,level);
    dis.display(m_sType, "m_sType");
    dis.display(m_sName, "m_sName");
    dis.display(m_sDesc, "m_sDesc");
    dis.display(m_sPic, "m_sPic");
    dis.display(m_wLogicType, "m_wLogicType");
    dis.display(m_vecEvent, "m_vecEvent");
    dis.display(m_vecFinish, "m_vecFinish");
    dis.display(m_mapReward, "m_mapReward");
    dis.display(m_bReach, "m_bReach");
}
CAchieveManager::~CAchieveManager()
{}

bool CAchieveManager::init()
{
    CEntryFile file;
    if(! file.init("achieve.ini"))
    {
        return false;
    }
    EntryContent::iterator it = file.m_entryContent.begin();
    while (it != file.m_entryContent.end())
    {
        CAchieve* item = new CAchieve;
        item->fromEntry(it->first, *it->second);
        ostringstream  out;
        item->display(out);
        CCLog("achieve:%s\n", out.str().c_str());
        m_vecAchieve.push_back(item);
        it++;
    }
    return true;
}

CAchieveManager* CAchieveManager::sharedAchieveManager()
{
    static CAchieveManager* pInst = NULL;
    if (pInst)
    {
        return pInst;
    }
    pInst = CAchieveManager::create();
    pInst->retain();
    return pInst;
}

CEntryFile::CEntryFile()
{
}
CEntryFile::~CEntryFile()
{
    
    for (EntryContent::iterator it = m_entryContent.begin(); it != m_entryContent.end(); it++)
    {
        delete it->second;
    }
}
bool CEntryFile::init(const char *pFileName)
{
    m_sFileName = pFileName;
    M_DEF_FU(pFu);
    const char* pFilePath = pFu->fullPathFromRelativePath(pFileName);
    fstream rfile(pFilePath);
    if (!rfile)
    {
        std::cout << "read file :" << pFilePath << " failed " << std::endl;
        return false;
    }
    string line;
    map<string, string>* pItem =  NULL;
    while (getline(rfile, line))
    {
        if( line.find('[') == 0 && line.find(']') == (line.size() - 1))
        {
            pItem = new map<string, string>();
            string key = line.substr(1,line.size()-2);
            m_entryContent.insert(make_pair(key, pItem));
            CCLog("section:%s\n", key.c_str());
        }
        else
        {
            if (!pItem)
            {
                continue;
            }
            int index = string::npos;
            if ((index = line.find('=')) == string::npos)
                continue;
            string key = line.substr(0, index-1);
            string value = line.substr(index+1, line.size() - 1 - index);
            CCLog("key:%s,value:%s\n", key.c_str(), value.c_str());
            pItem->insert(make_pair(CStringUtil::trim(key), CStringUtil::trim(value)));
            
        }
    }
    rfile.close();
    return true;
}


