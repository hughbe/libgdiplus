#ifdef WIN32
#ifndef __cplusplus
#error Please compile with a C++ compiler.
#endif
#endif

#if defined(USE_WINDOWS_GDIPLUS)
#include <Windows.h>
#include <GdiPlus.h>

#pragma comment(lib, "gdiplus.lib")
#else
#include <GdiPlusFlat.h>
#endif

#if defined(USE_WINDOWS_GDIPLUS)
using namespace Gdiplus;
using namespace DllExports;
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "testhelpers.h"

static const char *file = "temp_asset.wmf";
static WCHAR wFile[] = {'t', 'e', 'm', 'p', '_', 'a', 's', 's', 'e', 't', '.', 'w', 'm', 'f', 0};
GpImage *image;

#define createFile(buffer, expectedStatus) \
{ \
	save("wmfcodec", buffer, sizeof (buffer)); \
	GpStatus status; \
	FILE *f = fopen (file, "wb+"); \
	assert (f); \
	fwrite ((void *) buffer, sizeof (BYTE), sizeof (buffer), f); \
	fclose (f); \
 \
	status = GdipLoadImageFromFile (wFile, &image); \
	assertEqualInt (status, expectedStatus); \
}

#define createFileSuccess(buffer, x, y, width, height, dimensionWidth, dimensionHeight) \
{ \
	createFile (buffer, Ok); \
	verifyMetafile (image, wmfRawFormat, x, y, width, height, dimensionWidth, dimensionHeight) \
	GdipDisposeImage (image); \
}

