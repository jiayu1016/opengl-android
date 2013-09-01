/*
 * Engine.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: Simon Racz
 */

#include "Engine.h"

#include <GLES/gl.h>
//#include <GLES2/gl2.h>

#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/input.h>

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "Engine.cpp", __VA_ARGS__))

namespace Chaonis {

Engine::Engine(android_app* app):paused{true}, app{app}
{
	LOGD("At Engine constructor");
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;
}

void Engine::handleCommand(int32_t cmd)
{
	LOGD("At handleCommand()");

	switch (cmd) {
	        case APP_CMD_SAVE_STATE:
	            saveState();
	            break;
	        case APP_CMD_INIT_WINDOW:
	            // The window is being shown, get it ready.
	            if (app->window != NULL) {
	                initDisplay();
	                drawFrame();
	            }
	            break;
	        case APP_CMD_TERM_WINDOW:
	            // The window is being hidden or closed, clean it up.
	            terminateDisplay();
	            break;
	        case APP_CMD_GAINED_FOCUS:
	        	LOGD("At APP_CMD_GAINED_FOCUS");
	        	paused = false;
	            break;
	        case APP_CMD_LOST_FOCUS:
	        	LOGD("At APP_CMD_LOST_FOCUS");
	            // Also stop animating.
	            paused = true;
	            drawFrame();
	            break;
	    }
	return;
}

int32_t Engine::handleInput(AInputEvent* event)
{
	LOGD("At handleInput()");

	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		//handle rotation
		return 1;
	}
	return 0;
}

void Engine::restoreFromState(SavedState* state)
{
	currentShader = static_cast<Shaders> (state->currentShader);
}

void Engine::saveState()
{
	SavedState* savedState = static_cast<SavedState*>(malloc(sizeof(SavedState)));
	savedState->currentShader = static_cast<int>(currentShader);
	app->savedState = savedState;
	app->savedStateSize = sizeof(SavedState);
}

void Engine::initDisplay()
{
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGD("Unable to eglMakeCurrent");
        return;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    this->display = display;
    this->context = context;
    this->surface = surface;
    width = w;
    height = h;

    // Initialize GL state.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);
}

void Engine::terminateDisplay()
{
	if (display != EGL_NO_DISPLAY ) {
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE,
				EGL_NO_CONTEXT );
		if (context != EGL_NO_CONTEXT ) {
			eglDestroyContext(display, context);
		}
		if (surface != EGL_NO_SURFACE ) {
			eglDestroySurface(display, surface);
		}
		eglTerminate(display);
	}
	paused = true;
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;

}

void Engine::drawFrame()
{
	if (display == NULL) {
		return;
	}

	// Just fill the screen with a color.
	glClearColor(0.9,0.9,0.9,1);
	glClear(GL_COLOR_BUFFER_BIT);

	eglSwapBuffers(display, surface);
}

} //namespace
