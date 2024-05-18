#include "Camera.hpp"
#include "CameraInfoOverlay.hpp"
#include "Entity.hpp"
#include "Font.hpp"
#include "FontManager.hpp"
#include "Renderer.hpp"
#include "RendererOverlay.hpp"
#include "ShaderRef.hpp"
#include "Text.hpp"
#include "Timestep.hpp"
#include "Window.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

int main(void)
{
	birb::window window("Text rendering", birb::vec2<i32>(1280, 720));
	window.init_imgui();

	birb::timestep timestep;

	birb::scene scene;
	birb::camera camera;
	camera.position.z = -5;
	camera.near_clip = 0.0f;

	birb::renderer renderer;
	renderer.opt_blend(true);
	renderer.set_scene(scene);

	birb::overlay::renderer_overlay renderer_overlay(renderer);
	birb::overlay::camera_info camera_info(camera);

	birb::font_manager font_manager;
	birb::font manaspace = font_manager.load_font("manaspc.ttf", 48);

	birb::entity text_entity = scene.create_entity();

	birb::text text("Hello world!", manaspace, { 25.0f, 25.0f, 0.0f }, 1.0f, 0xFFFFFF);
	text_entity.add_component(text);

	// glm::mat4 projection_matrix = glm::ortho(0.0f, static_cast<float>(window.size().x), 0.0f, static_cast<float>(window.size().y));

	while (!window.should_close())
	{
		camera.process_input(window, timestep);

		window.clear();

		// Draw here
		renderer.draw_entities(camera.view_matrix(), camera.projection_matrix(birb::camera::projection_mode::orthographic, window.size()));

		renderer_overlay.draw();
		camera_info.draw();

		window.flip();

		window.poll();
		timestep.step();
	}
}
