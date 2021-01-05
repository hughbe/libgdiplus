#ifdef WIN32
#ifndef __cplusplus
#error Please compile with a C++ compiler.
#endif
#endif

#if defined(USE_WINDOWS_GDIPLUS)
#include <Windows.h>
#include <GdiPlus.h>

#pragma comment(lib, "gdiplus")
#else
#include <GdiPlusFlat.h>
#endif

#if defined(USE_WINDOWS_GDIPLUS)
using namespace Gdiplus;
using namespace DllExports;
#endif

#include <assert.h>
#include <fstream>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "../tests/testhelpers.h"

static void createFile (const char* fileName, const BYTE* buffer, size_t bufferLength)
{
	FILE *f = fopen (fileName, "wb+");
	assert (f);
	fwrite ((void *) buffer, sizeof (BYTE), bufferLength, f);
	fclose (f);
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    STARTUP;

    const char* fileName = "test_image";
    WCHAR *wFileName = createWchar(fileName);
    createFile (fileName, data, size);

    GpImage *image;
    GpStatus status;
    status = GdipLoadImageFromFile (wFileName, &image);

    freeWchar(wFileName);
    SHUTDOWN;
    return 0;
}
