#include "stdafx.h"
#include "Camera.h"

std::shared_ptr<collada::Camera> collada::Camera::define(const xmlParser::Node &node)
{
	return (std::make_shared<Camera>(node));
}

collada::Camera::Camera(const xmlParser::Node &node)
	: moveSpeed(false, 0), width(false, 0), height(false, 0),
	ratio(false, 0), fov(false, 0), nearestPoint(false, 0), furthestPoint(false, 0)
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
		fov.second = node["xfov"].getContent().getNumber<float>();
	}

	if (node.isNodeExist("aspect_ratio"))
	{
		ratio.second = node["aspect_ratio"].getContent().getNumber<float>();
	}

	if (node.isNodeExist("znear"))
	{
		nearestPoint.second = node["znear"].getContent().getNumber<float>();
	}

	if (node.isNodeExist("zfar"))
	{
		furthestPoint.second = node["zfar"].getContent().getNumber<float>();
	}
}

void collada::Camera::orthographic(const xmlParser::Node &node)
{
	mode = graphic::CameraMode::ORTHO;

	if (node.isNodeExist("znear"))
	{
		nearestPoint.second = node["znear"].getContent().getNumber<float>();
	}

	if (node.isNodeExist("zfar"))
	{
		nearestPoint.second = node["zfar"].getContent().getNumber<float>();
	}
}

void collada::Camera::instanciate(graphic::Scene &scene, glm::mat4 transform)
{
	graphic::Camera &camera = scene.getCamera();

	camera.setNewPosition(transform);

	if (mode == graphic::CameraMode::PERSPECTIVE)
	{
		if (fov.first)
			camera.setFov(fov.second);

		if (ratio.first)
			camera.setRatio(ratio.second);
	}
	else if (mode == graphic::CameraMode::ORTHO)
	{

	}

	if (nearestPoint.first)
		camera.setNearestPoint(nearestPoint.second);

	if (furthestPoint.first)
		camera.setFurthestPoint(furthestPoint.second);
}