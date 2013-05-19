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


#define MAX_BUFF_LENGTH 1024

void CAchieveEvent::display(std::ostream &os, int iLevel) const
{
    Displayer dis(os, iLevel);
    dis.display(m_ulID, "m_ulID");
    dis.display(m_iEvent, "m_iEvent");
    dis.display(m_bFinsh, "m_bFinsh");
}
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
                m_wLogicType = LOGIC_TYPE_OR;
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
                    CAchieveEvent a;
                    a.m_iEvent = atoi(item[0].c_str());
                    m_vecEvent.push_back(a);
                }
            }
        }
        it++;
    }
    
}
void CAchieve::display(ostream &os, int level) const
{
    Displayer dis(os,level);
    dis.display(m_sType, "m_sType");
    dis.display(m_sName, "m_sName");
    dis.display(m_sDesc, "m_sDesc");
    dis.display(m_sPic, "m_sPic");
    dis.display(m_wLogicType, "m_wLogicType");
    dis.display(m_vecEvent, "m_vecEvent");
    dis.display(m_mapReward, "m_mapReward");
    dis.display(m_bReach, "m_bReach");
}
CAchieveManager::~CAchieveManager()
{}

bool CAchieveManager::init()
{
    m_oContiner.setHandle(&m_oNow);
    m_oNow.setHandle(&m_oMix);
    m_oMix.setHandle(&m_oSeq);
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
CAchieve* CAchieveManager::push(int iEvent)
{
    CAchieveEvent event(++m_ulID, iEvent);
    for (size_t i = 0; i <  m_vecAchieve.size() && !m_vecAchieve[i]->m_bReach; i++)
    {
        if(m_oContiner.handle(m_vecAchieve[i], event))
        {
            if (m_vecAchieve[i]->m_bReach)
            {
                return m_vecAchieve[i];
            }
            return NULL;
        }
        
    }
    return NULL;
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
CAchieveHandle::CAchieveHandle(CAchieveHandle* pNext, short wType)
    :m_pNextHandle(pNext)
    ,m_wType(wType)
{
    
}
bool CAchieveHandle::hasHandle(CAchieve *pAchieve)
{
    return pAchieve?(pAchieve->m_wLogicType == m_wType):false;
}
void CAchieveHandle::setHandle(CAchieveHandle *pNext)
{
    m_pNextHandle = pNext;
}
void CAchieveHandle::setType(short wType)
{
    m_wType = wType;
}
bool CAchieveHandle::handle(CAchieve *pAchieve, const CAchieveEvent& event)
{
    if(m_pNextHandle != NULL)
    {
        return m_pNextHandle->handle(pAchieve, event);
    }
    return false;
}

CNowAchieveHandle::CNowAchieveHandle(CAchieveHandle* pNext, short wType)
{
    setHandle(pNext);
    setType(wType);
}

bool CNowAchieveHandle::handle(CAchieve *pAchieve, const CAchieveEvent &event)
{
    bool handle = false;
    if(hasHandle(pAchieve))
    {
        for (size_t i = 0; i < pAchieve->m_vecEvent.size(); i++)
        {
            if (pAchieve->m_vecEvent[i].m_iEvent == event.m_iEvent)
            {
                pAchieve->m_vecEvent[i].m_bFinsh = true;
                pAchieve->m_vecEvent[i].m_ulID = event.m_ulID;
                pAchieve->m_bReach = true;
                handle = true;
                break;
            }
        }
    }
    else
    {
        handle = CAchieveHandle::handle(pAchieve, event);
    }
    return handle;
}

CMIXAchieveHandle::CMIXAchieveHandle(CAchieveHandle* pNext, short wType)
{
    setHandle(pNext);
    setType(wType);
}

bool CMIXAchieveHandle::handle(CAchieve *pAchieve, const CAchieveEvent &event)
{
    bool handle = false;
    if(hasHandle(pAchieve))
    {
        int total = 0;
        bool find = false;
        for (size_t i = 0; i < pAchieve->m_vecEvent.size(); i++)
        {
            if (pAchieve->m_vecEvent[i].m_bFinsh)
            {
                total++;
            }
            else if (!find && pAchieve->m_vecEvent[i].m_iEvent == event.m_iEvent)
            {
                pAchieve->m_vecEvent[i].m_bFinsh = true;
                pAchieve->m_vecEvent[i].m_ulID = event.m_ulID;
                total++;
                find = true;
                handle = true;
            }
        }
        if (total == pAchieve->m_vecEvent.size())
        {
            pAchieve->m_bReach = true;
        }
    }
    else
    {
      handle =  CAchieveHandle::handle(pAchieve, event);
    }
    return handle;
}
COrderAchieveHandle::COrderAchieveHandle(CAchieveHandle* pNext, short wType)
{
    setHandle(pNext);
    setType(wType);
}
bool COrderAchieveHandle::handle(CAchieve *pAchieve, const CAchieveEvent &event)
{
    bool handle = false;
    if(hasHandle(pAchieve))
    {
        size_t i = 0;
        
        for (; i < pAchieve->m_vecEvent.size(); i++)
        {
            if (pAchieve->m_vecEvent[i].m_bFinsh)
            {
                continue;
            }
            else if (pAchieve->m_vecEvent[i].m_iEvent == event.m_iEvent
                     && (i == 0 || pAchieve->m_vecEvent[i].m_ulID > pAchieve->m_vecEvent[i-1].m_ulID))
            {
                pAchieve->m_vecEvent[i].m_ulID = event.m_ulID;
                pAchieve->m_vecEvent[i].m_bFinsh = true;
                handle = true;
            }
            break;
        }
        if (i == pAchieve->m_vecEvent.size() - 1)
        {
            pAchieve->m_bReach = true;
        }
    }
    else
    {
        handle  = CAchieveHandle::handle(pAchieve, event);
    }
    return handle;
}
CSeqAchieveHandle::CSeqAchieveHandle(CAchieveHandle* pNext, short wType)
{
    setHandle(pNext);
    setType(wType);
}
bool CSeqAchieveHandle::handle(CAchieve *pAchieve, const CAchieveEvent &event)
{
    bool handle = false;
    if(hasHandle(pAchieve))
    {
        size_t i = 0;
        for (; i < pAchieve->m_vecEvent.size(); i++)
        {
            if (pAchieve->m_vecEvent[i].m_bFinsh)
            {
                continue;
            }
            else if (pAchieve->m_vecEvent[i].m_iEvent == event.m_iEvent
                     && (i == 0 || pAchieve->m_vecEvent[i].m_ulID == pAchieve->m_vecEvent[i-1].m_ulID + 1))
            {
                pAchieve->m_vecEvent[i].m_ulID = event.m_ulID;
                pAchieve->m_vecEvent[i].m_bFinsh = true;
                handle = true;
            }
            break;
        }
        if (i == pAchieve->m_vecEvent.size() - 1)
        {
            pAchieve->m_bReach = true;
        }
    }
    else
    {
        handle = CAchieveHandle::handle(pAchieve, event);
    }
    return handle;
}