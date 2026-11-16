
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

// sets
void Camera::pan(float dx, float dy, float viewportHeight) {

	if (viewportHeight <= 0.0f) return;
	float unitsPerPixel = visibleHeight() / viewportHeight;

	glm::vec3 offset = -getRight() * dx * unitsPerPixel
					 +  getUp()    * dy * unitsPerPixel;

	m_target += offset;
}

// uses the camera movement to accumalate camera yaw, pitch
void Camera::orbit(float deltaYaw, float deltaPitch) {

	m_yaw += glm::radians(deltaYaw);
	m_pitch += glm::radians(deltaPitch);

	wrapYaw();
	clampState();
}

void Camera::focusOn(glm::vec3 point) { m_target = point; }
void Camera::resetView() {

	m_target = DefaultTarget;
	setDistance(DefaultDistance);
	setAngles(DefaultYaw, DefaultPitch);
}

void Camera::frameBounds(glm::vec3 min, glm::vec3 max) {

	glm::vec3 center = (min + max) * 0.5f;
	float radius = glm::length(max - min) * 0.5f;

	m_target = center;
	if (radius <= 0.0f) return;

	float halfV   = m_fov * 0.5f;
	float halfH   = glm::atan(glm::tan(halfV) * m_aspect);
	float halfFov = glm::min(halfV, halfH);

	const float margin = 1.1f;
	setDistance(radius / glm::sin(halfFov) * margin);
}

// uses the mouse scroll to lower m_distance
void Camera::zoom(float factor) { m_distance *= factor; clampState(); }
void Camera::setDistance(float d) { m_distance = d; clampState(); }
void Camera::setTarget(glm::vec3 point) { m_target = point; }

void Camera::setAngles(float yaw, float pitch) { 

	m_yaw = glm::radians(yaw); 
	m_pitch = glm::radians(pitch); 

	wrapYaw();
	clampState();
}

void Camera::setAspect(float aspect) { m_aspect = aspect; }
void Camera::setOrthographic(bool set) { m_isOrtho = set; }
void Camera::setFov(float degress) { m_fov = glm::radians(degress); }

// gets
glm::mat4 Camera::getViewMatrix() const { return glm::lookAt(getPosition(), m_target, WorldUp); }
glm::mat4 Camera::getViewProjection() const { return getProjectionMatrix() * getViewMatrix(); }

glm::mat4 Camera::getProjectionMatrix() const {

	float h = visibleHeight();
	float w = visibleHeight() * m_aspect;
	glm::vec2 nfp = nearFar();

	if (m_isOrtho) return glm::ortho(-w / 2, w / 2, -h / 2, h / 2, nfp[0], nfp[1]);
	return glm::perspective(m_fov, m_aspect, nfp[0], nfp[1]);
}

glm::vec3 Camera::getPosition() const {

	float x = m_distance * glm::cos(m_pitch) * glm::sin(m_yaw);
	float y = m_distance * glm::sin(m_pitch);
	float z = m_distance * glm::cos(m_pitch) * glm::cos(m_yaw);
	return m_target + glm::vec3(x, y, z);
}

glm::vec3 Camera::getForward() const { return glm::normalize(m_target - getPosition()); }
glm::vec3 Camera::getRight() const { return glm::normalize(glm::cross(getForward(), WorldUp)); }
glm::vec3 Camera::getUp() const { return glm::cross(getRight(), getForward()); }

float Camera::getDistance() const { return m_distance; }
float Camera::getPitch() const { return glm::degrees(m_pitch); }
float Camera::getYaw() const { return glm::degrees(m_yaw); }
float Camera::getFov() const { return glm::degrees(m_fov); }

glm::vec3 Camera::getTarget() const { return m_target; }
bool Camera::isOrthographic() const { return m_isOrtho; }

// helpers
void Camera::clampState() { 

	m_pitch = glm::clamp(m_pitch, -pitchLimit, pitchLimit);
	m_distance = glm::clamp(m_distance, minDistance, maxDistance);
}

void Camera::wrapYaw() { m_yaw = glm::mod(m_yaw, glm::two_pi<float>()); }

glm::vec2 Camera::nearFar() const { return glm::vec2(m_distance * 0.01f, m_distance * 100.0f); }
float Camera::visibleHeight() const { return 2.0f * m_distance * glm::tan(m_fov * 0.5f); }