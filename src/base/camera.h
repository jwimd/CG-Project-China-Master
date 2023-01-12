#pragma once

#include "object3d.h"

class Camera : public Object3D {
public:
	float fovy;
	float aspect;
	float left;
	float right;
	float bottom;
	float top;
	float znear;
	float zfar;

	glm::mat4 getViewMatrix() const;

	virtual glm::mat4 getProjectionMatrix() const = 0;
};


class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera(float fovy, float aspect, float znear, float zfar);

	~PerspectiveCamera() = default;

	glm::mat4 getProjectionMatrix() const override;
};


class OrthographicCamera : public Camera {
public:
	OrthographicCamera(float left, float right, float bottom, float top, float znear, float zfar);
	
	~OrthographicCamera() = default;

	glm::mat4 getProjectionMatrix() const override;
};