/*

*/

#pragma once


#include <Window.h>

class UserInterface {

public:

	typedef struct {
		int shadowSamples = 1;
		int indirectSamples = 0;
		int dofSamples = 1;
		int aoSamples = 0;
		std::string sceneFile = "";
		int bvhType;
		bool renderMode = true;
		bool changeNotify = false;
	} uiData;

	UserInterface(std::shared_ptr<Window> w, std::shared_ptr<uiData> d) {
		window = w;
		data = d;
	}

	~UserInterface();

	void initGUI();
	void renderGUI();
	
	std::shared_ptr<Window> window;
	std::shared_ptr<uiData> data;

	bool showGUI = true;
	bool showFileBrowser = false;
	bool showProfiler = false;
	bool showbvhChoose = false;
	bool showHelp = false;

};