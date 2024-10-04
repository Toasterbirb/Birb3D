#pragma once

#include <AL/al.h>

namespace birb
{
	void check_al_errors();
	void log_al_error_message(const ALenum error);
}