static void test_valid ()
{
	BYTE singleEOFRecord[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE nonZeroHandle[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x01, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF4, 0x54,
		/* Metafile Header */  0x02, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE diskFileType[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x02, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE nonZeroNumberOfObjects[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE zeroMaxRecordSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE nonZeroNoParameters[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE longEOFRecord[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* Random */           0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x02
	};
	BYTE topGreaterThanBottom[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0xCE, 0xF2, 0x00, 0x01, 0x32, 0x0D, 0x00, 0x00, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x05, 0xAA,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE leftGreaterThanRight[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x01, 0xCE, 0xF2, 0x00, 0x00, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x05, 0xAA,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	BYTE zeroInches[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D, 0x57,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE nonZeroReserved[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE nonDIBVersionNumber[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x01, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	createFileSuccess (singleEOFRecord, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
	createFileSuccess (nonZeroHandle, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
	createFileSuccess (diskFileType, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
	createFileSuccess (nonZeroNumberOfObjects, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
	createFileSuccess (zeroMaxRecordSize, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
	createFileSuccess (nonZeroNoParameters, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
	createFileSuccess (longEOFRecord, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
	createFileSuccess (topGreaterThanBottom, -3378, 0, 6756, 256, 17160.2383f, 650.239929f);
	createFileSuccess (leftGreaterThanRight, 0, -3378, 256, 6756, 650.239929f, 17160.2383f);
	createFileSuccess (zeroInches, -4008, -3378, 8016, 6756, 14139.333008f, 11916.833008f);
	createFileSuccess (nonZeroReserved, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
	createFileSuccess (nonDIBVersionNumber, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
}

static void test_invalidDataCorruptingGdiPlus ()
{
	// GDI+ produces some really strange results - including negative sizes - if the checksum is invalid.
	// We probably don't want to emulate this behaviour.
#if defined(USE_WINDOWS_GDIPLUS)
	BYTE invalidChecksum[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE zeroWidth[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xF2, 0x00, 0x00, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x05, 0xAB,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE zeroHeight[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0x00, 0x00, 0xA8, 0x0F, 0x00, 0x00, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x09, 0xAB,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	createFile (invalidChecksum, Ok);
	verifyImage (image, ImageTypeMetafile, emfRawFormat, PixelFormat32bppRGB, 0, 0, 2, 2, -0.005236f, -0.004651f, -0.104166f, -0.092590f, 327683, 0, TRUE);
	GdipDisposeImage(image);

	createFile (zeroWidth, Ok);
	verifyImage (image, ImageTypeMetafile, emfRawFormat, PixelFormat32bppRGB, 0, 0, 2, 2, -0.005236f, -0.004651f, -0.104166f, -0.092590f, 327683, 0, TRUE);
	GdipDisposeImage(image);

	createFile (zeroHeight, Ok);
	verifyImage (image, ImageTypeMetafile, emfRawFormat, PixelFormat32bppRGB, 0, 0, 2, 2, -0.005236f, -0.004651f, -0.104166f, -0.092590f, 327683, 0, TRUE);
	GdipDisposeImage(image);
#endif
}

static void test_invalidFileSize()
{
	BYTE equalToHeaderFileSizeWithoutExtraData[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE equalToHeaderFileSizeWithExtraData[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE tooLargeFileSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	createFileSuccess (equalToHeaderFileSizeWithoutExtraData, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
	createFileSuccess (equalToHeaderFileSizeWithExtraData, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
	createFileSuccess (tooLargeFileSize, -4008, -3378, 8016, 6756, 20360.638672f, 17160.2383f);
}

static void test_invalidPlaceableHeader ()
{
	BYTE shortKey1[]     = {0xD7};
	BYTE shortKey2[]     = {0xD7, 0xCD};
	BYTE shortKey3[]     = {0xD7, 0xCD, 0xC6};
	BYTE noHandle[]      = {0xD7, 0xCD, 0xC6, 0x9A};
	BYTE shortHandle[]   = {0xD7, 0xCD, 0xC6, 0x9A, 0x00};
	BYTE noLeft[]        = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00};
	BYTE shortLeft[]     = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00};
	BYTE noTop[]         = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05};
	BYTE shortTop[]      = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00};
	BYTE noRight[]       = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04};
	BYTE shortRight[]    = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00};
	BYTE noBottom[]      = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06};
	BYTE shortBottom[]   = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00};
	BYTE noInches[]      = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02};
	BYTE shortInches[]   = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00};
	BYTE noReserved[]    = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02};
	BYTE shortReserved[] = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00};
	BYTE noChecksum[]    = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00};
	BYTE shortChecksum[] = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50};

	createFile (shortKey1, OutOfMemory);
	createFile (shortKey2, OutOfMemory);
	createFile (shortKey3, OutOfMemory);
	createFile (noHandle, OutOfMemory);
	createFile (shortHandle, OutOfMemory);
	createFile (noLeft, OutOfMemory);
	createFile (shortLeft, OutOfMemory);
	createFile (noTop, OutOfMemory);
	createFile (shortTop, OutOfMemory);
	createFile (noRight, OutOfMemory);
	createFile (shortRight, OutOfMemory);
	createFile (noBottom, OutOfMemory);
	createFile (shortBottom, OutOfMemory);
	createFile (noInches, OutOfMemory);
	createFile (shortInches, OutOfMemory);
	createFile (noReserved, OutOfMemory);
	createFile (shortReserved, OutOfMemory);
	createFile (noChecksum, OutOfMemory);
	createFile (shortChecksum, OutOfMemory);
}

static void test_invalidMetafileHeader ()
{
	BYTE noType[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11
	};
	BYTE shortType[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01
	};
	BYTE noHeaderSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00
	};
	BYTE shortHeaderSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00, 0x09
	};
	BYTE noVersion[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00
	};
	BYTE shortVersion[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00
	};
	BYTE noFileSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03
	};
	BYTE shortFileSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 004, 0x00, 0x00
	};
	BYTE noNumObjects[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x05, 0x00, 0x00, 0x00
	};
	BYTE shortNumObjects[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00
	};
	BYTE noMaxRecordSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE shortMaxRecordSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE noNoParameters[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE shortNoParameters[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x06, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x50, 0x11,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	createFile (noType, OutOfMemory);
	createFile (shortType, OutOfMemory);
	createFile (noHeaderSize, OutOfMemory);
	createFile (shortHeaderSize, OutOfMemory);
	createFile (noVersion, OutOfMemory);
	createFile (shortVersion, OutOfMemory);
	createFile (noFileSize, OutOfMemory);
	createFile (shortFileSize, OutOfMemory);
	createFile (noNumObjects, OutOfMemory);
	createFile (shortNumObjects, OutOfMemory);
	createFile (noMaxRecordSize, OutOfMemory);
	createFile (shortMaxRecordSize, OutOfMemory);
	createFile (noNoParameters, OutOfMemory);
	createFile (shortNoParameters, OutOfMemory);
}

static void test_invalidImageData()
{
	BYTE zeroFileType[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x00, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE invalidFileType[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x03, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE zeroHeaderSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE smallHeaderSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x08, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE largeHeaderSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x0A, 0x00, 0x00, 0x03, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE zeroVersionNumber[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE twoVersionNumber[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x02, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE largeVersionNumber[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x04, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE zeroFileSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE smallerThanHeaderFileSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE smallerThanEOFFileSize1[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE smallerThanEOFFileSize2[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* META_EOF */         0x03, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE noRecordsZeroMaxSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE noRecordsSmallMaxSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	BYTE noRecordsLargeMaxSize[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
	};
#if defined(USE_WINDOWS_GDIPLUS)
	BYTE noSuchFunction[] = {
		/* Placeable Header */ 0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00, 0x58, 0xF0, 0xCE, 0xF2, 0xA8, 0x0F, 0x32, 0x0d, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x54,
		/* Metafile Header */  0x01, 0x00, 0x09, 0x00, 0x00, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* Random */           0x03, 0x00, 0x00, 0x00, 0x00, 0x01
	};
#endif

	createFile (zeroFileType, OutOfMemory);
	createFile (invalidFileType, OutOfMemory);
	createFile (zeroHeaderSize, OutOfMemory);
	createFile (smallHeaderSize, OutOfMemory);
	createFile (largeHeaderSize, OutOfMemory);
	createFile (zeroVersionNumber, OutOfMemory);
	createFile (twoVersionNumber, OutOfMemory);
	createFile (largeVersionNumber, OutOfMemory);
	createFile (zeroFileSize, OutOfMemory);
	createFile (smallerThanHeaderFileSize, OutOfMemory);
	createFile (smallerThanEOFFileSize1, OutOfMemory);
	createFile (smallerThanEOFFileSize2, OutOfMemory);
	createFile (noRecordsZeroMaxSize, OutOfMemory);
	createFile (noRecordsSmallMaxSize, OutOfMemory);
	createFile (noRecordsLargeMaxSize, OutOfMemory);
	// FIXME: seems like GDI+ validates records more than libgdiplus.
#if defined(USE_WINDOWS_GDIPLUS)
	createFile (noSuchFunction, OutOfMemory);
#endif
}

int
main (int argc, char**argv)
{
	STARTUP;

	test_valid ();
	test_invalidDataCorruptingGdiPlus ();
	test_invalidFileSize ();
	test_invalidPlaceableHeader ();
	test_invalidMetafileHeader ();
	test_invalidImageData ();

	deleteFile (file);

	SHUTDOWN;
	return 0;
}
