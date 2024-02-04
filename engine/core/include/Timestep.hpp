#pragma once

namespace birb
{
	class timestep
	{
	public:
		timestep();
		timestep(double target_fps);

		// Call this at the end of the gameloop to sleep
		// and keep up the target framerate. This also refreshes deltatime
		void step();

		// Time in seconds since the end of last frame
		double deltatime() const;

		// Get the current FPS. To get the most accurate value,
		// call this function after swapping buffers (calling flip())
		double fps() const;

		// Change the target FPS during runtime
		void set_target_fps(double target_fps);

	private:
		double frame_end;

		// Target FPS (frames per second)
		double target_fps;

		// Target frametime (milliseconds per frame)
		double target_frametime;
	};
}
