#pragma once


class CGameFile : public CCObject
{
public:
    enum FILE_ORIGIN
    {
        kBegin = SEEK_SET,
        kCur = SEEK_CUR,
        kEnd = SEEK_END
    };

public:
    CGameFile(void);
    virtual ~CGameFile(void);
    virtual bool init(const char* pFileName, const char* pMode);
    CREATE_FUNC_PARAM(CGameFile, (const char* pFileName, const char* pMode), pFileName, pMode);

    template <typename TYPE>
    size_t read(TYPE* pData, size_t uCount = 1);
    size_t tell() const;
    bool eof() const;
    bool seek(long lOffset, FILE_ORIGIN eOrigin);

protected:
    uint8_t* m_pData;
    unsigned long m_uSize;
    unsigned long m_uPos;
};

template <typename TYPE>
size_t CGameFile::read( TYPE* pData, size_t uCount /*= 1*/ )
{
    size_t uRead;
    TYPE* pCur = (TYPE*)((size_t)m_pData + m_uPos);
    for (uRead = 0; uRead < uCount && m_uPos + (uRead + 1) * sizeof(TYPE) <= m_uSize; ++uRead);
    size_t uReadSize = uRead * sizeof(TYPE);
    memmove(pData, pCur, uReadSize);
    m_uPos += uReadSize;
    return uRead;
}

