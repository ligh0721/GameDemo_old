//
//  Achieve.h
//  GameDemo
//
//  Created by qiuhelin on 13-5-11.
//
//

#ifndef GameDemo_Achieve_h
#define GameDemo_Achieve_h
//成就bean
class CAchieve
{
public:
    CAchieve();
    virtual ~CAchieve();
    bool init(const string title, const map<string, string>& entries);
    void fromEntry(const string title, const map<string, string>& entries);
    void display(ostream& os, int level = 0);
public:
    string m_sType; //成就类型
    string m_sName;//成就名称
    string m_sDesc;//成就描述
    string m_sPic;//图片名称
    short m_wLogicType;//达到成就的逻辑类型
    vector<int> m_vecEvent; //事件数组
    vector<bool> m_vecFinish; //事件完成标志
    map<int, float> m_mapReward;//奖励
    bool m_bReach;
};
//成就管理器
class CAchieveManager : public CCObject
{
public:
    CAchieveManager(){}
public:
    virtual ~CAchieveManager();
    
    bool init();
	CREATE_FUNC(CAchieveManager);
	static CAchieveManager* sharedAchieveManager();
public:
    vector<CAchieve*> m_vecAchieve;
};
typedef map<string, map<string, string>* > EntryContent;
//ini文件解析
class CEntryFile
{
public:
    CEntryFile();
    ~CEntryFile();
public:
    bool init(const char* pFileName);
    
public:
    string getFileName(){ return m_sFileName; }
    
protected:
    string m_sFileName;
public:
    EntryContent m_entryContent;
};
#endif
