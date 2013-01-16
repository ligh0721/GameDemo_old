#ifndef  __GAMELEVEL_H_
#define __GAMELEVEL_H_

class CGameFile;
class CGameMinorLeveInfo : public CCObject
{
public:	
	CGameMinorLeveInfo(int iMinorLevel, string& oTitle, string& oDesc, bool bPassed, int iStars);

public:
	int m_iMinorLevel; //关卡次等级 
	string  m_sTitle;//次等级标题
	string m_sDesc; //此等级描述
	bool m_bPassed; //是否过此等级
	int m_iStars; //评星

};
class CGameLevelInfo : public CCObject
{
public:
	CGameLevelInfo();
	
public: 
	int m_iMode;//模式 进攻，防守
	int m_iMajorLevel; //关卡主等级
	vector<CGameMinorLeveInfo*> m_vecMinorLevelInfo; //关卡此等级描述信息
	string m_sTitle;//关卡标题
	string m_sTotalDesc;//关卡总体描述
	bool m_bPassed;//是否过此等级
};

class CGameSimpleLeveInfo : public CCObject
{
public:	
	CGameSimpleLeveInfo();
public:
	int m_iLevel; //关卡等级 
	string  m_sTitle;//等级标题
	string m_sDesc; //等级描述
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
	int m_iLevel; //关卡等级 
	byte m_cPassed; //是否过此等级
	byte m_cUnlock; //是否解锁
	int m_iStars; //评星
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