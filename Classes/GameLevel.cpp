#include "CommInc.h"

#include "GameLevel.h"
#include "GameFile.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>

const char* g_pGameLevelFileName = "level.plist";

CGameMinorLeveInfo::CGameMinorLeveInfo( int iMinorLevel, string& oTitle, string& oDesc, bool bPassed, int iStars )
	: m_iMinorLevel(iMinorLevel)
	, m_sTitle(oTitle)
	, m_sDesc(oDesc)
	, m_bPassed(bPassed)
	, m_iStars(iStars)

{

}


CGameLevelInfo::CGameLevelInfo()
	: m_iMajorLevel(0)
	, m_iMode(0)
	, m_sTitle("")
	, m_sTotalDesc("")
	, m_bPassed(false)

{

}

CGameLevelManager::CGameLevelManager()
{

}

CGameLevelManager* CGameLevelManager::sharedGameLevelManager()
{
	static CGameLevelManager* pGlm = NULL;
	if (pGlm)
	{
		return pGlm;
	}
	pGlm = CGameLevelManager::create();
	pGlm->retain();
	return pGlm;
}

bool CGameLevelManager::init()
{
	//const char* xmlName = "level.plist"; //нд╪Ч
	unsigned int iGameLevelSize = readGameLevelInfo(GAMELEVEL_FILE.c_str());
	unsigned int iGameLevelMarkSize = readGameLevelMarkInfo(ARCHIVE_GAMELEVEL_FILE.c_str());
	if (iGameLevelMarkSize < iGameLevelSize)
	{
		for (VEC_GAMESIMPLELEVELINFO::iterator it = m_vecGameLevelInfo.begin(); it != m_vecGameLevelInfo.end(); ++it)
		{
			VEC_GMELEVELMARKINFO::iterator oMarkIt = m_vecGameLevelMarkInfo.begin();
			for ( ;oMarkIt != m_vecGameLevelMarkInfo.end(); ++oMarkIt)
			{
				if (it->m_iLevel == oMarkIt->m_iLevel)
				{
					break;
				}
				
			}
			if (oMarkIt == m_vecGameLevelMarkInfo.end())
			{
				CGameLeveMarkInfo oMarkInfo;
				oMarkInfo.m_iLevel = it->m_iLevel;
				if (it->m_iLevel == 1)
				{
					oMarkInfo.m_cUnlock = 1;
				}
				m_vecGameLevelMarkInfo.push_back(oMarkInfo);
			}	
		}
		saveGameLevelMarkInfo(ARCHIVE_GAMELEVEL_FILE.c_str());
	}
	return true;
}

unsigned int  CGameLevelManager::readGameLevelInfo(const char* pFileName)
{
	CCFileUtils* pFileUtiles = CCFileUtils::sharedFileUtils();
	const char *pPath = pFileUtiles->fullPathFromRelativePath(pFileName);
	CCDictionary* pDic = CCDictionary::createWithContentsOfFile(pPath);

	CCArray* gameLevel = (CCArray*)pDic->objectForKey("GameLevel");
	CCLog("gameLevel:count=%d", gameLevel->count());

	for(unsigned int i = 0; i < gameLevel->count(); i++)
	{
		CCDictionary* pDic = (CCDictionary*)gameLevel->objectAtIndex(i);
		CGameSimpleLeveInfo oInfo ;
		int iLevel = ((CCString*)pDic->objectForKey("Level"))->intValue();
		oInfo.m_iLevel = iLevel;
		string sTitle = ((CCString*)pDic->objectForKey("Title"))->m_sString;
		oInfo.m_sTitle = sTitle;
		string sDesc = ((CCString*)pDic->objectForKey("Desc"))->m_sString;
		oInfo.m_sDesc = sDesc;
		CCLog("%d|%s|%s", iLevel, sTitle.c_str(), sDesc.c_str());
		m_vecGameLevelInfo.push_back(oInfo);
	}
	return m_vecGameLevelInfo.size();
}

void CGameLevelManager::passCurLevel(int iStars)
{
	VEC_GMELEVELMARKINFO::iterator it = m_vecGameLevelMarkInfo.begin();
	CGameLeveMarkInfo* pMarkInfo = getGameLevelMarkInfoByLevel(m_iCurLevel);
	CCAssert(pMarkInfo, "pMarkInfo must not be null!!");
	pMarkInfo->m_iStars = iStars;
	pMarkInfo->m_cPassed = true;

	pMarkInfo = getGameLevelMarkInfoByLevel(m_iCurLevel + 1);
	CCAssert(pMarkInfo, "pMarkInfo must not be null!!");
	pMarkInfo->m_cUnlock = 1;

	saveGameLevelMarkInfo(ARCHIVE_GAMELEVEL_FILE.c_str());
}

