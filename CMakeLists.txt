

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(NKFADC_Unpacker)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO RHTTP)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)


#---Create a shared library with geneated dictionary
add_library(Sig SHARED src/Sig.cxx inc/Sig.h)
#
add_library(NKSig SHARED src/NKSig.cxx inc/NKSig.h)
target_link_libraries(NKSig Sig)
add_library(SigAna SHARED src/SigAna.cxx inc/SigAna.h)
target_link_libraries(SigAna Sig)

add_library(Hit SHARED src/Hit.cxx inc/Hit.h)
target_link_libraries(Hit SigAna)

add_library(EvtSimple SHARED src/EvtSimple.cxx inc/EvtSimple.h)
target_link_libraries(EvtSimple SigAna)

add_library(HitCrystal SHARED src/HitCrystal.cxx inc/HitCrystal.h)
target_link_libraries(HitCrystal Hit)
add_library(HitClover SHARED src/HitClover.cxx inc/HitClover.h)
target_link_libraries(HitClover HitCrystal Hit)
add_library(EvtASGARD SHARED src/EvtASGARD.cxx inc/EvtASGARD.h)
target_link_libraries(EvtASGARD HitClover)

add_library(HitX6 SHARED src/HitX6.cxx inc/HitX6.h)
target_link_libraries(HitX6 Hit)
add_library(EvtStarkJr SHARED src/EvtStarkJr.cxx inc/EvtStarkJr.h)
target_link_libraries(EvtStarkJr HitX6)
#
add_library(Event SHARED src/Event.cxx inc/Event.h)
target_link_libraries(Event EvtSimple EvtASGARD EvtStarkJr)
#
add_library(DataGetter SHARED src/DataGetter.cxx inc/DataGetter.h)
target_link_libraries(DataGetter Sig)
target_link_libraries(DataGetter TimeSorter)
add_library(NKFileReader SHARED src/NKFileReader.cxx inc/NKFileReader.h)
target_link_libraries(NKFileReader DataGetter NKSig)
add_library(NKMessageQueue SHARED src/NKMessageQueue.cxx inc/NKMessageQueue.h)
target_link_libraries(NKMessageQueue DataGetter NKSig rt)
#
add_library(TimeSorter SHARED src/TimeSorter.cxx inc/TimeSorter.h inc/Global.h)
#target_link_libraries(TimeSorter SigAna)
#
add_library(EventProcessor SHARED src/EventProcessor.cxx inc/EventProcessor.h inc/Global.h)
target_link_libraries(EventProcessor Event)
#target_link_libraries(EventProcessor SigAna)
#target_link_libraries(EventProcessor TimeSorter)
#target_link_libraries(EventProcessor TreeWriter)
#target_link_libraries(EventProcessor HistServerUser)
#
add_library(TreeWriter SHARED src/TreeWriter.cxx inc/TreeWriter.h inc/Global.h ${ROOT_LIBRARIES})
target_link_libraries(TreeWriter EvtASGARD)
target_link_libraries(TreeWriter ${ROOT_LIBRARIES})
add_library(HistServer SHARED src/HistServer.cxx inc/HistServer.h)
target_link_libraries(HistServer ${ROOT_LIBRARIES})

add_library(HistServerUser SHARED src/HistServerUser.cxx inc/HistServerUser.h)
target_link_libraries(HistServerUser HistServer ${ROOT_LIBRARIES})
#target_link_libraries(HistServerUser Event)
#target_link_libraries(HistServerUser ${ROOT_LIBRARIES})
#
add_library(Config SHARED src/Config.cxx inc/Config.h inc/Global.h)


#---Create  a main program using the library
add_executable(NKFADC_Unpacker Unpacker.cpp)
target_link_libraries(NKFADC_Unpacker DataGetter NKFileReader NKMessageQueue Config TimeSorter EventProcessor TreeWriter HistServer HistServerUser)
target_link_libraries(NKFADC_Unpacker ${ROOT_LIBRARIES})




#ROOT_GENERATE_DICTIONARY(EvtASGARD_dict MODULE TreeWriter LINKDEF EvtASGARDLinkDef.h)
#ROOT_GENERATE_DICTIONARY(EvtStarkJr_dict MODULE TreeWriter LINKDEF EvtStarkJrLinkDef.h)
ROOT_GENERATE_DICTIONARY(Event_dict MODULE TreeWriter LINKDEF EventLinkDef.h)
target_link_libraries(Event_dict EvtSimple EvtASGARD EvtStarkJr)


set(CONFIG_CSV
		config/det_map.csv
		config/erg_cal.csv
)


foreach(_script ${CONFIG_CSV})
	configure_file(
			${PROJECT_SOURCE_DIR}/${_script}
			${PROJECT_BINARY_DIR}/${_script}
			COPYONLY
			)
endforeach()



