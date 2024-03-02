#pragma once
#include <microprofile.h>

#define PROFILER_SCOPE_RENDER(NAME) MICROPROFILE_SCOPEI("Rendering", NAME, MP_LIGHTGREEN);
#define PROFILER_SCOPE_RENDER_FN() MICROPROFILE_SCOPEI("Rendering", __PRETTY_FUNCTION__, MP_LIGHTGREEN);

#define PROFILER_SCOPE_IO(NAME) MICROPROFILE_SCOPEI("I/O", NAME, MP_LIGHTBLUE);
#define PROFILER_SCOPE_IO_FN() MICROPROFILE_SCOPEI("I/O", __PRETTY_FUNCTION__, MP_LIGHTBLUE);

#define PROFILER_SCOPE_INPUT(NAME) MICROPROFILE_SCOPEI("Input", NAME, MP_GRAY);
#define PROFILER_SCOPE_INPUT_FN() MICROPROFILE_SCOPEI("Input", __PRETTY_FUNCTION__, MP_GRAY);

#define PROFILER_SCOPE_MISC(NAME) MICROPROFILE_SCOPEI("Misc.", NAME, MP_LIGHTYELLOW);
#define PROFILER_SCOPE_MISC_FN() MICROPROFILE_SCOPEI("Misc.", __PRETTY_FUNCTION__, MP_LIGHTYELLOW);
