#include "Camera.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "FBO.hpp"
#include "Logger.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Timestep.hpp"
#include "Vector.hpp"
#include "Viewport.hpp"
#include "Window.hpp"

#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int main(void)
{
	birb::window window("Birb3D editor", birb::vec2<int>(1920, 1080));
	window.init_imgui();

	birb::timestep timestep;

	birb::scene scene;
	editor::game_viewport game_viewport(scene, window, timestep);

	// Test model
	birb::entity suzanne = scene.create_entity();

	birb::model suzanne_model("suzanne.obj");
	suzanne.add_component(suzanne_model);

	birb::component::transform transform;
	transform.position.z = -4.0f;
	transform.rotation.y = 45.0f;
	suzanne.add_component(transform);

	birb::shader shader("default", "default_color");
	shader.reset_lights();
	shader.set_vec3("material.diffuse", {0.2f, 0.3f, 0.4f});
	shader.set_vec3("material.specular", {0.9f, 0.8f, 0.7f});
	shader.set_float("material.shininess", 32);
	suzanne.add_component(shader);

	birb::vec2<int> last_viewport_size = {1280, 720};
	birb::fbo fbo(last_viewport_size);

	while (!window.should_close())
	{
		window.clear();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		game_viewport.draw_viewport();

		window.flip();
		window.poll();
		timestep.step();
	}

	return 0;
}
