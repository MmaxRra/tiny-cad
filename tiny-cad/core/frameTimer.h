#pragma once
#include <GLFW/glfw3.h>
#include <algorithm>

class FrameTimer {

public:

	void frameTick() {

		double now = glfwGetTime();
		m_dt   = std::min((float)(now - m_last), 0.1f);
		m_last = now;

		++m_frames;
		m_secondTicked = false;
		if (now - m_fpsTimer >= 1.0) {
			m_fps          = m_frames;
			m_frameMs      = 1.000f / (float)std::max(m_frames, 1);
			m_frames	   = 0;
			
			m_fpsTimer	   += 1.0;
			if (now - m_fpsTimer >= 1.0) m_fpsTimer = now;
			m_secondTicked = true;
		}
	}

	float  dt()           const { return m_dt; }           // seconds since last frame
	int    fps()          const { return m_fps; }          // updated once per second
	double time()         const { return m_last; }         // seconds since start
	float  frameMs()      const { return m_frameMs; }      // average ms per frame
	bool   secondTicked() const { return m_secondTicked; } // true one frame per second

private:

	double m_last = glfwGetTime();
	double m_fpsTimer = m_last;
	float  m_dt = 0.0f;
	float  m_frameMs = 0.0f;
	int    m_frames = 0;
	int    m_fps = 0;
	bool   m_secondTicked = false;
};