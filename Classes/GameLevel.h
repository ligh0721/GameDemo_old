#ifndef  __GAMELEVEL_H_
#define __GAMELEVEL_H_

class CGameFile;
class CGameMinorLeveInfo : public CCObject
{
public:	
	CGameMinorLeveInfo(int iMinorLevel, string& oTitle, string& oDesc, bool bPassed, int iStars);

public:
	int m_iMinorLevel; //�ؿ��εȼ� 
	string  m_sTitle;//�εȼ�����
	string m_sDesc; //�˵ȼ�����
	bool m_bPassed; //�Ƿ���˵ȼ�
	int m_iStars; //����

};
class CGameLevelInfo : public CCObject
{
public:
	CGameLevelInfo();
	
public: 
	int m_iMode;//ģʽ ����������
	int m_iMajorLevel; //�ؿ����ȼ�
	vector<CGameMinorLeveInfo*> m_vecMinorLevelInfo; //�ؿ��˵ȼ�������Ϣ
	string m_sTitle;//�ؿ�����
	string m_sTotalDesc;//�ؿ���������
	bool m_bPassed;//�Ƿ���˵ȼ�
};

class CGameSimpleLeveInfo : public CCObject
{
public:	
	CGameSimpleLeveInfo();
public:
	int m_iLevel; //�ؿ��ȼ� 
	string  m_sTitle;//�ȼ�����
	string m_sDesc; //�ȼ�����
};

class CGameLeveMarkInfo : public CCObject
{
public:
	static const int CONST_FILE_DATA_SIZE;

public:	
	CGameLeveMarkInfo();
	virtual bool initWithFileStream( CGameFile* pFile );
	virtual void writeToFileStream(FILE* pFile);
public:
	int m_iLevel; //�ؿ��ȼ� 
	byte m_cPassed; //�Ƿ���˵ȼ�
	byte m_cUnlock; //�Ƿ����
	int m_iStars; //����
};

class CGameLevelManager : public CCObject
{
public:
	static const string GAMELEVEL_FILE;
	static const string ARCHIVE_GAMELEVEL_FILE;

	typedef vector<CGameSimpleLeveInfo> VEC_GAMESIMPLELEVELINFO;
	typedef vector<CGameLeveMarkInfo> VEC_GMELEVELMARKINFO;
public:
	CGameLevelManager();

	virtual bool init();

	M_CREATE_FUNC_PARAM(CGameLevelManager, ());

	static CGameLevelManager* sharedGameLevelManager();

	unsigned int  getGameSimpleLeveInfoSize();

	unsigned int  getGameLeveMarkInfoSize();

	CGameSimpleLeveInfo* getGameSimpleLeveInfoByIndex(unsigned int iIndex);

	CGameLeveMarkInfo* getGameLevelMarkInfoByIndex(unsigned int iIndex);

	CGameSimpleLeveInfo* getGameSimpleLeveInfoByLevel(int iLevel);

	CGameLeveMarkInfo* getGameLevelMarkInfoByLevel(int iLevel);

	void setCurLevel(int iLevel){ m_iCurLevel = iLevel; }

	int getCurLevel(){ return m_iCurLevel; }

	void passCurLevel(int iStars);

	unsigned int  readGameLevelInfo(const char* pFileName);

	unsigned int  readGameLevelMarkInfo(const char* pFileName);

	void  saveGameLevelMarkInfo(const char* pFileName);


protected:
	VEC_GAMESIMPLELEVELINFO m_vecGameLevelInfo;
	VEC_GMELEVELMARKINFO m_vecGameLevelMarkInfo;
	int m_iCurLevel;
};
#endif //__GAMELEVEL_H_