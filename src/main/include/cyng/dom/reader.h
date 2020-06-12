/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_DOM_READER_H
#define CYNG_DOM_READER_H

#include <cyng/object.h>
#include <cstdint>
#include <cyng/dom/algorithm.h>

namespace cyng 
{
	/**
	 * Define an abstract base class to access the dom 
	 */
	class dom_access
	{
	public:
		/**
		 * @return true if current element is a leaf node and contains
		 * without childrens.
		 */
		virtual bool is_leaf() const = 0;
		
		/**
		 * Simple element access by name.
		 * Works for paramater maps and searchs in tuples, vectors 
		 * and sets for an parameter with the specified name.
		 */
		virtual object get(std::string const& name) const = 0;
		
		/**
		 * Simple element access by index
		 * Works for attribute maps and vectors. Searchs in tuples, vectors 
		 * and sets for an attribute with the specified index.
		 * Note: The search for attributes has a higher precedence in vectors 
		 * thean access by index.
		 */
		virtual object get(std::size_t idx) const = 0;

		/**
		 * @return true if specified element exsists
		 */
		virtual bool exists(std::string const& name) const = 0;
		//virtual bool exists(std::size_t idx) const = 0;

	};
	
	
	template <typename T>
	class reader : public dom_access
	{
	public:
		reader(T const& container)
		: container_(container)
		{}

		reader(reader const& other)
		: container_(other.container_)
		{}

		/**
		 * @return true
		 */
		virtual bool is_leaf() const
		{
			return true;
		}
		
		/**
		 * Simple element access by name.
		 * Works for paramater maps and searchs in tuples, vectors 
		 * and sets for an parameter with the specified name.
		 */
		virtual object get(std::string const& name) const override
		{
			return find(container_, name);
		}
		
		/**
		 * Simple element access by index
		 * Works for attribute maps and vectors. Searchs in tuples, vectors 
		 * and sets for an attribute with the specified index.
		 * Note: The search for attributes has a higher precedence in vectors 
		 * thean access by index.
		 */
		virtual object get(std::size_t idx) const override
		{
			return find(container_, idx);
		}

		/**
		 * @return true if specified entry exists
		 */
		virtual bool exists(std::string const& name) const override
		{
			return cyng::exists(container_, name);
		}
		
		/**
		 * @return size of the container 
		 */
		std::size_t size() const
		{
			return container_.size();
		}
		
	protected:
		T const& container_;
	};
	
	template <>
	class reader<object> : public dom_access
	{
	public:
		/**
		 * working with an empty data set 
		 */
		reader();
		reader(object const&);
		
		/**
		 * @return true if current element is a leaf node and contains
		 * without childrens.
		 */
		virtual bool is_leaf() const override;
		
		/**
		 * Simple element access by name.
		 * Works for paramater maps and searchs in tuples, vectors 
		 * and sets for an parameter with the specified name.
		 */
		virtual object get(std::string const& name) const override;
		
		/**
		 * Simple element access by index
		 * Works for attribute maps and vectors. Searchs in tuples, vectors 
		 * and sets for an attribute with the specified index.
		 * Note: The search for attributes has a higher precedence in vectors 
		 * thean access by index.
		 */
		virtual object get(std::size_t idx) const override;

		/**
		 * @return true if specified entry exists
		 */
		virtual bool exists(std::string const& name) const override;

		/**
		 * Nested array subscript operator
		 */
		reader operator[](std::string const& name) const;

		/**
		 * Nested array subscript operator
		 */
		reader operator[](std::size_t idx) const;
		
	private:
		const object root_;
	};
		
	/**
	* @brief reader class for tuples
	*/
	class tuple_reader : public reader<tuple_t>
	{
	public:
		tuple_reader(tuple_t const&);
		
		/**
		 * @return true
		 */
		virtual bool is_leaf() const override;

		/**
		 * Nested array subscript operator
		 */
		reader<object> operator[](std::string const& name) const;

		/**
		 * Nested array subscript operator
		 */
		reader<object> operator[](std::size_t idx) const;

	};

	/**
	 * @brief reader class for vectors
	 */
	class vector_reader : public reader<vector_t>
	{
	public:
		vector_reader(vector_t const&);
		
		/**
		 * @return true
		 */
		virtual bool is_leaf() const override;
		
		/**
		 * Nested array subscript operator
		 */
		reader<object> operator[](std::size_t idx) const;
		
	};

	/**
	 * @brief reader class for sets
	 */
	class set_reader : public reader<set_t>
	{
	public:
		set_reader(set_t const&);
		
		/**
		 * @return true
		 */
		virtual bool is_leaf() const override;
	};
	
	/**
	 * @brief reader class for attr_map_t
	 */
	class attr_map_reader : public reader<attr_map_t>
	{
	public:
		attr_map_reader(attr_map_t const&);
		
		/**
		 * @return true
		 */
		virtual bool is_leaf() const override;
	};

	/**
	 * @brief reader class for param_map_t
	 */
	class param_map_reader : public reader<param_map_t>
	{
	public:
		param_map_reader(param_map_t const&);
		
		/**
		 * @return true
		 */
		virtual bool is_leaf() const override;

		/**
		 * Nested array subscript operator
		 */
		reader<object> operator[](std::string const& name) const;

	};

	/**
	 * get the reader type for the specified input
	 */
	template < typename T >
	struct select_reader
	{ /* no reader specified */ };

	template <>
	struct select_reader< tuple_t >
	{
		using type = tuple_reader;
	};

	template <>
	struct select_reader< vector_t >
	{
		using type = vector_reader;
	};

	template <>
	struct select_reader< set_t >
	{
		using type = set_reader;
	};
	
	template <>
	struct select_reader< object >
	{
		using type = reader<object>;
	};

	template <>
	struct select_reader< attr_map_t >
	{
		using type = attr_map_reader;
	};

	template <>
	struct select_reader< param_map_t >
	{
		using type = param_map_reader;
	};

	/**
	 * helper function to select the reader type
	 * automatically.
	 */
	template < typename T >
	auto make_reader(T const& c) -> typename select_reader< T >::type
	{
		using reader_t = typename select_reader< T >::type;
		return reader_t(c);
	}

}

#endif 	// CYNG_DOM_READER_H


