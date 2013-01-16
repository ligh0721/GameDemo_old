#include "CommInc.h"
#include "GameFile.h"


CGameFile::CGameFile(void)
{
}


CGameFile::~CGameFile(void)
{
}

bool CGameFile::init( const char* pFileName, const char* pMode )
{
    M_DEF_FU(pFu);
    m_uPos = m_uSize = 0;
    m_pData = pFu->getFileData(pFu->fullPathFromRelativePath(pFileName), pMode, &m_uSize);
    if (!m_pData)
    {
        return false;
    }
    return true;
}

size_t CGameFile::tell() const
{
    return m_uPos;
}

bool CGameFile::eof() const
{
    return m_uPos >= m_uSize;
}

bool CGameFile::seek( long lOffset, FILE_ORIGIN eOrigin )
{
    unsigned long uPos = 0;
    switch (eOrigin)
    {
    case kBegin:
        uPos = lOffset;
        break;

    case kCur:
        uPos = m_uPos + lOffset;
        break;

    case kEnd:
        uPos = m_uSize + lOffset;
        break;

    default:
        return false;
    }

    if (uPos < 0 || uPos > m_uSize)
    {
        return false;
    }

    m_uPos = uPos;
    return true;
}
