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

#include <stdint.h>
#include "../tests/testhelpers.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    STARTUP;

    GpRegion *region = NULL;
    GpStatus status;
    status = GdipCreateRegionRgnData (data, size, &region);
    if (region) {
        GdipDeleteRegion (region);
    }

    SHUTDOWN;
    return 0;
}
