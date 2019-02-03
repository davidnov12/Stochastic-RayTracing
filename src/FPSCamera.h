/*
Projekt PGPa + GMU
David Novák, xnovak1l
2018
*/
#pragma once

#include <Camera.h>

#include <vector>


class FPSCamera : public Camera {

public:
	/**
	* \brief
	* \param position
	* \param up
	* \param yaw
	* \param pitch
	*/
	FPSCamera();


	/**
	* \brief
	*/
	~FPSCamera();


	/*
	*/
	std::vector<glm::vec3> getScreenCoords();


	/**
	* \brief
	* \param dir
	* \param delta
	*/
	virtual void keyboardEvent(cameraMove dir, float delta) override;


	/**
	* \brief
	* \param x
	* \param y
	*/
	virtual void mouseEvent(float x, float y) override;



	/**
	* \brief
	* \param y
	*/
	virtual void scrollEvent(float y) override;

};