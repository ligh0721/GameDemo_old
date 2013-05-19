//
//  Achieve.h
//  GameDemo
//
//  Created by qiuhelin on 13-5-11.
//
//

#ifndef GameDemo_Achieve_h
#define GameDemo_Achieve_h
#include "Displayer.h"
//�ɾ�����߼�����
enum ACHIEVE_LOGIC_TYPE
{
    LOGIC_TYPE_NO = 0, //�޴����ͳɾ�
    LOGIC_TYPE_OR,   //��ʱ�ɾ�
    LOGIC_TYPE_MIX,    //�ۻ��ɾ�
    LOGIC_TYPE_ORDER,  //�ۻ��ɾͣ����Ⱥ�˳��
    LOGIC_TYPE_SEQ,    //�ۻ��ɾͣ��ϸ�����з�ʽ
};
//�ɾ��¼�
class CAchieveEvent : public CDisplayObj
{
public:
    CAchieveEvent()
    :m_ulID(0)
    ,m_iEvent(0)
    ,m_bFinsh(false){}
    CAchieveEvent(unsigned long ulID, int iEvent)
    :m_ulID(ulID)
    ,m_iEvent(iEvent){}
    
    virtual void display(ostream& os, int level = 0) const;
    unsigned long m_ulID;
    int m_iEvent;
    bool m_bFinsh;
};

//�ɾ�bean
class CAchieve : public CDisplayObj
{
public:
    CAchieve();
    virtual ~CAchieve();
    bool init(const string title, const map<string, string>& entries);
    void fromEntry(const string title, const map<string, string>& entries);
    virtual void display(ostream& os, int level = 0) const;
public:
    string m_sType; //�ɾ�����
    string m_sName;//�ɾ�����
    string m_sDesc;//�ɾ�����
    string m_sPic;//ͼƬ����
    short m_wLogicType;//�ﵽ�ɾ͵��߼�����
    vector<CAchieveEvent> m_vecEvent; //�¼�����
    map<int, float> m_mapReward;//����
    bool m_bReach;
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
//�ɾʹ������
class CAchieveHandle
{
public:
    CAchieveHandle(CAchieveHandle* pNext=0, short wType=0);
    virtual bool handle(CAchieve*  pAchieve, const CAchieveEvent& event);
    virtual bool hasHandle(CAchieve* pAchieve);
    void setHandle(CAchieveHandle* pNext);
    void setType(short wType);
private:
    CAchieveHandle* m_pNextHandle;
    short m_wType;
};
class CNowAchieveHandle : public CAchieveHandle
{
public:
    CNowAchieveHandle(CAchieveHandle* pNext=0, short wType=LOGIC_TYPE_OR);
    virtual bool handle(CAchieve*  pAchieve, const CAchieveEvent& event);
};
class CMIXAchieveHandle : public CAchieveHandle
{
public:
    CMIXAchieveHandle(CAchieveHandle* pNext=0, short wType=LOGIC_TYPE_MIX);
    virtual bool handle(CAchieve*  pAchieve, const CAchieveEvent& event);
};
class COrderAchieveHandle : public CAchieveHandle
{
public:
    COrderAchieveHandle(CAchieveHandle* pNext=0, short wType=LOGIC_TYPE_OR);
    virtual bool handle(CAchieve*  pAchieve, const CAchieveEvent& event);
};
class CSeqAchieveHandle : public CAchieveHandle
{
public:
    CSeqAchieveHandle(CAchieveHandle* pNext=0, short wType=LOGIC_TYPE_SEQ);
    virtual bool handle(CAchieve*  pAchieve, const CAchieveEvent& event);
};
//�ɾ͹�����
class CAchieveManager : public CCObject
{
public:
    CAchieveManager():m_ulID(0){}
public:
    virtual ~CAchieveManager();
    
    bool init();
	CREATE_FUNC(CAchieveManager);
	static CAchieveManager* sharedAchieveManager();
    CAchieve* push(int iEvent);
public:
    vector<CAchieve*> m_vecAchieve;
private:
    unsigned long m_ulID;
    CAchieveHandle m_oContiner;
    CNowAchieveHandle m_oNow;
    CMIXAchieveHandle m_oMix;
    COrderAchieveHandle m_oOrder;
    CSeqAchieveHandle m_oSeq;
};

#endif
