#pragma once

#include <memory>

#include <Window.h>
#include <Scene.h>

class Renderer {

public:

	virtual void init(){}
	virtual void render(){}
	virtual void updateScene(Scene& s){}
	virtual void setWindowObject(std::shared_ptr<Window> win){}


};