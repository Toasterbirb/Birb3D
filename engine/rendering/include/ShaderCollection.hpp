#pragma once

#include <memory>
#include <unordered_map>

namespace birb
{
	class shader;
	struct shader_ref;

	class shader_collection
	{
	public:
		~shader_collection();

		/**
		 * @brief Compile often used shaders
		 */
		static void precompile_basic_shaders();

		/**
		 * @brief Add a new custom shader to the shader collection
		 *
		 * This function will add the hashes of the vertex and fragment shader
		 * nanes to the shader collection name hash hashmaps
		 *
		 * The returned shader reference can then be used to fetch a reference
		 * to the actually compiled shader program, which will get compiled
		 * when it is fetched for the first time
		 *
		 * @return A shader reference to the registered shader
		 */
		static shader_ref register_shader(const std::string& vertex, const std::string& fragment);

		/**
		 * @brief Check if a custom shader has been registered already
		 */
		static bool is_shader_registered(const shader_ref& shader);

		/**
		 * @brief Get a reference to a shader built from the given shader source names
		 *
		 * If the shader hasn't been compiled yet when its requested for,
		 * it'll get compiled once and after that it can be re-used
		 */
		static std::shared_ptr<shader> get_shader(const shader_ref& ref);

		/**
		 * @brief Clear the shader collection
		 */
		static void wipe();

	private:
		/**
		 * @brief Compile a new shader and return a shared pointer to it
		 *
		 * This function will also cache the shader to the shader collection
		 */
		static std::shared_ptr<shader> compile_shader(const shader_ref& ref);

		static void hash_shader_source_names();

		// Have the builtin shaders been hashed yet
		static inline bool builtin_shaders_hashed = false;

		static inline std::unordered_map<u64, std::shared_ptr<shader>> shader_storage;
		static inline std::unordered_map<u64, std::string> vertex_shader_hashes;
		static inline std::unordered_map<u64, std::string> fragment_shader_hashes;
	};
}
