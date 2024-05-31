#include "UIBase.hpp"
#include "UUID.hpp"

namespace birb
{
	namespace ui
	{
		ui_base::ui_base()
		:uuid(uuid::generate())
		{}
	}
}
