cmake_minimum_required(VERSION 3.0.2)
project(RemoteControl CXX ASM)

macro(addOptionFlag flag ONorOFF)
  option(Flag_${flag} "Flag ${Flag_${flag}}:" ${ONorOFF})
  message(STATUS "Flag_${flag}: ${Flag_${flag}}")
  if(Flag_${flag})
    list(APPEND COMMON_FLAGS -${flag})
  endif()
endmacro()

addOptionFlag(Wall ON)
addOptionFlag(pedantic ON)
addOptionFlag(Waddress ON)
addOptionFlag(zdefs ON)
addOptionFlag(Wno-psabi ON)
addOptionFlag(Wunused-parameter ON)
addOptionFlag(Wextra ON)
addOptionFlag(Werror OFF)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/CommonDrivers/src")
  message(FATAL_ERROR
    "\nThere is no commonDriversMCU repository."
    "\nTry 'git submodule update --init --recursive'"
  )
endif()

add_subdirectory(src)
add_subdirectory(simulator)
add_subdirectory(unitTests)
add_subdirectory(stm32)
