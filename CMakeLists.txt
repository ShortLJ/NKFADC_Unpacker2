

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
add_library(Sig SHARED src/Sig.cxx inc/Sig.h)

add_library(NKSig SHARED src/NKSig.cxx inc/NKSig.h)
target_link_libraries(NKSig Sig)
add_library(SigAna SHARED src/SigAna.cxx inc/SigAna.h)
target_link_libraries(SigAna Sig)

add_library(Hit STATIC src/Hit.cxx inc/Hit.h)

add_library(HitCrystal SHARED src/HitCrystal.cxx inc/HitCrystal.h)
target_link_libraries(HitCrystal SigAna)
add_library(HitClover SHARED src/HitClover.cxx inc/HitClover.h)
target_link_libraries(HitClover HitCrystal)
add_library(EvtASGARD SHARED src/EvtASGARD.cxx inc/EvtASGARD.h)
target_link_libraries(EvtASGARD HitClover)

add_library(HitX6 SHARED src/HitX6.cxx inc/HitX6.h)
target_link_libraries(HitX6 SigAna)
add_library(EvtStarkJr SHARED src/EvtStarkJr.cxx inc/EvtStarkJr.h)
target_link_libraries(EvtASGARD HitX6)

add_library(Event SHARED src/Event.cxx inc/Event.h)
target_link_libraries(Event EvtASGARD EvtStarkJr)

add_library(DataGetter STATIC src/DataGetter.cxx inc/DataGetter.h)
target_link_libraries(DataGetter NKSig)
target_link_libraries(DataGetter TimeSorter)

add_library(TimeSorter SHARED src/TimeSorter.cxx inc/TimeSorter.h)
target_link_libraries(TimeSorter SigAna)

add_library(EventProcessor SHARED src/EventProcessor.cxx inc/EventProcessor.h)
target_link_libraries(EventProcessor SigAna)
target_link_libraries(EventProcessor TimeSorter)
target_link_libraries(EventProcessor TreeWriter)
target_link_libraries(EventProcessor HistServerUser)

add_library(TreeWriter SHARED src/TreeWriter.cxx inc/TreeWriter.h)
target_link_libraries(TreeWriter Event)
target_link_libraries(TreeWriter ${ROOT_LIBRARIES})
add_library(HistServerUser SHARED src/HistServer.cxx inc/HistServer.h src/HistServerUser.cxx inc/HistServerUser.h)
target_link_libraries(HistServerUser Event)
target_link_libraries(HistServerUser ${ROOT_LIBRARIES})

add_library(Config SHARED src/Config.cxx inc/Config.h)


#---Create  a main program using the library
add_executable(NKFADC_Unpacker Unpacker.cpp)
target_link_libraries(NKFADC_Unpacker Config DataGetter TimeSorter EventProcessor TreeWriter HistServerUser)
target_link_libraries(NKFADC_Unpacker ${ROOT_LIBRARIES})




