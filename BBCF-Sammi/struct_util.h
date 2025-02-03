#pragma once

#include <type_traits>

template <typename T>
using array_elem_type = std::remove_reference_t<decltype(std::declval<T>()[0])>;

template <typename T>
constexpr auto array_elem_count = sizeof(std::declval<T>()) / sizeof(std::declval<T>()[0]);

#define FIELD(OFFSET, TYPE, NAME)                                  \
	std::add_lvalue_reference_t<TYPE> get_##OFFSET() const         \
	{                                                              \
		return *(std::add_pointer_t<TYPE>)((char *)this + OFFSET); \
	}                                                              \
	__declspec(property(get = get_##OFFSET)) TYPE NAME

#define ARRAY_FIELD(OFFSET, TYPE, NAME)                                              \
	std::add_lvalue_reference_t<array_elem_type<TYPE>> get_##OFFSET(int index) const \
	{                                                                                \
		return ((std::decay_t<TYPE>)((char *)this + OFFSET))[index];                 \
	}                                                                                \
	__declspec(property(get = get_##OFFSET)) array_elem_type<TYPE> NAME[array_elem_count<TYPE>]

#define BIT_FIELD(OFFSET, MASK, NAME)                         \
	bool get_##OFFSET_##MASK() const                          \
	{                                                         \
		return (*(int *)((char *)this + OFFSET) & MASK) != 0; \
	}                                                         \
	__declspec(property(get = get_##OFFSET_##MASK)) bool NAME
