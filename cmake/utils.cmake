# Get pillar version from include/pillar/version.h and put it in PILLAR_VERSION
function(pillar_extract_version)
  file(READ "${CMAKE_CURRENT_LIST_DIR}/include/pillar/version.h" file_contents)
  string(REGEX MATCH "YUZU_PILLAR_VER_MAJOR ([0-9]+)" _ "${file_contents}")
  if(NOT CMAKE_MATCH_COUNT EQUAL 1)
    message(
      FATAL_ERROR "Could not extract major version number from pillar/version.h"
    )
  endif()
  set(ver_major ${CMAKE_MATCH_1})

  string(REGEX MATCH "YUZU_PILLAR_VER_MINOR ([0-9]+)" _ "${file_contents}")
  if(NOT CMAKE_MATCH_COUNT EQUAL 1)
    message(
      FATAL_ERROR "Could not extract minor version number from pillar/version.h"
    )
  endif()

  set(ver_minor ${CMAKE_MATCH_1})
  string(REGEX MATCH "YUZU_PILLAR_VER_PATCH ([0-9]+)" _ "${file_contents}")
  if(NOT CMAKE_MATCH_COUNT EQUAL 1)
    message(
      FATAL_ERROR "Could not extract patch version number from pillar/version.h"
    )
  endif()
  set(ver_patch ${CMAKE_MATCH_1})

  set(PILLAR_VERSION_MAJOR
      ${ver_major}
      PARENT_SCOPE)
  set(PILLAR_VERSION_MINOR
      ${ver_minor}
      PARENT_SCOPE)
  set(PILLAR_VERSION_PATCH
      ${ver_patch}
      PARENT_SCOPE)
  set(PILLAR_VERSION
      "${ver_major}.${ver_minor}.${ver_patch}"
      PARENT_SCOPE)
endfunction()
