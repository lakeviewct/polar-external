#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "../utils/memory/memory.h"
#include "offsets.h"

// i copied this from rosehack 
// spoiler: not fucking writing an sdk from scratch
// it is modified tho so dw
namespace rbx
{
	struct instance;
	struct primitive;
	struct model_instance;
	struct player;

	struct addressable
	{
		std::uint64_t address;

		addressable() : address(0) {}
		addressable(std::uint64_t address) : address(address) {}
	};

	struct nameable : public addressable
	{
		using addressable::addressable;

		std::string get_name();
		std::string get_class_name();
	};

	struct rbx_interface
	{
		template <typename T>
		std::vector<T> get_children();

		std::vector<rbx::instance> get_children();
		rbx::instance find_first_child(std::string_view str);
		rbx::instance find_first_child_by_class(std::string_view str);
	};

	struct instance : public nameable, public rbx_interface
	{
		using nameable::nameable;
		rbx::instance get_team();
		rbx::player get_local_player();
	};

	struct player final : public instance
	{
		using instance::instance;

		rbx::model_instance get_model_instance();
	};

	struct model_instance final : public instance
	{
		using instance::instance;
	};

	struct humanoid final : public addressable
	{
		using addressable::addressable;


		float get_health();
		std::uint8_t get_rig_type();
	};

	struct part : public instance
	{
		using instance::instance;

		rbx::primitive get_primitive();
	};

	struct primitive final : public addressable
	{
		using addressable::addressable;

		glm::vec3 get_size();
		glm::vec3 get_position();
		glm::mat3 get_rotation();
	};

	struct visualengine final : public addressable
	{
		glm::vec2 get_dimensions();
		glm::mat4 get_viewmatrix();
		bool world_to_screen(glm::vec3& world, glm::vec2& out, glm::vec2& dims, glm::mat4& view);
	};
}

template <typename T>
std::vector<T> rbx::rbx_interface::get_children()
{
	rbx::instance* base = static_cast<rbx::instance*>(this);

	std::uint64_t start = memory->read<std::uint64_t>(base->address + Offsets::Instance::ChildrenStart);
	std::uint64_t end = memory->read<std::uint64_t>(start + Offsets::Instance::ChildrenEnd);

	std::vector<T> children;
	children.reserve(32);

	for (std::uint64_t instance = memory->read<std::uint64_t>(start); instance != end; instance += sizeof(std::shared_ptr<void*>))
	{
		children.emplace_back(memory->read<std::uint64_t>(instance));
	}

	return children;
}