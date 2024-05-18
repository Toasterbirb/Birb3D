#pragma once

#include "EditorComponent.hpp"

#include <string>

namespace birb
{
	class info : public editor_component
	{
	public:
		info() : name("untitled") {}
		explicit info(const std::string& name) : name(name) {}
		std::string name;

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(name);
		}

	private:
		static inline const std::string editor_header_name = "NULL";
	};
}
