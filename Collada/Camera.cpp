#include "stdafx.h"
#include "Camera.h"

std::shared_ptr<collada::Camera> collada::Camera::define(const xmlParser::Node &node)
{
	return (std::make_shared<Camera>(node));
}

collada::Camera::Camera(const xmlParser::Node &node)
	: width(0), height(0), fov(45),
	nearestPoint(0.1), furthestPoint(100)
{
	const xmlParser::Node &optic = node["optics"]["technique_common"];
	if (optic.isNodeExist("perspective"))
	{
		perspective(optic["perspective"]);
	}
	else if (optic.isNodeExist("orthographic"))
	{
		orthographic(optic["orthographic"]);
	}
}

void collada::Camera::perspective(const xmlParser::Node &node)
{
	mode = graphic::CameraMode::PERSPECTIVE;

	if (node.isNodeExist("xfov"))
	{
		fov = node["xfov"].getContent().getNumber<float>();
	}

	if (node.isNodeExist("znear"))
	{
		nearestPoint = node["znear"].getContent().getNumber<float>();
	}

	if (node.isNodeExist("zfar"))
	{
		furthestPoint = node["zfar"].getContent().getNumber<float>();
	}
}

void collada::Camera::orthographic(const xmlParser::Node &node)
{
	mode = graphic::CameraMode::ORTHO;

	if (node.isNodeExist("znear"))
	{
		nearestPoint = node["znear"].getContent().getNumber<float>();
	}

	if (node.isNodeExist("zfar"))
	{
		nearestPoint = node["zfar"].getContent().getNumber<float>();
	}
}

void collada::Camera::instanciate(graphic::Scene &scene, glm::mat4 transform)
{
	graphic::Camera &camera = scene.getCamera();

	camera.setNewPosition(transform);

	if (mode == graphic::CameraMode::PERSPECTIVE)
	{
		camera.setFov(fov);
	}
	else if (mode == graphic::CameraMode::ORTHO)
	{

	}
	
	camera.setNearestPoint(nearestPoint);
	camera.setFurthestPoint(furthestPoint);
}