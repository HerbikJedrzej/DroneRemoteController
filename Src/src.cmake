macro(addFlag flag)
  if(NOT ADDITIONAL_FLAGS MATCHES ${flag})
    set(ADDITIONAL_FLAGS "${ADDITIONAL_FLAGS} ${flag}")
  endif()
endmacro()

addFlag("-lm")

list(APPEND LOCAL_INCLUDE_PATHS
  ScreenDriver
  ScreenDriver/Screens
  baseDrivers
)

list(APPEND LOCAL_SOURCE_PATHS
  baseDrivers/ADC.cpp
  baseDrivers/BatteryObserver.cpp
  baseDrivers/LDC.cpp
  baseDrivers/ServoStructure.cpp
  ScreenDriver/Screens/BatteryTuner.cpp
  ScreenDriver/Screens/Directory.cpp
  ScreenDriver/Screens/MeasurementsAltitude.cpp
  ScreenDriver/Screens/MeasurementsPID.cpp
  ScreenDriver/Screens/PrintRadioData.cpp
  ScreenDriver/Screens/SelectMode.cpp
  ScreenDriver/Screens/SetAngleOffset.cpp
  ScreenDriver/Screens/SetPercentValue.cpp
  ScreenDriver/Screens/SetPIDparams.cpp
  ScreenDriver/Visualizer.cpp
)

function(prepend_list var prefix)
    set(temp "")
    foreach(f ${${var}})
        list(APPEND temp "${prefix}${f}")
    endforeach()
    set(${var} "${temp}" PARENT_SCOPE)
endfunction()
