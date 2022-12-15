#pragma once

#include "Components/Component.hpp"
#include <vector>

namespace voyage
{
	class FRAMEWORK_API GameObject
	{
	public:
		GameObject();
		

	private:
		std::vector<Component*> _components;
	};
}