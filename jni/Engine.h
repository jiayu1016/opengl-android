/*
 * Engine.h
 *
 *  Created on: Aug 28, 2013
 *      Author: Simon Racz
 */

#include <cstdint>

#include <EGL/egl.h>

#ifndef ENGINE_H
#define ENGINE_H

struct android_app;
struct AInputEvent;

namespace Chaonis {

struct SavedState {
	int currentShader;
};

class Engine {
	android_app* app;
	int width = 0;
	int height = 0;
	EGLDisplay display = EGL_NO_DISPLAY;
	EGLSurface surface = EGL_NO_CONTEXT;
	EGLContext context = EGL_NO_SURFACE;
	enum class Shaders : int {FLAT = 0, PHONG, GOURAUD};
	Shaders currentShader;
public:
	Engine(android_app* app);

	void handleCommand(int32_t cmd);
	int32_t handleInput(AInputEvent* event);
	void restoreFromState(SavedState* state);
	void saveState();

	void initDisplay();
	void terminateDisplay();
	//void drawFrame(float time);
	void drawFrame();

	bool paused;
};

} //namespace

#endif /* ENGINE_H */
