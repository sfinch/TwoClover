# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(TwoClover)

# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# Find ROOT
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
find_package(ROOT REQUIRED)

# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})

# Locate sources and headers for this project
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}/include
					${ROOT_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


# Add the executable, and link it to the Geant4 and root libraries
add_executable(TwoClover TwoClover.cc ${sources} ${headers})
target_link_libraries(TwoClover ${Geant4_LIBRARIES} )
target_link_libraries(TwoClover ${ROOT_LIBRARIES} )

# Copy all scripts to the build directory, i.e. the directory in which we
# build N03. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
set(TwoClover_SCRIPTS
    rndmSeed.mac run1.mac vis.mac gun.mac 
	holeDepth.mac
	hole_dead.mac deadLayer.mac deadLayer2.mac
	hole_dead2.mac
	plot.C 
  )

foreach(_script ${TwoClover_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

# Add program to the project targets
# (this avoids the need of typing the program name after make)
#add_custom_target(TwoClover DEPENDS TwoClover)

# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
install(TARGETS TwoClover DESTINATION bin)

