cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

# set the project name
project(ComSquare)

# show compilation warnings
add_compile_options(-W -Wall -Wextra -Wshadow)

# make unit tests
add_executable(unit_tests
        tests/CPU/testAddressingMode.cpp
        tests/CPU/testInterupts.cpp
        tests/testMemoryBus.cpp
        tests/tests.hpp
        sources/SNES.cpp
        sources/SNES.hpp
        sources/Memory/MemoryBus.cpp
        sources/Memory/MemoryBus.hpp
        sources/Memory/AMemory.hpp
        sources/Memory/AMemory.cpp
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
        sources/Memory/ARectangleMemory.cpp
        sources/Memory/ARectangleMemory.hpp
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
        sources/Utility/Utility.hpp
        sources/Utility/Utility.cpp
        sources/CPU/Instructions/BitsInstructions.cpp
        tests/CPU/testBits.cpp
        sources/APU/Instructions/Bit.cpp
        sources/APU/Instructions/Stack.cpp
        sources/APU/Instructions/Subroutine.cpp
        sources/APU/Instructions/ProgramFlow.cpp
        sources/APU/Operand.cpp
        sources/APU/Instructions/DecimalCompensation.cpp
        sources/APU/Instructions/MultiplicationDivision.cpp
        sources/APU/Instructions/16bitArithmetic.cpp
        sources/APU/Instructions/16bitDataTransmission.cpp
        sources/APU/Instructions/8bitShiftRotation.cpp
        sources/APU/Instructions/8bitIncrementDecrement.cpp
        sources/APU/Instructions/8bitLogical.cpp
        tests/APU/testOperand.cpp
        sources/APU/Instructions/8bitArithmetic.cpp
        sources/APU/Instructions/8bitDataTransmission.cpp
        sources/APU/IPL/IPL.hpp
        sources/APU/IPL/IPL.cpp
        tests/CPU/Math/testSBC.cpp
        sources/CPU/Instructions/TransferRegisters.cpp
        tests/CPU/testTransfers.cpp
        sources/CPU/AddressingModes.cpp
        sources/Models/Components.hpp
        sources/CPU/Instruction.hpp
        sources/Exceptions/DebuggableError.hpp
        tests/CPU/Math/testOthersMath.cpp
        tests/testRectangleMemory.cpp
        tests/CPU/Math/testCMP.cpp
        sources/PPU/Backgrounds.cpp
        sources/PPU/Background.cpp
        sources/PPU/Background.hpp
        sources/CPU/DMA/DMA.cpp
        sources/CPU/DMA/DMA.hpp
        tests/CPU/testDMA.cpp sources/Memory/IMemory.hpp)

# include criterion & coverage
target_link_libraries(unit_tests criterion -lgcov)
target_compile_options(unit_tests PUBLIC -fprofile-arcs -ftest-coverage)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# make app
add_executable(ComSquare
        sources/main.cpp
        sources/SNES.cpp
        sources/SNES.hpp
        sources/Memory/MemoryBus.cpp
        sources/Memory/MemoryBus.hpp
        sources/Memory/AMemory.hpp
        sources/Memory/AMemory.cpp
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
        sources/Memory/ARectangleMemory.cpp
        sources/Memory/ARectangleMemory.hpp
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
        sources/Debugger/CPU/CPUDebug.cpp
        sources/Debugger/CPU/CPUDebug.hpp
        sources/Renderer/QtRenderer/QtSFML.cpp
        sources/Renderer/QtRenderer/QtSFML.hpp
        sources/Renderer/QtRenderer/QtWidgetSFML.cpp
        sources/Renderer/QtRenderer/QtWidgetSFML.hpp
        ui/cpuView.ui
        ui/ramView.ui
        ui/cartridgeView.ui
        ui/apuView.ui
        ui/busView.ui
        resources/appResources.qrc
        sources/Utility/Utility.hpp
        sources/Debugger/MemoryViewer.cpp
        sources/Debugger/MemoryViewer.hpp
        sources/Utility/Utility.cpp
        sources/Debugger/HeaderViewer.cpp
        sources/Debugger/HeaderViewer.hpp
        sources/CPU/Instructions/BitsInstructions.cpp
        sources/Debugger/HeaderViewer.cpp
        sources/Debugger/HeaderViewer.hpp
        sources/Debugger/APUDebug.hpp
        sources/Debugger/APUDebug.cpp
        sources/APU/Instructions/Stack.cpp
        sources/APU/Instructions/Subroutine.cpp
        sources/APU/Instructions/ProgramFlow.cpp
        sources/APU/Operand.cpp
        sources/APU/Instructions/DecimalCompensation.cpp
        sources/APU/Instructions/MultiplicationDivision.cpp
        sources/APU/Instructions/16bitArithmetic.cpp
        sources/APU/Instructions/16bitDataTransmission.cpp
        sources/APU/Instructions/8bitShiftRotation.cpp
        sources/APU/Instructions/8bitIncrementDecrement.cpp
        sources/APU/Instructions/8bitLogical.cpp
        sources/APU/Instructions/8bitArithmetic.cpp
        sources/APU/Instructions/8bitDataTransmission.cpp
        sources/APU/IPL/IPL.hpp
        sources/APU/IPL/IPL.cpp
        sources/CPU/Instructions/TransferRegisters.cpp
        sources/CPU/AddressingModes.cpp
        sources/Debugger/MemoryBusDebug.cpp
        sources/Debugger/MemoryBusDebug.hpp
        sources/Debugger/ClosableWindow.hpp
        sources/Models/Components.hpp
        sources/CPU/Instruction.hpp
        sources/Exceptions/DebuggableError.hpp
        sources/Debugger/CPU/Disassembly.cpp
        sources/Models/Components.hpp
        sources/Debugger/CGramDebug.cpp
        sources/Debugger/CGramDebug.hpp
        sources/Models/Vector2.hpp
        sources/PPU/Backgrounds.cpp
        sources/PPU/Background.cpp
        sources/PPU/Background.hpp
        sources/CPU/DMA/DMA.cpp
        sources/CPU/DMA/DMA.hpp
        ui/registersView.ui
        sources/Debugger/RegisterViewer.cpp
        sources/Debugger/RegisterViewer.hpp
        sources/Memory/IMemory.hpp)

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