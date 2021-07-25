cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
project(ComSquare)

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wshadow -Wno-unused-parameter -W -g")
endif ()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(sources)

set(SOURCES
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
	sources/Models/Ints.hpp
	sources/Models/Ints.hpp
	sources/Ram/Ram.cpp
	sources/Ram/Ram.hpp
	sources/Memory/MemoryShadow.cpp
	sources/Memory/MemoryShadow.hpp
	sources/Memory/ARectangleMemory.cpp
	sources/Memory/ARectangleMemory.hpp
	sources/APU/DSP/DSP.cpp
	sources/APU/DSP/DSP.hpp
	sources/Renderer/IRenderer.hpp
	sources/Renderer/NoRenderer.cpp
	sources/Renderer/NoRenderer.hpp
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
	sources/Utility/Utility.hpp
	sources/Utility/Utility.cpp
	sources/CPU/Instructions/BitsInstructions.cpp
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
	sources/Models/Components.hpp
	sources/CPU/Instruction.hpp
	sources/Exceptions/DebuggableError.hpp
	sources/Models/Components.hpp
	sources/Models/Vector2.hpp
	sources/PPU/Background.cpp
	sources/PPU/Background.hpp
	sources/CPU/DMA/DMA.cpp
	sources/CPU/DMA/DMA.hpp
	sources/Memory/IMemory.hpp
	sources/APU/DSP/Voice.cpp
	sources/APU/DSP/Echo.cpp
	sources/APU/DSP/Gauss.cpp
	sources/APU/DSP/Envelope.cpp
	sources/APU/DSP/Timer.cpp
	sources/APU/DSP/BRR.cpp
	sources/PPU/PPUUtils.cpp
	sources/PPU/Tile.hpp
	sources/PPU/TileRenderer.cpp
	sources/PPU/TileRenderer.hpp
	sources/PPU/Tile.hpp
	sources/CPU/Registers.hpp
	sources/Memory/IMemoryBus.hpp
	sources/Models/Callback.hpp
	sources/Models/Logger.hpp
	sources/PPU/PpuDebug.cpp
	sources/PPU/PpuDebug.hpp
	sources/PPU/PPURegisters.hpp
	)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ./)

add_executable(comsquare
	sources/main.cpp
	${SOURCES}
	sources/Renderer/SFRenderer.hpp
	sources/Renderer/SFRenderer.cpp
	sources/Renderer/QtRenderer/QtSFML.cpp
	sources/Renderer/QtRenderer/QtSFML.hpp
	sources/Renderer/QtRenderer/QtWidgetSFML.cpp
	sources/Renderer/QtRenderer/QtWidgetSFML.hpp
	sources/Renderer/QtRenderer/QtSfmlTileRenderer.cpp
	sources/Renderer/QtRenderer/QtSfmlTileRenderer.hpp
	sources/Renderer/QtRenderer/QtSfmlTileRenderer.cpp
	sources/Renderer/QtRenderer/QtSfmlTileRenderer.hpp
	sources/Debugger/ClosableWindow.hpp
	sources/Debugger/CPU/CPUDebug.cpp
	sources/Debugger/CPU/CPUDebug.hpp
	sources/Debugger/CPU/Disassembly.cpp
	sources/Debugger/CPU/SymbolLoaders/WlaDx.cpp
	sources/Debugger/CPU/SymbolLoaders/WlaDx.hpp
	sources/Debugger/MemoryViewer.cpp
	sources/Debugger/MemoryViewer.hpp
	sources/Debugger/HeaderViewer.cpp
	sources/Debugger/HeaderViewer.hpp
	sources/Debugger/APUDebug.hpp
	sources/Debugger/APUDebug.cpp
	sources/Debugger/MemoryBusDebug.cpp
	sources/Debugger/MemoryBusDebug.hpp
	sources/Debugger/CGramDebug.cpp
	sources/Debugger/CGramDebug.hpp
	sources/Debugger/RegisterViewer.cpp
	sources/Debugger/RegisterViewer.hpp
	sources/Debugger/TileViewer/TileViewer.cpp
	sources/Debugger/TileViewer/TileViewer.hpp
	sources/Debugger/TileViewer/RAMTileRenderer.cpp
	sources/Debugger/TileViewer/RAMTileRenderer.hpp
	ui/tileView.ui
	ui/registersView.ui
	ui/cpuView.ui
	ui/ramView.ui
	ui/cartridgeView.ui
	ui/apuView.ui
	ui/busView.ui
	resources/appResources.qrc
	)
target_include_directories(comsquare PUBLIC ${PROJECT_BINARY_DIR})
target_compile_definitions(comsquare PUBLIC DEBUGGER_ENABLED)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(comsquare
	sfml-graphics
	sfml-window
	sfml-system
	sfml-audio
	sfml-network
	Qt5::Widgets
	)

add_executable(unit_tests EXCLUDE_FROM_ALL
	${SOURCES}
	tests/tests.hpp
	tests/CPU/testInterupts.cpp
	tests/APU/testAPUInstructions.cpp
	tests/APU/testAPU.cpp
	tests/PPU/testPpuWrite.cpp
	tests/PPU/testPpuWriteFromVmain.cpp
	tests/CPU/Math/testADC.cpp
	tests/CPU/testStore.cpp
	tests/CPU/testInternal.cpp
	tests/CPU/testBits.cpp
	tests/APU/testOperand.cpp
	tests/CPU/Math/testSBC.cpp
	tests/CPU/testTransfers.cpp
	tests/CPU/Math/testOthersMath.cpp
	tests/testRectangleMemory.cpp
	tests/CPU/Math/testCMP.cpp
	tests/CPU/testDMA.cpp
	tests/CPU/testAddressingMode.cpp
	tests/testMemoryBus.cpp
	tests/PPU/testPPUUtils.cpp
	tests/PPU/testCGRAMColorToRGBA.cpp
	)

target_include_directories(unit_tests PUBLIC tests)
target_compile_definitions(unit_tests PUBLIC TESTS)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/libs)
find_package(Catch2 REQUIRED)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

if (CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(unit_tests PRIVATE -lgcov)
	target_compile_options(unit_tests PUBLIC -fprofile-arcs -ftest-coverage)
endif ()
