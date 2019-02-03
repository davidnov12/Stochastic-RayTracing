/*

*/

#pragma once

#include <memory>
#include <iostream>

#include <Window.h>
#include <UserInterface.h>
#include <Renderer.h>

#define APP_DEFAULT_WIDTH 1200
#define APP_DEFAULT_HEIGHT 800
#define APP_DEFAULT_TITLE "windowTitle"

template <typename RenderTech> class App {

public:

	App(unsigned width = APP_DEFAULT_WIDTH, unsigned height = APP_DEFAULT_HEIGHT, std::string title = APP_DEFAULT_TITLE);

	void init(unsigned width, unsigned height, std::string title);
	void run();

	std::shared_ptr<Window> win;
	std::shared_ptr<UserInterface> ui;
	std::shared_ptr<Renderer> ren;
	std::shared_ptr<UserInterface::uiData> ui_data;
	std::shared_ptr<Scene> scene;

};

template<typename RenderTech>
inline App<RenderTech>::App(unsigned width, unsigned height, std::string title){

	init(width, height, title);

}

template<typename RenderTech>
inline void App<RenderTech>::init(unsigned width, unsigned height, std::string title){

	win = std::make_shared<Window>(width, height, title);
	win->showWindow();

	ui_data = std::make_shared<UserInterface::uiData>();
	ui = std::make_shared<UserInterface>(win, ui_data);
	ui->initGUI();

	ren = std::make_shared<RenderTech>();
	ren->setWindowObject(win);
	ren->init();

	scene = std::make_shared<Scene>();
	//Scene s;
	// load ....
	//s.loadScene("");
	//ren->updateScene(s);
	
}

template<typename RenderTech>
inline void App<RenderTech>::run(){

	while (!win->isClosed()) {
		
		if (ui_data->changeNotify) {
			ui_data->changeNotify = false;

			//for (int i = 0; i < 100000; i++) printf("photon\n");
			//Scene s;
			// load ....
			std::cout << ui_data->sceneFile << std::endl;
			scene->loadScene(ui_data->sceneFile);
			ren->updateScene(*scene.get());
			
		}

		ren->render();
		ui->renderGUI();
		win->swapBuffers();
	}

}
