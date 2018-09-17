set(tests
  testadjustablearrowcap
  testbitmap
  testbits
  testbmpcodec
  testbrush
  testcodecs
  testclip
  testcustomlinecap
  testemfcodec
  testfont
  testgeneral
  testgifcodec
  testgpimage
  testgraphics
  testgraphicspath
  testhatchbrush
  testicocodec
  testimageattributes
  testjpegcodec
  testlineargradientbrush
  testmatrix
  testmetafile
  testpathgradientbrush
  testpen
  testpng
  testpngcodec
  testregion
  testreversepath
  testsolidbrush
  teststringformat
  testtext
  testtexturebrush
  testtiffcodec
  testwmfcodec
)

include_directories("${CMAKE_SOURCE_DIR}/src")

# Add support for check and outputting on failure.
if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} 
        --force-new-ctest-process --output-on-failure 
        --build-config "$<CONFIGURATION>"
        USES_TERMINAL)
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} 
        --force-new-ctest-process --output-on-failure
        USES_TERMINAL)
endif()

foreach(test ${tests})

  add_executable(${test} "${test}.c")
  target_link_libraries(${test} libgdiplus)
  add_test(NAME ${test} COMMAND ${test} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests")
  add_dependencies(check ${test})

endforeach()
