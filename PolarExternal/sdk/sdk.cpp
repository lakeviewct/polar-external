#include "sdk.h"
#include "offsets.h"
#include "../utils/memory/memory.h"
#include "../game/game.h"

#include <glm/glm.hpp>

std::string rbx::nameable::get_name()
{
	std::uint64_t name = memory->read<std::uint64_t>(this->address + Offsets::Instance::Name);

	if (name)
	{
		return memory->read_string(name);
	}

	return "unknown";
}

std::string rbx::nameable::get_class_name()
{
	std::uint64_t class_descriptor = memory->read<std::uint64_t>(this->address + Offsets::Instance::ClassDescriptor);
	std::uint64_t class_name = memory->read<std::uint64_t>(class_descriptor + Offsets::Instance::ClassName);

	if (class_name)
	{
		return memory->read_string(class_name);
	}

	return "unknown";
}

std::vector<rbx::instance> rbx::rbx_interface::get_children()
{
	rbx::instance* base = static_cast<rbx::instance*>(this);

	std::uint64_t start{ memory->read<std::uint64_t>(base->address + Offsets::Instance::ChildrenStart) };
	std::uint64_t end{ memory->read<std::uint64_t>(start + Offsets::Instance::ChildrenEnd) };

	std::vector<rbx::instance> children;
	children.reserve(32);

	for (std::uint64_t instance = memory->read<std::uint64_t>(start); instance != end; instance += sizeof(std::shared_ptr<void*>))
	{
		children.emplace_back(memory->read<std::uint64_t>(instance));
	}

	return children;
}

float rbx::humanoid::get_health()
{
	return memory->read<float>(this->address + Offsets::Humanoid::Health);
}

rbx::instance rbx::rbx_interface::find_first_child(std::string_view str)
{
	std::vector<rbx::instance> children = this->get_children();

	for (rbx::instance& child : children)
	{
		if (child.get_name() == str)
		{
			return child;
		}
	}

	return {};
}

rbx::player rbx::instance::get_local_player()
{
	return memory->read<uint64_t>(game::players.address + Offsets::Player::LocalPlayer);
}

rbx::instance rbx::rbx_interface::find_first_child_by_class(std::string_view str)
{
	std::vector<rbx::instance> children = this->get_children();

	for (rbx::instance& child : children)
	{
		if (child.get_class_name() == str)
		{
			return child;
		}
	}

	return {};
}

rbx::instance rbx::instance::get_team()
{
	return memory->read<std::uint64_t>(this->address + Offsets::Player::Team);
}

rbx::model_instance rbx::player::get_model_instance()
{
	return memory->read<std::uint64_t>(this->address + Offsets::Player::ModelInstance);
}

std::uint8_t rbx::humanoid::get_rig_type()
{
	return memory->read<std::uint8_t>(this->address + Offsets::Humanoid::RigType);
}

rbx::primitive rbx::part::get_primitive()
{
	return memory->read<std::uint64_t>(this->address + Offsets::BasePart::Primitive);
}

glm::vec3 rbx::primitive::get_size()
{
	return memory->read<glm::vec3>(this->address + Offsets::Primitive::Size);
}

glm::vec3 rbx::primitive::get_position()
{
	return memory->read<glm::vec3>(this->address + Offsets::Primitive::Position);
}

glm::mat3 rbx::primitive::get_rotation()
{
	return glm::transpose(memory->read<glm::mat3>(this->address + Offsets::Primitive::Rotation));
}

glm::vec2 rbx::visualengine::get_dimensions()
{
	return memory->read<glm::vec2>(this->address + Offsets::VisualEngine::Dimensions);
}

glm::mat4 rbx::visualengine::get_viewmatrix()
{
	return glm::transpose(memory->read<glm::mat4>(this->address + Offsets::VisualEngine::ViewMatrix));
}

bool rbx::visualengine::world_to_screen(glm::vec3& world, glm::vec2& out, glm::vec2& dims, glm::mat4& view)
{
	// world but a vec4
	glm::vec4 world_4 = { world.x, world.y , world.z, 1.0f };
	glm::vec4 clip = view * world_4;

	if (clip.w < 0.1f)
	{
		return false;
	}

	clip.x /= clip.w;
	clip.y /= clip.w;

	out.x = (dims.x * 0.5f * clip.x) + (dims.x * 0.5f);
	out.y = -(dims.y * 0.5f * clip.y) + (dims.y * 0.5f);

	return true;
}