unsigned int CGameLevelManager::getGameSimpleLeveInfoSize()
{
	return m_vecGameLevelInfo.size();
}

unsigned int CGameLevelManager::getGameLeveMarkInfoSize()
{
	return m_vecGameLevelMarkInfo.size();
}

CGameSimpleLeveInfo* CGameLevelManager::getGameSimpleLeveInfoByIndex( unsigned int iIndex )
{
	if (iIndex + 1 < m_vecGameLevelInfo.size())
		return &m_vecGameLevelInfo[iIndex];
	return NULL;
}

CGameLeveMarkInfo* CGameLevelManager::getGameLevelMarkInfoByIndex( unsigned int iIndex )
{
	if (iIndex + 1 < m_vecGameLevelMarkInfo.size())
		return &m_vecGameLevelMarkInfo[iIndex];
	return NULL;
}

CGameSimpleLeveInfo* CGameLevelManager::getGameSimpleLeveInfoByLevel( int iLevel )
{
	for (unsigned int i = 0; i < m_vecGameLevelInfo.size(); i++)
	{
		if (m_vecGameLevelInfo[i].m_iLevel == iLevel)
		{
			return &m_vecGameLevelInfo[i];
		}
	}
	return NULL;
}

CGameLeveMarkInfo* CGameLevelManager::getGameLevelMarkInfoByLevel( int iLevel )
{
	for (unsigned int i = 0; i < m_vecGameLevelMarkInfo.size(); i++)
	{
		if (m_vecGameLevelMarkInfo[i].m_iLevel == iLevel)
		{
			return &m_vecGameLevelMarkInfo[i];
		}
	}
	return NULL;
}
unsigned int  CGameLevelManager::readGameLevelMarkInfo( const char* pFileName )
{
	if (strcmp(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(pFileName), pFileName) == 0)
	{
		return 0;
	}

	CGameFile* pFile = CGameFile::create(pFileName, "rb");
	if (pFile == NULL)
	{

	}
	uint32_t dwHdr = 0;
	int iRet = 0;
	do 
	{
		pFile->read(&dwHdr);
		if (dwHdr != 'GLMI')
		{
			break;
		}
		CGameLeveMarkInfo oInfo;
		while (oInfo.initWithFileStream(pFile))
		{
			++iRet;
			m_vecGameLevelMarkInfo.push_back(oInfo);
		}
	} while (false);

	return iRet;

}

void CGameLevelManager::saveGameLevelMarkInfo( const char* pFileName )
{
	string sFullName = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(pFileName);
	FILE* pFile = fopen(sFullName.c_str(), "wb");
	uint32_t uTag =  'GLMI';
	fwrite(&uTag, 1, 4, pFile);
	for (VEC_GMELEVELMARKINFO::iterator it = m_vecGameLevelMarkInfo.begin(); it != m_vecGameLevelMarkInfo.end(); ++it)
	{
		it->writeToFileStream(pFile);
	}
	fclose(pFile);
}

const string CGameLevelManager::ARCHIVE_GAMELEVEL_FILE="gamelevel.ar";

const string CGameLevelManager::GAMELEVEL_FILE = "level.plist";

CGameSimpleLeveInfo::CGameSimpleLeveInfo()
	: m_iLevel(0)
{
}


CGameLeveMarkInfo::CGameLeveMarkInfo()
	: m_iLevel(0)
	, m_cPassed(0)
	, m_iStars(0)
	, m_cUnlock(0)
{

}

const int CGameLeveMarkInfo::CONST_FILE_DATA_SIZE 
	= sizeof(m_iLevel)
	+ sizeof(m_iStars)
	+ sizeof(m_cPassed)
	+ sizeof(m_cUnlock);

