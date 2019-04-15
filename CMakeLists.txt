#Setup Project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(N_G_Det)
#Find Geant4

option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include
${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(N_G_Det N_G_Det.cc
  ${sources} ${add_sources} ${fortran_sources}
  ${headers} ${add_headers})
target_link_libraries(N_G_Det ${Geant4_LIBRARIES} ${HBOOK_LIBRARIES})

set(N_G_DET_Scripts
	init_vis.mac vis.mac AmBe.mac Beam.mac README
)
foreach(_script ${N_G_DET_Scripts})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

install(TARGETS N_G_Det DESTINATION bin)

