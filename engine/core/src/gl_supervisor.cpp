#include "GLSupervisor.hpp"
#include "Logger.hpp"

namespace birb
{
	gl_supervisor::gl_supervisor()
	{
#ifndef NDEBUG
		////////////////////////////////////////////////////////////////////
		// ^ The OpenGL error was detected at the start of the function ^ //
		process_gl_errors();                                              //
		////////////////////////////////////////////////////////////////////
#endif
	}

	gl_supervisor::~gl_supervisor()
	{
#ifndef NDEBUG
		//////////////////////////////////////////////////////////////////
		// v The OpenGL error was detected at the end of the function v //
		process_gl_errors();                                            //
		//////////////////////////////////////////////////////////////////
#endif
	}
}
