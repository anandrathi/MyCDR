# Install script for directory: E:/ani/soci-3.1.0/soci-3.1.0/core

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files/SOCI")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/soci" TYPE FILE FILES
    "E:/ani/soci-3.1.0/soci-3.1.0/core/backend-loader.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/blob-exchange.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/blob.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/boost-fusion.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/boost-gregorian-date.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/boost-optional.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/boost-tuple.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/connection-pool.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/error.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/exchange-traits.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/into-type.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/into.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/once-temp-type.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/prepare-temp-type.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/procedure.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/ref-counted-prepare-info.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/ref-counted-statement.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/row-exchange.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/row.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/rowid-exchange.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/rowid.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/rowset.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/session.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/soci-backend.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/soci-config.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/soci-platform.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/soci-simple.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/soci.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/statement.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/transaction.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/type-conversion-traits.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/type-conversion.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/type-holder.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/type-ptr.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/unsigned-types.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/use-type.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/use.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/values-exchange.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/values.h"
    "E:/ani/soci-3.1.0/soci-3.1.0/core/version.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/lib/Debug/soci_core_3_1.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/lib/Release/soci_core_3_1.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/lib/MinSizeRel/soci_core_3_1.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/lib/RelWithDebInfo/soci_core_3_1.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/bin/Debug/soci_core_3_1.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/bin/Release/soci_core_3_1.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/bin/MinSizeRel/soci_core_3_1.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/bin/RelWithDebInfo/soci_core_3_1.dll")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/lib/Debug/libsoci_core_3_1.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/lib/Release/libsoci_core_3_1.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/lib/MinSizeRel/libsoci_core_3_1.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/ani/soci-3.1.0/soci-3.1.0/build/lib/RelWithDebInfo/libsoci_core_3_1.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

