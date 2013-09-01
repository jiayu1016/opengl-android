/*
 * Engine.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: Simon Racz
 */

#include "Engine.h"

#include <EGL/egl.h>

#include <GLES2/gl2.h>

#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/input.h>

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "Engine.cpp", __VA_ARGS__))

namespace Chaonis {

Engine::Engine(android_app* app):paused{true}, app{app}
{
	LOGD("At Engine constructor");
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

}

void Engine::terminateDisplay()
{


}

void Engine::drawFrame()
{

}

} //namespace
