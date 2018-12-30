/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#ifndef CYNG_DOCSCRIPT_GENERATOR_H
#define CYNG_DOCSCRIPT_GENERATOR_H

#include <cyng/vm/controller.h>
#include <cyng/async/scheduler.h>
#include <cyng/dom/reader.h>

#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/filesystem.hpp>

namespace cyng	
{
	namespace docscript
	{
		/**
		 * helper to store structure information
		 */
		struct element
		{
			enum type {
				HEADER,
				FIGURE,
				TABLE,
				LISTING,
			};
			element(type, std::string const&, std::vector<std::size_t> const& chapter);
			std::string level() const;
			std::string to_str() const;
			std::size_t depth() const;

			//	assignment
			element& operator=(element const&) = delete;

			const type type_;
			const std::string text_;
			const std::vector<std::size_t>	chapter_;
		};

		//	comparison
		/*
		* Structure elements are first sorted by type and than lexicographically
		* by the chapter vector
		*/
		bool operator==(element const&, element const&);
		bool operator<(element const&, element const&);
		bool operator!=(element const&, element const&);
		bool operator>(element const&, element const&);
		bool operator<=(element const&, element const&);
		bool operator>=(element const&, element const&);

		using structure_t = std::map<boost::uuids::uuid, element>;
		using element_t = typename structure_t::value_type;

		class generator
		{
		public:
			generator(std::vector< boost::filesystem::path > const& inc, int verbose, bool body_only);
			virtual ~generator();

			/**
			 * produce the document
			 */
			void run(cyng::vector_t const&);

			/**
			 * generate an index file (JSON)
			 */
			//std::size_t index(boost::filesystem::path const& out) const;

			/**
			 * generate a meta file (JSON)
			 */
			std::size_t meta(boost::filesystem::path const& out) const;

		protected:
			boost::filesystem::path resolve_path(std::string const& s) const;

		private:
			void register_this();
			void update_meta(cyng::param_map_t const&);
			void generate(std::ostream&, cyng::vector_t::const_iterator begin, cyng::vector_t::const_iterator end);
			std::string generate_header(std::size_t level, std::string const& txt, boost::uuids::uuid tag);

			void fun_contents(context& ctx);
			void fun_header(context& ctx);
			void fun_header_1(context& ctx);
			void fun_header_2(context& ctx);
			void fun_header_3(context& ctx);
			void fun_header_4(context& ctx);
			void fun_paragraph(context& ctx);
			void fun_bold(context& ctx);
			void fun_emphasise(context& ctx);
			void fun_color(context& ctx);
			void fun_link(context& ctx);
			void fun_figure(context& ctx);
			void fun_quote(context& ctx);
			void fun_cite(context& ctx);

			void fun_list(context& ctx);
			void fun_item(context& ctx);

			void fun_generate(context& ctx);
			void fun_meta(context& ctx);
			void fun_title(context& ctx);


		private:
			const std::vector< boost::filesystem::path > includes_;
			const int verbosity_;
			const bool body_only_;

			boost::uuids::random_generator	uuid_gen_;	//	basic_random_generator<mt19937>
			boost::uuids::name_generator name_gen_;

			cyng::async::scheduler	scheduler_;
			controller vm_;

			param_map_t meta_;
			std::vector<std::size_t> numeration_;
			structure_t	structure_;
			std::string language_;	//!< target language
		};

		//
		//	free functions
		//
		std::string accumulate(cyng::vector_reader const& reader, std::size_t start, std::size_t end = 1);
		std::string accumulate(cyng::vector_reader const& reader
			, std::size_t start
			, std::size_t end
			, std::string tag);
		std::string get_extension(boost::filesystem::path const& p);

	}	//	docscript
}	//	cyng

#endif	//	CYNG_DOCSCRIPT_GENERATOR_H
