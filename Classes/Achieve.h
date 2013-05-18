//
//  Achieve.h
//  GameDemo
//
//  Created by qiuhelin on 13-5-11.
//
//

#ifndef GameDemo_Achieve_h
#define GameDemo_Achieve_h
//�ɾ�bean
class CAchieve
{
public:
    CAchieve();
    virtual ~CAchieve();
    bool init(const string title, const map<string, string>& entries);
    void fromEntry(const string title, const map<string, string>& entries);
    void display(ostream& os, int level = 0);
public:
    string m_sType; //�ɾ�����
    string m_sName;//�ɾ�����
    string m_sDesc;//�ɾ�����
    string m_sPic;//ͼƬ����
    short m_wLogicType;//�ﵽ�ɾ͵��߼�����
    vector<int> m_vecEvent; //�¼�����
    vector<bool> m_vecFinish; //�¼���ɱ�־
    map<int, float> m_mapReward;//����
    bool m_bReach;
};
//�ɾ͹�����
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
//ini�ļ�����
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
