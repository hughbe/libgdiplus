set(sources
  adjustablearrowcap.c
  bitmap.c
  bmpcodec.c
  brush.c
  customlinecap.c
  dstream.c
  emfcodec.c
  emfplus.c
  font.c
  gdi32.c
  general.c
  gifcodec.c
  graphics-cairo.c
  graphics-metafile.c
  graphics-path.c
  graphics-pathiterator.c
  graphics.c
  hatchbrush.c
  icocodec.c
  image.c
  imageattributes.c
  jpegcodec.c
  lineargradientbrush.c
  matrix.c
  metafile.c
  pathgradientbrush.c
  pen.c
  pngcodec.c
  print.c
  region-bitmap.c
  region-path-tree.c
  region.c
  solidbrush.c
  stringformat.c
  text-cairo.c
  text-metafile.c
  text-pango.c
  text.c
  texturebrush.c
  tiffcodec.c
  wmfcodec.c
)

if(WIN32)
  set(sources ${sources} win32_io.c)
endif()

if(APPLE)
  set(sources ${sources} carbon-private.c)
  set(libraries ${libraries} ${IOKit} ${CoreFoundation} ${CoreGraphics})
endif()

if(UNIX)
  set(libraries ${libraries} m)
endif()

add_library(libgdiplus ${sources})
target_link_libraries(libgdiplus ${libraries})
