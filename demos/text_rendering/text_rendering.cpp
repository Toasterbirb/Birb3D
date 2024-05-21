#include "Camera.hpp"
#include "Entity.hpp"
#include "Font.hpp"
#include "FontManager.hpp"
#include "PerformanceOverlay.hpp"
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

	birb::overlay::performance performance_overlay(timestep);
	birb::overlay::renderer_overlay renderer_overlay(renderer);

	birb::font_manager font_manager;
	birb::font manaspace = font_manager.load_font("manaspc.ttf", 48);
	birb::font manaspace_small = font_manager.load_font("manaspc.ttf", 12);

	birb::entity text_big = scene.create_entity();
	birb::entity text_small = scene.create_entity();

	birb::text text("Hello world!\nSecond line of text\nSome more text", manaspace, { 25.0f, 128.0f, 0.0f }, 1.0f, 0x0295f7);
	text_big.add_component(text);

	birb::text text2("Iste distinctio illum cupiditate soluta aut voluptas sit odio. Ut est vitae rerum velit praesentium sunt.\nVoluptatem qui nihil nesciunt voluptas mollitia repellendus.\nEst repellendus veritatis assumenda repudiandae vel at dolores.\nOccaecati ex laudantium recusandae hic dolor voluptas reiciendis nisi.\nEt qui quod et. Et aperiam dolor qui alias. In repellendus enim optio magni consequatur quisquam.\nAliquam explicabo expedita dolores.\nMagnam minima molestiae sint saepe inventore fuga ullam voluptates.\nIllum libero est impedit molestiae aperiam vitae. Corrupti velit nulla quisquam officiis fugit aspernatur. Exercitationem dolor pariatur sint sit aut quas molestiae explicabo.\nQuia animi sit assumenda laudantium est. Ad provident voluptates quia laborum voluptatibus laudantium fuga dolores.\nSunt voluptatum temporibus fuga aliquam dignissimos totam ad.\nAut voluptas reprehenderit a est debitis voluptas voluptatum. Ipsam aliquid incidunt perferendis.\nVoluptatem sed ex quae doloremque illo cupiditate nam. Vel tenetur sint eum deserunt vel provident nostrum eos.\nVel optio laudantium ad praesentium adipisci laboriosam. Illo est ut et.\nIusto illum iste sed et alias voluptatem est. Tempora voluptatem pariatur accusamus occaecati dolorem nihil.\nVelit alias facere aspernatur omnis et optio. Iste distinctio illum cupiditate soluta aut voluptas sit odio.\nUt est vitae rerum velit praesentium sunt.\nVoluptatem qui nihil nesciunt voluptas mollitia repellendus.\nEst repellendus veritatis assumenda repudiandae vel at dolores.\nOccaecati ex laudantium recusandae hic dolor voluptas reiciendis nisi.\nEt qui quod et. Et aperiam dolor qui alias. In repellendus enim optio magni consequatur quisquam.\nAliquam explicabo expedita dolores.\nMagnam minima molestiae sint saepe inventore fuga ullam voluptates.\nIllum libero est impedit molestiae aperiam vitae. Corrupti velit nulla quisquam officiis fugit aspernatur.\nExercitationem dolor pariatur sint sit aut quas molestiae explicabo.\nQuia animi sit assumenda laudantium est. Ad provident voluptates quia laborum voluptatibus laudantium fuga dolores.\nSunt voluptatum temporibus fuga aliquam dignissimos totam ad.\nAut voluptas reprehenderit a est debitis voluptas voluptatum. Ipsam aliquid incidunt perferendis.\nVoluptatem sed ex quae doloremque illo cupiditate nam. Vel tenetur sint eum deserunt vel provident nostrum eos.\nVel optio laudantium ad praesentium adipisci laboriosam. Illo est ut et.\nIusto illum iste sed et alias voluptatem est. Tempora voluptatem pariatur accusamus occaecati dolorem nihil.\nVelit alias facere aspernatur omnis et optio. Iste distinctio illum cupiditate soluta aut voluptas sit odio. Ut est vitae rerum velit praesentium sunt.\nVoluptatem qui nihil nesciunt voluptas mollitia repellendus.\nEst repellendus veritatis assumenda repudiandae vel at dolores.\nOccaecati ex laudantium recusandae hic dolor voluptas reiciendis nisi.\nEt qui quod et. Et aperiam dolor qui alias. In repellendus enim optio magni consequatur quisquam.\nAliquam explicabo expedita dolores.\nMagnam minima molestiae sint saepe inventore fuga ullam voluptates.\nIllum libero est impedit molestiae aperiam vitae. Corrupti velit nulla quisquam officiis fugit aspernatur. Exercitationem dolor pariatur sint sit aut quas molestiae explicabo.\nQuia animi sit assumenda laudantium est. Ad provident voluptates quia laborum voluptatibus laudantium fuga dolores.\nSunt voluptatum temporibus fuga aliquam dignissimos totam ad.\nAut voluptas reprehenderit a est debitis voluptas voluptatum. Ipsam aliquid incidunt perferendis.\nVoluptatem sed ex quae doloremque illo cupiditate nam. Vel tenetur sint eum deserunt vel provident nostrum eos.\nVel optio laudantium ad praesentium adipisci laboriosam. Illo est ut et.\nIusto illum iste sed et alias voluptatem est. Tempora voluptatem pariatur accusamus occaecati dolorem nihil.\nVelit alias facere aspernatur omnis et optio. Iste distinctio illum cupiditate soluta aut voluptas sit odio.\nUt est vitae rerum velit praesentium sunt.\nVoluptatem qui nihil nesciunt voluptas mollitia repellendus.\nEst repellendus veritatis assumenda repudiandae vel at dolores.\nOccaecati ex laudantium recusandae hic dolor voluptas reiciendis nisi.\nEt qui quod et. Et aperiam dolor qui alias. In repellendus enim optio magni consequatur quisquam.\nAliquam explicabo expedita dolores.\nMagnam minima molestiae sint saepe inventore fuga ullam voluptates.\nIllum libero est impedit molestiae aperiam vitae. Corrupti velit nulla quisquam officiis fugit aspernatur.\nExercitationem dolor pariatur sint sit aut quas molestiae explicabo.\nQuia animi sit assumenda laudantium est. Ad provident voluptates quia laborum voluptatibus laudantium fuga dolores.\nSunt voluptatum temporibus fuga aliquam dignissimos totam ad.\nAut voluptas reprehenderit a est debitis voluptas voluptatum. Ipsam aliquid incidunt perferendis.\nVoluptatem sed ex quae doloremque illo cupiditate nam. Vel tenetur sint eum deserunt vel provident nostrum eos.\nVel optio laudantium ad praesentium adipisci laboriosam. Illo est ut et.\nIusto illum iste sed et alias voluptatem est. Tempora voluptatem pariatur accusamus occaecati dolorem nihil.\nVelit alias facere aspernatur omnis et optio.", manaspace_small, birb::vec3<f32>(320.0f, window.size().y + 256, 0));
	text_small.add_component(text2);

	// glm::mat4 projection_matrix = glm::ortho(0.0f, static_cast<float>(window.size().x), 0.0f, static_cast<float>(window.size().y));
	u64 frame_counter = 0;

	while (!window.should_close())
	{
		camera.process_input(window, timestep);

		text_big.get_component<birb::text>().set_text("Frame: " + std::to_string(frame_counter++) + "\nDeltatime: " + std::to_string(timestep.deltatime()));

		window.clear();

		// Draw here
		renderer.draw_entities(camera, window.size());

		performance_overlay.draw();
		renderer_overlay.draw();

		window.flip();

		window.poll();
		timestep.step();
	}
}
