#include "CommInc.h"

#include "CommDef.h"

#ifdef WIN32
#include "iconv/iconv.h"

const char* GBKToUTF8(const char *pGBKStr)
{
    static char szGBKConvUTF8Buf[5000];
    iconv_t hIconv;
    hIconv = iconv_open("utf-8","gb2312");
    if (!hIconv)
    {
        return NULL;
    }
    size_t uStrLen = strlen(pGBKStr);
    size_t uOutLen = uStrLen << 2;
    size_t uCopyLen = uOutLen;
    memset(szGBKConvUTF8Buf, 0, 5000);

    char* pOutBuf = (char*)malloc(uOutLen);
    char* pBuff = pOutBuf;
    memset( pOutBuf, 0, uOutLen);

    if (iconv(hIconv, &pGBKStr, &uStrLen, &pOutBuf, &uOutLen) == -1)
    {
        iconv_close(hIconv);
        return NULL;
    }
    memcpy(szGBKConvUTF8Buf,pBuff,uCopyLen);
    free(pBuff);
    iconv_close(hIconv);
    return szGBKConvUTF8Buf;
}

#endif

