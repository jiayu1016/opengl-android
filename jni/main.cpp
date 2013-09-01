#include <jni.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include <cstdint>
#include "Engine.h"

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "main.cpp", __VA_ARGS__))

using namespace Chaonis;

static void engine_handle_cmd(android_app* app, int32_t cmd)
{
	static_cast<Engine*>(app->userData)->engine.handleCommand(cmd);
}

static int32_t engine_handle_input(android_app* app, AInputEvent* event)
{
	static_cast<Engine*>(app->userData)->handleInput(event);
}

void android_main(android_app* app)
{
	LOGD("At android_main()");

	app_dummy();

	Engine engine(app);

	app->userData = &engine;
	app->onAppCmd = engine_handle_cmd;
	app->onInputEvent = engine_handle_input;

	if (app->savedState != NULL) {
		engine.restoreFromState(static_cast<SavedState*>(app->savedState));
	}

    int ident;
    int events;
    struct android_poll_source* source;

    while (1) {
        // Read all pending events.

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.paused ? -1 : 0, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine.terminateDisplay();
                return;
            }
        }

        if (!engine.paused) {

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine.drawFrame();
        }
    }
}
