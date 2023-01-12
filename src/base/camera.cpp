#include "camera.h"

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(position, position + getFront(), getUp());
}


PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float znear, float zfar) {
	this->fovy = fovy;
	this->aspect = aspect;
	this->znear = znear;
	this->zfar = zfar;
}

glm::mat4 PerspectiveCamera::getProjectionMatrix() const {
	return glm::perspective(fovy, aspect, znear, zfar);
}

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float znear, float zfar) {
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->znear = znear;
	this->zfar = zfar;
}

glm::mat4 OrthographicCamera::getProjectionMatrix() const {
	return glm::ortho(left, right, bottom, top, znear, zfar);
}