set(COMMON_CXX_ASM_COMPILE_FLAGS "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft")

set(CMAKE_CXX_FLAGS "${COMMON_CXX_ASM_COMPILE_FLAGS} -DSTM32F100xB -DUSE_HAL_DRIVER -specs=nosys.specs -specs=nano.specs -Wl,-Map=output.map -Wl,--gc-sections -fno-exceptions -fno-rtti -O1 -g3 -fmessage-length=0 -ffunction-sections -fno-exceptions -fno-rtti -MMD -MP")
set(CMAKE_ASM_FLAGS "${COMMON_CXX_ASM_COMPILE_FLAGS} -g")

set(OPENOCD_CFG "RemoteControl.cfg")
# set(OPENOCD_CFG "/usr/share/openocd/scripts/board/st_nucleo_f103rb.cfg")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "arm-none-eabi-as")
set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_CURRENT_LIST_DIR}/STM32F100R8Tx_FLASH.ld")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "" )