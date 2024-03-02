#include "Camera.hpp"
#include "CameraInfoOverlay.hpp"
#include "Entity.hpp"
#include "Model.hpp"
#include "PerformanceOverlay.hpp"
#include "Random.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Timestep.hpp"
#include "Vector.hpp"
#include "Window.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(void)
{
	birb::window window("Suzanne", birb::vec2<int>(1280, 720));
	window.lock_cursor_to_window();
	birb::renderer renderer;
	birb::timestep timestep;
	birb::camera camera;

	window.init_imgui();

	birb::overlay::performance perf_overlay(timestep);
	birb::overlay::camera_info cam_info(camera);

	birb::scene scene;

	birb::entity suzanne = scene.create_entity();

	birb::model suzanne_model("suzanne.obj");
	suzanne.add_component(suzanne_model);

	birb::shader shader("color");
	shader.set_vec4("color", { 0.2f, 0.3f, 0.4f, 1.0f });

	while (!window.should_close())
	{
		camera.process_input(window, timestep);

		glm::mat4 model = glm::mat4(1.0f);
		shader.set_mat4("model", model);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window.size().x) / static_cast<float>(window.size().y), 0.1f, 100.0f);
		shader.set_mat4("projection", projection);

		shader.set_mat4("view", camera.get_view_matrix());

		window.clear();

		shader.activate();

		// Render all models
		auto view = scene.get_registry().view<birb::model>();
		for (auto ent : view)
			view.get<birb::model>(ent).draw(shader);

		suzanne.get_component<birb::model>().draw(shader);

		perf_overlay.draw();
		cam_info.draw();

		window.flip();
		window.poll();
		timestep.step();
	}

	return 0;
}
