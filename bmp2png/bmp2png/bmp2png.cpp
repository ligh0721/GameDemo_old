// bmp2png.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int GetEncoderClsid(const TCHAR* pFormat, CLSID* pClsid)
{
    UINT uNum = 0;
    UINT uSize = 0;
    ::GetImageEncodersSize(&uNum, &uSize);
    if (!uSize)
    {
        return -1;
    }

    ImageCodecInfo* pImgCodecInfo = (ImageCodecInfo*)malloc(uSize);
    if (::GetImageEncoders(uNum, uSize, pImgCodecInfo) != Ok)
    {
        return -1;
    }

    for (UINT i = 0; i < uNum; ++i)
    {
        if (!_tcscmp(pImgCodecInfo[i].MimeType, pFormat))
        {
            *pClsid = pImgCodecInfo[i].Clsid;
            free(pImgCodecInfo);
            return 0;
        }
    }
    free(pImgCodecInfo);
    return -1;
}

CLSID g_stPngClsid = {};

int bmp2png(const TCHAR* pSrc, const TCHAR* pDst)
{
    Image oImg(pSrc);

    Status eStat = oImg.Save(pDst, &g_stPngClsid);
    if (eStat != Ok)
    {
        fprintf(stderr, "save not ok, stat(%d)\n", eStat);
        return -1;
    }

    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc != 3)
    {
        _tprintf(_T("usage:\n%s SRC DEST\n"), argv[0]);
        return EXIT_FAILURE;
    }
    ULONG_PTR uGdipToken = 0;
    GdiplusStartupInput stGdiIn;
    GdiplusStartupOutput stGdiOut;
    ::GdiplusStartup(&uGdipToken, &stGdiIn, &stGdiOut);
    
    if (::GetEncoderClsid(_T("image/png"), &g_stPngClsid) < 0)
    {
        return EXIT_FAILURE;
    }

    if (::bmp2png(argv[1], argv[2]) < 0)
    {
        return EXIT_FAILURE;
    }

    ::GdiplusShutdown(uGdipToken);

	return EXIT_SUCCESS;
}
