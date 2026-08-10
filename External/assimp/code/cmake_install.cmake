# Install script for directory: D:/Engine/External/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Engine/External/assimp/Product")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp6.0.5-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Engine/External/assimp/lib/Debug/assimp-vc145-mtd.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Engine/External/assimp/lib/Release/assimp-vc145-mt.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Engine/External/assimp/lib/MinSizeRel/assimp-vc145-mt.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Engine/External/assimp/lib/RelWithDebInfo/assimp-vc145-mt.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp6.0.5" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Engine/External/assimp/bin/Debug/assimp-vc145-mtd.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Engine/External/assimp/bin/Release/assimp-vc145-mt.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Engine/External/assimp/bin/MinSizeRel/assimp-vc145-mt.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Engine/External/assimp/bin/RelWithDebInfo/assimp-vc145-mt.dll")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/Engine/External/assimp/code/../include/assimp/anim.h"
    "D:/Engine/External/assimp/code/../include/assimp/aabb.h"
    "D:/Engine/External/assimp/code/../include/assimp/ai_assert.h"
    "D:/Engine/External/assimp/code/../include/assimp/camera.h"
    "D:/Engine/External/assimp/code/../include/assimp/color4.h"
    "D:/Engine/External/assimp/code/../include/assimp/color4.inl"
    "D:/Engine/External/assimp/code/../include/assimp/config.h"
    "D:/Engine/External/assimp/code/../include/assimp/ColladaMetaData.h"
    "D:/Engine/External/assimp/code/../include/assimp/commonMetaData.h"
    "D:/Engine/External/assimp/code/../include/assimp/defs.h"
    "D:/Engine/External/assimp/code/../include/assimp/cfileio.h"
    "D:/Engine/External/assimp/code/../include/assimp/light.h"
    "D:/Engine/External/assimp/code/../include/assimp/material.h"
    "D:/Engine/External/assimp/code/../include/assimp/material.inl"
    "D:/Engine/External/assimp/code/../include/assimp/matrix3x3.h"
    "D:/Engine/External/assimp/code/../include/assimp/matrix3x3.inl"
    "D:/Engine/External/assimp/code/../include/assimp/matrix4x4.h"
    "D:/Engine/External/assimp/code/../include/assimp/matrix4x4.inl"
    "D:/Engine/External/assimp/code/../include/assimp/mesh.h"
    "D:/Engine/External/assimp/code/../include/assimp/ObjMaterial.h"
    "D:/Engine/External/assimp/code/../include/assimp/pbrmaterial.h"
    "D:/Engine/External/assimp/code/../include/assimp/GltfMaterial.h"
    "D:/Engine/External/assimp/code/../include/assimp/postprocess.h"
    "D:/Engine/External/assimp/code/../include/assimp/quaternion.h"
    "D:/Engine/External/assimp/code/../include/assimp/quaternion.inl"
    "D:/Engine/External/assimp/code/../include/assimp/scene.h"
    "D:/Engine/External/assimp/code/../include/assimp/metadata.h"
    "D:/Engine/External/assimp/code/../include/assimp/texture.h"
    "D:/Engine/External/assimp/code/../include/assimp/types.h"
    "D:/Engine/External/assimp/code/../include/assimp/vector2.h"
    "D:/Engine/External/assimp/code/../include/assimp/vector2.inl"
    "D:/Engine/External/assimp/code/../include/assimp/vector3.h"
    "D:/Engine/External/assimp/code/../include/assimp/vector3.inl"
    "D:/Engine/External/assimp/code/../include/assimp/version.h"
    "D:/Engine/External/assimp/code/../include/assimp/cimport.h"
    "D:/Engine/External/assimp/code/../include/assimp/AssertHandler.h"
    "D:/Engine/External/assimp/code/../include/assimp/importerdesc.h"
    "D:/Engine/External/assimp/code/../include/assimp/Importer.hpp"
    "D:/Engine/External/assimp/code/../include/assimp/DefaultLogger.hpp"
    "D:/Engine/External/assimp/code/../include/assimp/ProgressHandler.hpp"
    "D:/Engine/External/assimp/code/../include/assimp/IOStream.hpp"
    "D:/Engine/External/assimp/code/../include/assimp/IOSystem.hpp"
    "D:/Engine/External/assimp/code/../include/assimp/Logger.hpp"
    "D:/Engine/External/assimp/code/../include/assimp/LogStream.hpp"
    "D:/Engine/External/assimp/code/../include/assimp/NullLogger.hpp"
    "D:/Engine/External/assimp/code/../include/assimp/cexport.h"
    "D:/Engine/External/assimp/code/../include/assimp/Exporter.hpp"
    "D:/Engine/External/assimp/code/../include/assimp/DefaultIOStream.h"
    "D:/Engine/External/assimp/code/../include/assimp/DefaultIOSystem.h"
    "D:/Engine/External/assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "D:/Engine/External/assimp/code/../include/assimp/SceneCombiner.h"
    "D:/Engine/External/assimp/code/../include/assimp/fast_atof.h"
    "D:/Engine/External/assimp/code/../include/assimp/qnan.h"
    "D:/Engine/External/assimp/code/../include/assimp/BaseImporter.h"
    "D:/Engine/External/assimp/code/../include/assimp/Hash.h"
    "D:/Engine/External/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "D:/Engine/External/assimp/code/../include/assimp/ParsingUtils.h"
    "D:/Engine/External/assimp/code/../include/assimp/StreamReader.h"
    "D:/Engine/External/assimp/code/../include/assimp/StreamWriter.h"
    "D:/Engine/External/assimp/code/../include/assimp/StringComparison.h"
    "D:/Engine/External/assimp/code/../include/assimp/StringUtils.h"
    "D:/Engine/External/assimp/code/../include/assimp/SGSpatialSort.h"
    "D:/Engine/External/assimp/code/../include/assimp/GenericProperty.h"
    "D:/Engine/External/assimp/code/../include/assimp/SpatialSort.h"
    "D:/Engine/External/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/Engine/External/assimp/code/../include/assimp/SmallVector.h"
    "D:/Engine/External/assimp/code/../include/assimp/SmoothingGroups.h"
    "D:/Engine/External/assimp/code/../include/assimp/SmoothingGroups.inl"
    "D:/Engine/External/assimp/code/../include/assimp/StandardShapes.h"
    "D:/Engine/External/assimp/code/../include/assimp/RemoveComments.h"
    "D:/Engine/External/assimp/code/../include/assimp/Subdivision.h"
    "D:/Engine/External/assimp/code/../include/assimp/Vertex.h"
    "D:/Engine/External/assimp/code/../include/assimp/LineSplitter.h"
    "D:/Engine/External/assimp/code/../include/assimp/TinyFormatter.h"
    "D:/Engine/External/assimp/code/../include/assimp/Profiler.h"
    "D:/Engine/External/assimp/code/../include/assimp/LogAux.h"
    "D:/Engine/External/assimp/code/../include/assimp/Bitmap.h"
    "D:/Engine/External/assimp/code/../include/assimp/XMLTools.h"
    "D:/Engine/External/assimp/code/../include/assimp/IOStreamBuffer.h"
    "D:/Engine/External/assimp/code/../include/assimp/CreateAnimMesh.h"
    "D:/Engine/External/assimp/code/../include/assimp/XmlParser.h"
    "D:/Engine/External/assimp/code/../include/assimp/BlobIOSystem.h"
    "D:/Engine/External/assimp/code/../include/assimp/MathFunctions.h"
    "D:/Engine/External/assimp/code/../include/assimp/Exceptional.h"
    "D:/Engine/External/assimp/code/../include/assimp/ByteSwapper.h"
    "D:/Engine/External/assimp/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/Engine/External/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "D:/Engine/External/assimp/code/../include/assimp/Compiler/poppack1.h"
    "D:/Engine/External/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Engine/External/assimp/bin/Debug/assimp-vc145-mtd.pdb")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Engine/External/assimp/bin/Release/assimp-vc145-mt.pdb")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Engine/External/assimp/bin/MinSizeRel/assimp-vc145-mt.pdb")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Engine/External/assimp/bin/RelWithDebInfo/assimp-vc145-mt.pdb")
  endif()
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "D:/Engine/External/assimp/code/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
