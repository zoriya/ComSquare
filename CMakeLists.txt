cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

# set the project name
project(ComSquare)

# show compilation warnings
add_compile_options(-W -Wall -Wextra -Wshadow)

# make unit tests
add_executable(unit_tests
        tests/CPU/testAddressingMode.cpp
        tests/tests.cpp
        tests/CPU/testInterupts.cpp
        tests/testMemoryBus.cpp
        tests/tests.hpp
        sources/SNES.cpp
        sources/SNES.hpp
        sources/Memory/MemoryBus.cpp
        sources/Memory/MemoryBus.hpp
        sources/Memory/IMemory.hpp
        sources/Memory/IMemory.cpp
        sources/PPU/PPU.cpp
        sources/PPU/PPU.hpp
        sources/CPU/CPU.cpp
        sources/CPU/CPU.hpp
        sources/Cartridge/Cartridge.cpp
        sources/Cartridge/Cartridge.hpp
        sources/Exceptions/NotImplementedException.hpp
        sources/APU/APU.hpp
        sources/APU/APU.cpp
        sources/Exceptions/InvalidAddress.hpp
        sources/Exceptions/InvalidRom.hpp
        sources/Models/Int24.hpp
        sources/Models/Int24.hpp
        sources/Ram/Ram.cpp
        sources/Ram/Ram.hpp
        sources/Memory/MemoryShadow.cpp
        sources/Memory/MemoryShadow.hpp
        sources/Memory/IRectangleMemory.cpp
        sources/Memory/IRectangleMemory.hpp
        sources/APU/DSP/DSP.cpp
        sources/APU/DSP/DSP.hpp
        sources/Renderer/IRenderer.hpp
        sources/Renderer/NoRenderer.hpp
        sources/Renderer/NoRenderer.cpp
        sources/Exceptions/InvalidAction.hpp
        sources/Cartridge/InterruptVectors.hpp
        sources/Memory/RectangleShadow.cpp
        sources/Memory/RectangleShadow.hpp
        sources/Exceptions/InvalidOpcode.hpp
        sources/CPU/Instructions/Interrupts.cpp
        sources/CPU/Instructions/MathematicalOperations.cpp
        sources/APU/Instructions/Standbys.cpp
        tests/APU/testAPUInstructions.cpp
        sources/APU/Instructions/ProgramStatusWord.cpp
        sources/APU/Instructions/Bit.cpp
        tests/APU/testAPU.cpp
        sources/CPU/Instructions/MathematicalOperations.cpp
        tests/CPU/testAddressingMode.cpp
        tests/tests.cpp
        tests/PPU/testPpuWrite.cpp
        tests/PPU/testPpuWriteFromVmain.cpp
        sources/CPU/Instructions/MathematicalOperations.cpp
        sources/CPU/Instructions/MemoryInstructions.cpp
        tests/CPU/Math/testADC.cpp
        tests/CPU/testStore.cpp
        sources/CPU/Instructions/InternalInstruction.cpp
        tests/CPU/testInternal.cpp
        sources/Ram/ExtendedRam.cpp
        sources/Ram/ExtendedRam.hpp
        sources/Utility/Utility.hpp sources/Utility/Utility.cpp
        sources/APU/Instructions/Bit.cpp
)

# include criterion & coverage
target_link_libraries(unit_tests criterion -lgcov)
target_compile_options(unit_tests PUBLIC -fprofile-arcs -ftest-coverage)

if ("${GITBUILD}" STREQUAL true)
    include_directories(ComSquare build/include)
    link_directories(ComSquare build/lib)
    message("Git build is true")
endif ()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# make app
add_executable(ComSquare
        main.cpp
        sources/SNES.cpp
        sources/SNES.hpp
        sources/Memory/MemoryBus.cpp
        sources/Memory/MemoryBus.hpp
        sources/Memory/IMemory.hpp
        sources/Memory/IMemory.cpp
        sources/PPU/PPU.cpp
        sources/PPU/PPU.hpp
        sources/CPU/CPU.cpp
        sources/CPU/CPU.hpp
        sources/Cartridge/Cartridge.cpp
        sources/Cartridge/Cartridge.hpp
        sources/Exceptions/NotImplementedException.hpp
        sources/APU/APU.hpp
        sources/APU/APU.cpp
        sources/Exceptions/InvalidAddress.hpp
        sources/Exceptions/InvalidRom.hpp
        sources/Models/Int24.hpp
        sources/Models/Int24.hpp
        sources/Ram/Ram.cpp
        sources/Ram/Ram.hpp
        sources/Memory/MemoryShadow.cpp
        sources/Memory/MemoryShadow.hpp
        sources/Memory/IRectangleMemory.cpp
        sources/Memory/IRectangleMemory.hpp
        sources/APU/DSP/DSP.cpp
        sources/APU/DSP/DSP.hpp
        sources/Renderer/IRenderer.hpp
        sources/Renderer/SFRenderer.hpp
        sources/Renderer/SFRenderer.cpp
        sources/Exceptions/InvalidAction.hpp
        sources/Cartridge/InterruptVectors.hpp
        sources/Memory/RectangleShadow.cpp
        sources/Memory/RectangleShadow.hpp
        sources/Exceptions/InvalidOpcode.hpp
        sources/CPU/Instructions/Interrupts.cpp
        sources/CPU/Instructions/MathematicalOperations.cpp
        sources/APU/Instructions/Standbys.cpp
        sources/APU/Instructions/ProgramStatusWord.cpp
        sources/APU/Instructions/Bit.cpp
        sources/CPU/Instructions/MathematicalOperations.cpp
        sources/CPU/Instructions/MemoryInstructions.cpp
        sources/CPU/Instructions/InternalInstruction.cpp
        sources/Ram/ExtendedRam.cpp
        sources/Ram/ExtendedRam.hpp
        sources/Debugger/CPUDebug.cpp
        sources/Debugger/CPUDebug.hpp
        sources/Renderer/QtRenderer/QtSFML.cpp
        sources/Renderer/QtRenderer/QtSFML.hpp
        sources/Renderer/QtRenderer/QtWidgetSFML.cpp
        sources/Renderer/QtRenderer/QtWidgetSFML.hpp
        ui/cpu.ui
        ui/ramView.ui
        ui/cartridgeView.ui
        ui/apuView.ui
        resources/appResources.qrc
        sources/Utility/Utility.hpp
        sources/Debugger/MemoryViewer.cpp
        sources/Debugger/MemoryViewer.hpp
        sources/Utility/Utility.cpp
        sources/Debugger/HeaderViewer.cpp
        sources/Debugger/HeaderViewer.hpp
        sources/Debugger/APUDebug.hpp
        sources/Debugger/APUDebug.cpp
)

target_compile_definitions(ComSquare PUBLIC DEBUGGER_ENABLED)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

target_link_libraries(ComSquare
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
        Qt5::Widgets
)