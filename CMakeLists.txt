

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(NKFADC_Unpacker)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
ROOT_GENERATE_DICTIONARY(EvtASGARD_dict MODULE NKFADC_Unpacker LINKDEF EvtASGARDLinkDef.h)
ROOT_GENERATE_DICTIONARY(EvtStarkJr_dict MODULE NKFADC_Unpacker LINKDEF EvtStarkJrLinkDef.h)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)


#---Create a shared library with geneated dictionary
add_library(Sig STATIC src/Sig.cxx inc/Sig.h)

add_library(NKSig SHARED src/NKSig.cxx inc/NKSig.h)
target_link_libraries(NKSig Sig)
add_library(SigAna SHARED src/SigAna.cxx inc/SigAna.h)
target_link_libraries(SigAna Sig)

add_library(Hit STATIC src/Hit.cxx inc/Hit.h)

add_library(HitCrystal SHARED src/HitCrystal.cxx inc/HitCrystal.h)
target_link_libraries(HitCrystal SigAna)
add_library(HitClover SHARED src/HitClover.cxx inc/HitClover.h)
target_link_libraries(HitClover HitCrystal)

add_library(HitX6 SHARED src/HitX6.cxx inc/HitX6.cxx)
target_link_libraries(HitX6 SigAna)

add_library(DataGetter STATIC src/DataGetter.cxx inc/DataGetter.h)
target_link_libraries(DataGetter NKSig)








