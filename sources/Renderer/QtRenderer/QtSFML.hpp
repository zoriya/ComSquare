//
// Created by anonymus-raccoon on 2/15/20.
//

#pragma once

#include <QtWidgets/QWidget>
#include <SFML/Graphics/RenderWindow.hpp>
#include <QtCore/QTimer>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include "Renderer/IRenderer.hpp"
#include "Renderer/SFRenderer.hpp"
#include "QtWidgetSFML.hpp"

namespace ComSquare::Renderer
{
	//! @brief The SFML window that manage the update.
	class QtFullSFML : public QtWidgetSFML {
	private:
		//! @brief The snes to update.
		SNES &_snes;
		void onUpdate() override;
	public:
#ifdef DEBUGGER_ENABLED
		//! @brief Action called when clicking on the enable CPU debugger button.
		void enableDebugCPU();
		//! @brief Action called when clicking on the enable Ram viewer button.
		void enableRamViewer();
		//! @brief Action called when clicking on the enable Header viewer button.
		void enableHeaderViewer();
		//! @brief Action called when clicking on the enable APU debugger button.
		void enableDebugAPU();
		//! @brief Action called when clicking on the enable Memory Bus debugger button.
		void enableDebugBus();
		//! @brief Action called when clicking on the enable Palette viewer button.
		void enableCgramViewer();
		//! @brief Action called when clicking on the enable DMA viewer button.
		void enableRegisterViewer();
		//! @brief Action called when clicking on the enable Tile viewer button
		void enableTileViewer();
#endif

		//! @brief Action called when clicking on the reset button.
		void reset();

		QtFullSFML(SNES &snes, QWidget* parent, const QPoint& position, const QSize& size, int frameRate = 0);
		QtFullSFML(const QtFullSFML &) = delete;
		QtFullSFML &operator=(const QtFullSFML &) = delete;
		~QtFullSFML() override = default;
	};

	//! @brief A SFML renderer inside a QT widget.
	class QtSFML : public IRenderer {
	private:
		//! @brief The main window that the app reside on.
		QWidget *_window;
	protected:
		//! @brief The SFML widget.
		QtFullSFML *_sfWidget = nullptr;
	public:
		//! @brief Use this function to create the window.
		//! @param maxFPS The number of FPS you aim to run on.
		void createWindow(SNES &snes, int maxFPS) override;
		//! @brief Add a pixel to the buffer to the coordinates x, y with the color rgba.
		//! @param X horizontal index.
		//! @param Y vertical index.
		//! @param rgba The color of the pixel.
		void putPixel(unsigned y, unsigned x, uint32_t rgba) override;
		//! @brief This function doesn't do anything because QT internally handle drawing to the screen.
		void drawScreen() override;
		//! @brief Playing all samples from buffer
		//! @param samples Buffer containing samples
		//! @param sampleCount number of samples inside buffer
		void playAudio(std::span<int16_t> samples, uint64_t sampleCount) override;
		//! @brief Set a new name to the window, if there is already a name it will be overwrite.
		//! @param newWindowName new title for the window.
		void setWindowName(std::string &newWindowName) override;
		//! @brief Constructor that return a SFML renderer inside a QT widget.
		explicit QtSFML(QWidget *parentWidget);
		QtSFML(const QtSFML &) = delete;
		QtSFML &operator=(const QtSFML &) = delete;
		~QtSFML() = default;
	};

	class QtSFMLWindow : public QtSFML {
	private:
		//! @brief The main window that the app reside on.
		QMainWindow _window;
	public:
		//! @brief Use this function to create the window.
		//! @param maxFPS The number of FPS you aim to run on.
		void createWindow(SNES &snes, int maxFPS) override;
		//! @brief Constructor that return a SFML renderer inside a QT window.
		//! @param height _height of the window.
		//! @param width _width of the window.
		QtSFMLWindow(int height, int width);
		QtSFMLWindow(const QtSFMLWindow &) = delete;
		QtSFMLWindow &operator=(const QtSFMLWindow &) = delete;
		~QtSFMLWindow() = default;
	};
}
