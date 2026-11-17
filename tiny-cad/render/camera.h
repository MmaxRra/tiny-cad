#pragma once

#include <glm/glm.hpp>

class Camera {

public:

	Camera() = default;
	~Camera() = default;

	// camera sets
	void pan(float dx, float dy, float viewportHeight);
	void orbit(float deltaYaw, float deltaPitch);
	void zoom(float factor);
	
	void resetView();
	void focusOn(glm::vec3 point);
	void frameBounds(glm::vec3 min, glm::vec3 max);

	void setTarget(glm::vec3 point);
	void setDistance(float d);
	void setAngles(float yaw, float pitch);

	void setAspect(float aspect);
	void setOrthographic(bool set);
	void setFov(float degrees);

	// camera outputs
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewProjection() const;

	glm::vec3 getPosition() const;
	glm::vec3 getForward() const;
	glm::vec3 getRight() const;
	glm::vec3 getUp() const;

	float getDistance() const;
	float getPitch() const;
	float getYaw() const;
	float getFov() const;

	glm::vec3 getTarget() const;
	bool isOrthographic() const;

private:

	// define set up
	static constexpr glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	static constexpr float maxDistance = 20000.0f;
	static constexpr float minDistance = 0.5f;
	static constexpr float pitchLimit  = glm::radians(89.0f); // +-

	static constexpr glm::vec3 DefaultTarget = glm::vec3(0.0f);
	static constexpr float DefaultDistance   = 100.0f;
	static constexpr float DefaultYaw        = 45.5f;
	static constexpr float DefaultPitch      = 30.0f;

	// point to orbit around
	glm::vec3 m_target = DefaultTarget;
	float m_distance   = DefaultDistance;
	float m_yaw		   = glm::radians(DefaultYaw);
	float m_pitch	   = glm::radians(DefaultPitch);

	float m_fov        = glm::radians(45.0f);
	float m_aspect     = 16.0f / 9.0f;
	bool  m_isOrtho    = false;

	void clampState();
	void wrapYaw();

	glm::vec2 nearFar() const;
	float visibleHeight() const;
};