#pragma once

#include "EditorComponent.hpp"

#include <string>

namespace birb
{
	namespace component
	{
		class info : public editor_component
		{
		public:
			info() : name("untitled") {}
			explicit info(const std::string& name) : name(name) {}
			std::string name;

			void draw_editor_ui() override;

			template<class Archive>
			void serialize(Archive& ar)
			{
				ar(name);
			}

		private:
		};
	}
}
