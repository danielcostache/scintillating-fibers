cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(Simulation)

find_package(Geant4 REQUIRED)

include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

file(GLOB MACRO_FILES
    "*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB DATA_FILES
    "*.dat")
file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(sim src/sim.cc ${sources})
target_link_libraries(sim ${Geant4_LIBRARIES})

add_custom_target(Simulation DEPENDS sim)