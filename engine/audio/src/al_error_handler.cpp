#include "ALErrorHandler.hpp"
#include "Logger.hpp"

namespace birb
{
	static constexpr char al_error_msg[] = "AL error: ";

	void log_al_error_message(const ALenum error)
	{
		switch (error)
		{
			case AL_NO_ERROR:
				break;

			case AL_INVALID_NAME:
				log_error(al_error_msg, "invalid name");
				break;

			case AL_INVALID_ENUM:
				log_error(al_error_msg, "invalid enum");
				break;

			case AL_INVALID_VALUE:
				log_error(al_error_msg, "invalid value");
				break;

			case AL_INVALID_OPERATION:
				log_error(al_error_msg, "invalid operation");
				break;

			case AL_OUT_OF_MEMORY:
				log_error(al_error_msg, "out of memory");
				break;
		}
	}
}