bool CGameLeveMarkInfo::initWithFileStream( CGameFile* pFile )
{
	uint32_t wDataSize = 0;
	if (pFile->read(&wDataSize) != 1)
	{
		return false;
	}
	size_t uPos = pFile->tell();
	bool bRet = false;
	do
	{
		if (wDataSize != CONST_FILE_DATA_SIZE
			|| pFile->read(&m_iLevel) != 1
			|| pFile->read(&m_iStars) != 1
			|| pFile->read(&m_cPassed) != 1
			|| pFile->read(&m_cUnlock) != 1
			)
		{
			break;
		}
		bRet = true;

	} while (false);

	if (!bRet)
	{
		pFile->seek(uPos, CGameFile::kBegin);
	}
	return bRet;
}

void CGameLeveMarkInfo::writeToFileStream( FILE* pFile )
{
	fwrite(&CONST_FILE_DATA_SIZE, sizeof(CONST_FILE_DATA_SIZE), 1, pFile);
	fwrite(&m_iLevel, sizeof(m_iLevel), 1, pFile);
	fwrite(&m_iStars, sizeof(m_iStars), 1, pFile);
	fwrite(&m_cPassed, sizeof(m_cPassed), 1, pFile);
	fwrite(&m_cUnlock, sizeof(m_cUnlock), 1, pFile);
}


/*
void CGameLevelManager::saveGameLevelInfo( CGameLevelInfo* pInfo )
{
	CGameMinorLeveInfo* pGameMinorLevelInfo = NULL;
	for (unsigned int i=0; i < pInfo->m_vecMinorLevelInfo.size(); i++)
	{
		if (pInfo->m_vecMinorLevelInfo[i]->m_iMinorLevel == m_iCurlMinorLevel)
		{
			pGameMinorLevelInfo = pInfo->m_vecMinorLevelInfo[i];
			break;
		}
	}
	CCAssert(pGameMinorLevelInfo, "CGameMinorLeveInfo should not be null!");
	ostringstream os;
	os << m_iCurMode;
	string mode = os.str();
	CCLog("%s", mode.c_str());
	os.clear();
	os << m_iCurMajorLevel;
	string majorLevel = os.str();
	CCLog("%s", majorLevel.c_str());
	os.clear();
	os << m_iCurlMinorLevel;
	string minorLevel = os.str();
	CCLog("%s", majorLevel.c_str());
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(g_pGameLevelFileName));

	CCAssert(doc, "Document not parsed successfully.");

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) 
	{
		CCLog("empty document\n");
		xmlFreeDoc(doc);
		return;
	}

	cur = cur->xmlChildrenNode;
	while (cur != NULL) 
	{
		if (xmlStrcmp(cur->name, (const xmlChar *)"key") 
			&& xmlStrcmp(cur->content, (const xmlChar *)"Mode")
			&& (cur = cur->next)
			&& xmlStrcmp(cur->content, (const xmlChar *)mode.c_str())
			&& (cur = cur->next)
			&& xmlStrcmp(cur->content, (const xmlChar *)"MajorLevel")
			&& (cur = cur->next)
			&& xmlStrcmp(cur->content, (const xmlChar *)majorLevel.c_str()))
		{
			while (cur != NULL)
			{
				if (xmlStrcmp(cur->name, (const xmlChar *)"key") 
					&& xmlStrcmp(cur->content, (const xmlChar *)"Passed")
					&& (cur = cur->next))
				{
					cur->name = (const xmlChar *)(pInfo->m_bPassed ? "true" : "false");

					while (cur != NULL)
					{
						if (xmlStrcmp(cur->content, (const xmlChar *)"MinorLevel")
							&& (cur = cur->next)
							&& xmlStrcmp(cur->content, (const xmlChar *)majorLevel.c_str()))
						{
							while (cur != NULL)
							{
								if (xmlStrcmp(cur->name, (const xmlChar *)"key")
									&& xmlStrcmp(cur->content, (const xmlChar *)"Passed")
									&& (cur = cur->next))
							{
									cur->name = (const xmlChar *)(pGameMinorLevelInfo->m_bPassed ? "true" : "false");
									os.clear();
									os <<  pGameMinorLevelInfo->m_iStars;
									cur->next->next->content = (xmlChar *)os.str().c_str();
									break;
								}
							}
							break;
						}
						cur = cur->next;
					}
					break;
				}
				cur = cur->next;
			}
		}

		cur = cur->next;
	}
	int iRel = xmlSaveFile(g_pGameLevelFileName,doc);
	CCLog("save gamelevel ret=%d", iRel);
	xmlFreeDoc(doc);
}
*/