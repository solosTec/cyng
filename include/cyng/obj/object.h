/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_OBJ_OBJECT_H
#define CYNG_OBJ_OBJECT_H

#include <cyng/obj/type_interface.h>
#include <cyng/obj/tracker.h>

#include <memory>
#include <utility>
// #include <compare>	//	requires gcc 10.x

namespace cyng {

	class obj_interface;
	class object
	{
		friend void swap(object&, object&);
		template <typename T>
		friend const T* object_cast(object const&) noexcept;
		template < typename U >
		friend auto object_release(object& obj) noexcept -> std::pair<U*, obj_interface*>;
		template <typename T, typename... Args>
		friend object make_object(Args&&... args);
		template <typename T>
		friend object make_object(T&& v);

		/**
		 * internal representation as unique pointer
		 */
		using ptr_t = std::unique_ptr<obj_interface, tracker>;

	public:
		/**
		 * To use an object in a STL container class
		 * a default constructor is required (std::vector is an exception).
		 * Otherwise the use of the empty factory() method
		 * is highly recommended.
		 */
		object();

		/**
		 * Move-constructs an object from obj. After the construction, *this contains
		 * a copy of the previous state of obj and obj is empty.
		 */
		object(object&& obj) noexcept;

		/**
		 * Copy-semantics
		 * Create a null object if obj is empty
		 */
		object(object const& obj);

		/**
		 * Allows safe inheritence without UB.
		 */
		virtual ~object();

		/**
		 * @return hash code of the hidden value.
		 */
		std::size_t hash() const noexcept;

		/**
		 * @return copy of this object
		 */
		[[nodiscard]]
		object clone() const;

		/**
		 * @return type info
		 */
		type_interface const& rtti() const;

		/**
		 * transfer ownership
		 */
		object& operator=(object&& obj) noexcept;

		/**
		 * copy
		 */
		 object& operator=(object const& obj);

		/**
		 * Test if internal pointer is a nullptr
		 */
		explicit operator bool() const noexcept;

		/**
		 * ordering
		 */
		// auto operator<=>(const object&) const = default;

	private:
		/**
		 * takes ownership
		 */
		object(obj_interface* ip);
		object(ptr_t p);

	private:
		/**
		 * internal data pointer (never nullptr)
		 */
		ptr_t obj_;

	public:
		static tracker tracker_;
	};

	/**
	 *  Swaps the values a and b. 
	 */
	void swap(object& a, object& b);

	/**
	 * Compare type signature of both objects.
	 * 
	 * @return true if both objects of same type;
	 */
	bool is_same_type(object const& a, object const& b);

	template <typename T>
	bool is_same_type(object const& obj) {
		return obj.rtti().rtti() == typeid(T);
	}

}

#include <functional>

namespace std {

	template <>
	class hash<cyng::object> {
	public:
		size_t operator()(cyng::object const& obj) const;
	};
}

#endif
