/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/docscript/generator.h>
#include <cyng/vm/generator.h>
#include <cyng/factory.h>
#include <cyng/value_cast.hpp>
#include <cyng/crypto/base64.h>
#include <cyng/io/serializer.h>
#include <cyng/dom/reader.h>
#include <cyng/docscript/filter/verbatim.h>
#include <cyng/docscript/filter/cpp.h>
#include <cyng/json.h>

#include <iostream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cyng	
{
	namespace docscript
	{

		generator::generator(std::vector< boost::filesystem::path > const& incs, int verbosity, bool body_only)
			: includes_(incs)
			, verbosity_(verbosity)
			, body_only_(body_only)
			, uuid_gen_()
			, name_gen_(uuid_gen_())
			, scheduler_()
			, vm_(scheduler_.get_io_service(), uuid_gen_(), std::cout, std::cerr)
			, meta_()
			, numeration_()
			, structure_()
			, language_("en")
		{
			register_this();
		}

		generator::~generator()
		{
			scheduler_.stop();
		}

		void generator::register_this()
		{
            vm_.register_function("now", 1, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				std::cout
					<< "\n***info: now("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl
					;
#endif
				//
				//	produce result value
				//
				ctx.push(cyng::make_now());
            });

			//
			//	4 parameters required. If less function call is empty and therefore useless
			//	
			vm_.register_function("meta", 4, std::bind(&generator::fun_meta, this, std::placeholders::_1));

            vm_.register_function("debug", 3, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				//std::cout
				//	<< "\n***info: debug("
				//	<< cyng::io::to_literal(frame)
				//	<< ")"
				//	<< std::endl;

#endif
				const std::string v = value_cast<std::string>(frame.at(2), "");
// 				const std::string v = cyng::string_cast(frame.at(2));
// 				const cyng::code op = boost::algorithm::iequals(v, "on")
// 					? cyng::code::DBG_ON
// 					: cyng::code::DBG_OFF
// 					;

				if (verbosity_ > 1)
				{
					std::cout
						<< "***info: debug("
						<< std::boolalpha
						<< boost::algorithm::iequals(v, "on")
						<< ")"
						<< std::endl;
				}

				//
				// function meta has no return values
				//

            });

			vm_.register_function("title", 4, std::bind(&generator::fun_title, this, std::placeholders::_1));
			vm_.register_function("contents", 2, std::bind(&generator::fun_contents, this, std::placeholders::_1));

			vm_.register_function("header", 3, std::bind(&generator::fun_header, this, std::placeholders::_1));
			vm_.register_function("header.1", 3, std::bind(&generator::fun_header_1, this, std::placeholders::_1));
			vm_.register_function("header.2", 3, std::bind(&generator::fun_header_2, this, std::placeholders::_1));
			vm_.register_function("header.3", 3, std::bind(&generator::fun_header_3, this, std::placeholders::_1));
			vm_.register_function("header.4", 3, std::bind(&generator::fun_header_4, this, std::placeholders::_1));
			vm_.register_function("paragraph", 4, std::bind(&generator::fun_paragraph, this, std::placeholders::_1));
			vm_.register_function("bold", 3, std::bind(&generator::fun_bold, this, std::placeholders::_1));
			vm_.register_function("emphasise", 3, std::bind(&generator::fun_emphasise, this, std::placeholders::_1));
			vm_.register_function("color", 4, std::bind(&generator::fun_color, this, std::placeholders::_1));
			vm_.register_function("link", 4, std::bind(&generator::fun_link, this, std::placeholders::_1));
			vm_.register_function("figure", 4, std::bind(&generator::fun_figure, this, std::placeholders::_1));
			vm_.register_function("quote", 4, std::bind(&generator::fun_quote, this, std::placeholders::_1));
			vm_.register_function("cite", 4, std::bind(&generator::fun_cite, this, std::placeholders::_1));

			vm_.register_function("list", 4, std::bind(&generator::fun_list, this, std::placeholders::_1));
			vm_.register_function("item", 3, std::bind(&generator::fun_item, this, std::placeholders::_1));

			vm_.register_function("env.open", 4, std::bind(&generator::fun_env, this, std::placeholders::_1));

			vm_.register_function("generate.file", 1, std::bind(&generator::fun_generate_file, this, std::placeholders::_1));
			vm_.register_function("generate.meta", 1, std::bind(&generator::fun_generate_meta, this, std::placeholders::_1));
		}

		void generator::fun_contents(context& ctx)
		{
			//	[1idx,true,%(("depth":"4"))]
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

			std::cout
				<< "\n***info: contents("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;

#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type

			//
			//	ToDo: use structure_ to generate a content list
			//
		}

		void generator::fun_header(context& ctx)
		{
			//	[0001,1,true,%(("level":1),("tag":79bf3ba0-2362-4ea5-bcb5-ed93844ac59a),("title":Second Header))]
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

			std::cout
				<< "\n***info: header("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;

#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (cyng::value_cast(reader.get(2), false)) {

				auto const level = value_cast<std::size_t>(reader[3].get("level"), 0);
				auto const txt = value_cast<std::string>(reader[3].get("title"), "NO TITLE");
				auto const stag = value_cast<std::string>(reader[3].get("tag"), boost::uuids::to_string(uuid_gen_()));
				auto const tag = name_gen_(stag);

				const std::string node = generate_header(level, txt, tag);
				ctx.push(cyng::make_object(node));
			}
			else {

				std::cerr
					<< "***error: function header() only accepts named parameters"
					<< std::endl;

				ctx.push(cyng::make_object("function header() only accepts named parameters"));

			}
		}

		void generator::fun_header_1(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0001,1,false,2018-12-31 18:28:49.91697640]
			//	
			//	* function type
			//	* recursion depth
			//	* parameters/arguments
			//	* values
			//
			std::cout
				<< "\n***info: header.1("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (cyng::value_cast(reader.get(2), false)) {

				//	named parameters
				//BOOST_ASSERT_MSG(false, "header.1(params) not implemented yet");
				ctx.push(cyng::make_object("header.1(params) not implemented yet"));

			}
			else {

				//	argument list
				auto const txt = accumulate(reader, 3, frame.size());
				auto const node = generate_header(1, txt, uuid_gen_());
				ctx.push(cyng::make_object(node));
			}
		}

		void generator::fun_header_2(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0001,1,false,Examples,of,DocScript]
			//	[0001,1,true,%(("tag":b3fc0bdc-5d95-45e9-b66c-f0c541f1592c),("title":Basic Rules))]
			std::cout
				<< "\n***info: header.2("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (cyng::value_cast(reader.get(2), false)) {

				//	named parameters
				auto const txt = value_cast<std::string>(reader[3].get("title"), "NO TITLE");
				auto const stag = value_cast<std::string>(reader[3].get("tag"), boost::uuids::to_string(uuid_gen_()));
				auto const tag = name_gen_(stag);

				const std::string node = generate_header(2, txt, tag);
				ctx.push(cyng::make_object(node));

			}
			else {

				//	argument list
				auto const txt = accumulate(reader, 3, frame.size());
				auto const node = generate_header(2, txt, uuid_gen_());
				ctx.push(cyng::make_object(node));
			}
		}

		void generator::fun_header_3(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			std::cout
				<< "\n***info: header.3("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (cyng::value_cast(reader.get(2), false)) {

				//	named parameters
				ctx.push(cyng::make_object("header.3(params) not implemented yet"));

			}
			else {
				auto const txt = accumulate(reader, 3, frame.size());
				auto const node = generate_header(3, txt, uuid_gen_());
				ctx.push(cyng::make_object(node));
			}
		}

		void generator::fun_header_4(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			std::cout
				<< "\n***info: header.3("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (cyng::value_cast(reader.get(2), false)) {

				//	named parameters
				ctx.push(cyng::make_object("header.4(params) not implemented yet"));

			}
			else {
				auto const txt = accumulate(reader, 3, frame.size());
				auto const node = generate_header(4, txt, uuid_gen_());
				ctx.push(cyng::make_object(node));
			}
		}

		void generator::fun_paragraph(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0001,3,false,16,Science,may,never,come,up,with,a,better,office,communication,system,,than,the,coffee,break,.]
			//
			//	* function type (0 ... 3)
			//	* recursion depth
			//	* parameters/arguments
			//	* chunk count
			//	* text chunks
			//
			std::cout
				<< "\n***info: paragraph("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (!cyng::value_cast(reader.get(2), false)) {

				auto const size = value_cast<std::size_t>(reader.get(3), 0u);
				BOOST_ASSERT(size == frame.size() - 4);
				auto const node = accumulate(reader, 4, frame.size(), "p");
				ctx.push(cyng::make_object(node));

			}
			else {

				ctx.push(cyng::make_object("error: paragraph only accepts arguments"));

			}
		}

		void generator::fun_bold(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0002,0,false,dolor]
			std::cout
				<< "\n***info: bold("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (!cyng::value_cast(reader.get(2), false)) {

				const std::string node = accumulate(reader, 3, frame.size(), "b");

				if (verbosity_ > 3)
				{
					std::cout
						<< "***info: bold("
						<< node
						<< ")"
						<< std::endl;
				}

				ctx.push(cyng::make_object(node));
			}
			else {

				std::cerr
					<< "***error: function bold() only accepts argument list"
					<< std::endl;

				ctx.push(cyng::make_object("function bold() only accepts argument list"));
			}
		}

		void generator::fun_emphasise(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();

#ifdef _DEBUG
			//	[0002,0,false,sed,diam,nonumy,eirmod,tempor,invidunt,ut,labore]
			std::cout
				<< "\n***info: emphasise("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (!cyng::value_cast(reader.get(2), false)) {

				const std::string node = accumulate(reader, 3, frame.size(), "em");

				if (verbosity_ > 3)
				{
					std::cout
						<< "***info: emphasise("
						<< node
						<< ")"
						<< std::endl;
				}

				ctx.push(cyng::make_object(node));
			}
			else {

				std::cerr
					<< "***error: function emphasise() only accepts argument list"
					<< std::endl;

				ctx.push(cyng::make_object("function emphasise() only accepts argument list"));
			}
		}

		void generator::fun_color(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

			//	[0002,0,true,%(("red":amet))]
			std::cout
				<< "\n***info: color("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;

#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (cyng::value_cast(reader.get(2), false)) {

				const auto map = value_cast(reader.get(3), param_map_t());
				BOOST_ASSERT_MSG(map.size() == 1, "internal error (color)");

				std::stringstream ss;
				ss
					<< "<span style=\"color:"
					<< map.begin()->first
					<< "\">"
					<< cyng::io::to_str(map.begin()->second)
					<< "</span>"
					;
				const std::string node = ss.str();
				if (verbosity_ > 3)
				{
					std::cout
						<< "***info: color("
						<< node
						<< ")"
						<< std::endl;
				}

				ctx.push(cyng::make_object(node));
			}
			else {

				std::cerr
					<< "***error: function color() only accepts named parameters"
					<< std::endl;

				ctx.push(cyng::make_object("function color() only accepts named parameters"));
			}
		}

		void generator::fun_link(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0000,0,true,%(("text":LaTeX),("url":https://www.latex-project.org/))]
			std::cout
				<< "\n***info: link("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (cyng::value_cast(reader.get(2), false)) {

				const std::string url = value_cast<std::string>(reader[3].get("url"), "");
				const std::string node = "<a href=\""
					+ url
					+ "\" title=\""
					+ value_cast<std::string>(reader[3].get("title"), url)
					+ "\">"
					+ value_cast<std::string>(reader[3].get("text"), "")
					+ "</a>"
					;

				ctx.push(cyng::make_object(node));
			}
			else {

				std::cerr
					<< "***error: function link() only accepts named parameters"
					<< std::endl;

				ctx.push(cyng::make_object("function link() only accepts named parameters"));

			}
		}

		//<figure>
		//  <img src="img_pulpit.jpg" alt="The Pulpit Rock" width="304" height="228">
		//  <figcaption>Fig1. - A view of the pulpit rock in Norway.</figcaption>
		//</figure>
		void generator::fun_figure(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0001,1,true,%(("alt":Giovanni Bellini, Man wearing a turban),("caption":Giovanni Bellini, Man wearing a turban),("source":LogoSmall.jpg),("tag":0c65390a-9405-43d0-b504-3e22d8c267a0))]
			std::cout
				<< "\n***info: figure("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif

			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (cyng::value_cast(reader.get(2), false)) {

				const std::string source = value_cast<std::string>(reader[3].get("source"), "");
				const boost::filesystem::path p = resolve_path(source);
				std::ifstream file(p.string(), std::ios::binary | std::ios::ate);
				if (!file.is_open())
				{
					std::cerr
						<< "***error cannot open figure file ["
						<< source
						<< "]"
						<< std::endl;
					ctx.push(cyng::make_object("cannot open file [" + source + "]"));
				}
				else
				{
					//
					//	do not skip 
					//
					file.unsetf(std::ios::skipws);

					//
					//	get file size
					//
					std::streamsize size = file.tellg();
					file.seekg(0, std::ios::beg);

					//
					//	read into buffer
					//
					cyng::buffer_t buffer(size);
					file.read(buffer.data(), size);
					BOOST_ASSERT(file.gcount() == size);

					//
					//	encode image as base 64
					//
					std::string const alt = value_cast<std::string>(reader[3].get("alt"), "");
					std::string const cap = value_cast<std::string>(reader[3].get("caption"), "");
					auto const tag = value_cast(reader[3].get("tag"), uuid_gen_());

					auto pos = structure_.emplace(std::piecewise_construct,
						std::forward_as_tuple(tag),
						std::forward_as_tuple(element::FIGURE, cap, numeration_));

					if (pos.second)
					{

						std::stringstream ss;
						ss
							<< std::endl
							<< "<figure id=\""
							<< tag
							<< "\">"
							<< std::endl
							<< "<img alt=\""
							<< alt
							<< "\" src=\"data:image/"
							<< get_extension(p)
							<< ";base64,"
							<< cyng::crypto::base64_encode(buffer.data(), buffer.size())
							<< "\" />"
							<< std::endl
							<< "<figcaption>figure "
							<< pos.first->second.to_str()
							<< "</figcaption>"
							<< std::endl
							<< "</figure>"
							;

						const std::string node = ss.str();
						if (verbosity_ > 1)
						{
							std::cout
								<< "***info: figure("
								<< cap
								<< " - "
								<< size
								<< " bytes)"
								<< std::endl;
						}

						ctx.push(cyng::make_object(node));
					}
					else {
						std::cerr
							<< "***error: cannot insert figure "
							<< source
							<< std::endl;

						ctx.push(cyng::make_object("cannot insert figure " + source));

					}
				}
			}
			else {
				std::cerr
					<< "***error: function figure() only accepts named parameters"
					<< std::endl;

				ctx.push(cyng::make_object("function figure() only accepts named parameters"));

			}
		}

		void generator::fun_quote(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0004,1,true,%(("source":Earl Wilson),("url":https://www.brainyquote.com/quotes/quotes/e/earlwilson385998.html)),1,Science,may,never,come,up,with,a,better,office,communication,system,,than,the,coffee,break,.]
			//
			//	* function type (0 ... 3)
			//	* recursion depth
			//	* parameters/arguments
			//	* text chunks
			//
			std::cout
				<< "\n***info: quote("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type

			if (cyng::value_cast(reader.get(2), false)) {

				std::string const source = value_cast<std::string>(reader[3].get("source"), "");
				std::string const url = value_cast<std::string>(reader[3].get("url"), "https://example.org");
				auto const tag = value_cast(reader[3].get("tag"), uuid_gen_());

				std::stringstream ss;
				ss
					<< std::endl
					<< "<blockquote cite=\""
					<< url
					<< "\">"
					<< accumulate(reader, 4, frame.size())
					<< std::endl
					<< "<footer>- <cite>"
					<< source
					<< "</cite></footer>"
					<< std::endl
					<< "</blockquote>"
					;

				const std::string node = ss.str();
				ctx.push(cyng::make_object(node));

			}
			else {

				std::cerr
					<< "***error: function quote() only accepts named parameters"
					<< std::endl;

				ctx.push(cyng::make_object("function quote() only accepts named parameters"));

			}
		}

		void generator::fun_cite(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0003,1,true,%(("source":Earl Wilson),("url":https://www.brainyquote.com/quotes/quotes/e/earlwilson385998.html)),1,<p>Science may never come up with a better office communication system, than the coffee break.</p>]
			std::cout
				<< "\n***info: cite("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif
			auto const reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (cyng::value_cast(reader.get(2), false)) {

				auto const source = value_cast<std::string>(reader[3].get("source"), "");
				auto const url = value_cast<std::string>(reader[3].get("url"), "https://example.org");
				auto const tag = value_cast(reader[3].get("tag"), uuid_gen_());

				std::stringstream ss;
				ss
					<< std::endl
					<< "<blockquote cite=\""
					<< url
					<< "\">"
					<< accumulate(reader, 4, frame.size())
					<< std::endl
					<< "<footer>- <cite>"
					<< source
					<< "</cite></footer>"
					<< std::endl
					<< "</blockquote>"
					;

				const std::string node = ss.str();
				ctx.push(cyng::make_object(node));
			}
			else {

				std::cerr
					<< "***error: function cite() only accepts named parameters"
					<< std::endl;

				ctx.push(cyng::make_object("function cite() only accepts named parameters"));
			}
		}

		void generator::fun_title(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0001,1,false,Introduction,into,docScript]
			//	
			//	* function type (0 ... 3)
			//	* recursion depth
			//	* parameters/arguments
			//	* title chunks
			//
			std::cout
				<< "\n***info: title("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;

#endif
			const auto reader = make_reader(frame);

			//	accept only arguments
			if (!cyng::value_cast(reader.get(2), false)) {

				std::string const title = accumulate(reader, 3, frame.size());

				if (verbosity_ > 1)
				{
					std::cout
						<< "***info: title("
						<< title
						<< ")"
						<< std::endl;
				}

				meta_.emplace("title", cyng::make_object(title));
			}
			else {

				std::cerr
					<< "***error: function title() only accepts arguments"
					<< std::endl;

			}

			//
			// function meta has no return values
			//
		}

		void generator::fun_meta(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

			//	[0001,0,true,%(("file-size":4939),("last-write-time":2018-12-30 21:16:04.00000000))]
			//	
			//	* function type
			//	* recursion depth
			//	* parameters/arguments
			//	* parameter set
			//
			std::cout
				<< "\n***info: meta("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl
				;
#endif
			const auto reader = make_reader(frame);
			if (cyng::value_cast(reader.get(2), false)) {

				param_map_t data;
				data = value_cast<param_map_t>(reader.get(3), data);
				this->update_meta(data);
			}
			else {

				std::cerr
					<< "***error: function meta() only accepts named parameters"
					<< std::endl;

			}

			//
			// function meta has no return values
			//
		}

		void generator::fun_list(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0003,1,true,%(("style":upper-roman),("type":ordered)),Change the list style type,See the result,Some of the list types does not work in IE or Opera]
			std::cout
				<< "\n***info: list("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl
				;
#endif
			const auto reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			BOOST_ASSERT(cyng::value_cast(reader.get(2), false));

			auto const type = value_cast<std::string>(reader[3].get("type"), "ordered");
			auto const style = value_cast<std::string>(reader[3].get("style"), "disc");
			auto const tag = (boost::algorithm::equals(type, "ordered")) ? "ol" : "ul";

			std::stringstream ss;

			//
			//	open list
			//
			ss
				<< std::endl
				<< '<'
				<< tag
				<< ' '
				<< "style="
				<< '"'
				<< "list-style-type: "
				<< style
				<< '"'
				<< '>'
				;

			for (std::size_t idx = 4; idx < frame.size(); ++idx)
			{
				const auto item = value_cast<std::string>(reader.get(idx), "");
				ss 
					<< std::endl
					<< "<li>"
					<< item
					<< "</li>"
					;
			}

			//
			//	close list
			//
			ss
				<< std::endl
				<< '<'
				<< '/'
				<< tag
				<< '>'
				;

			const std::string node = ss.str();
			ctx.push(cyng::make_object(node));
		}

		void generator::fun_item(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0001,3,false,See the result]
			std::cout
				<< "\n***info:entry("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl
				;
#endif
			const auto reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			if (!cyng::value_cast(reader.get(2), false)) {

				//
				//	argument list
				//
				auto const txt = value_cast<std::string>(reader.get(3), "");	//	item text
				ctx.push(cyng::make_object(txt));	//	return value is item text
			}
			else {

				std::cerr
					<< "***error: function item() only accepts argument list"
					<< std::endl;

				ctx.push(cyng::make_object("function item() only accepts argument list"));
			}
		}

		void generator::fun_env(context& ctx)
		{
			const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
			//	[0004,1,true,%(("filter":JavaScript),("process":false)),CONTENT]
			std::cout
				<< "\n***info:env.open("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl
				;
#endif
			const auto reader = make_reader(frame);
			auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
			auto const filter = value_cast<std::string>(reader[3].get("filter"), "verbatim");	//	filter
			auto const line_numbers = value_cast(reader[3].get("linenumbers"), false);
			auto const tag = value_cast(reader[3].get("tag"), uuid_gen_());
			auto const input = value_cast<std::string>(reader.get(4), "");	//	input
			auto start = std::begin(input);
			auto stop = std::end(input);

			if (boost::algorithm::equals(filter, "verbatim")) {

				cyng::filter::verbatim filter(input.size());
				for (auto pos = boost::u8_to_u32_iterator<std::string::const_iterator>(start); pos != boost::u8_to_u32_iterator<std::string::const_iterator>(stop); ++pos) {
					filter.put(*pos);
				}
				ctx.push(cyng::make_object("<pre><code>" + filter.get_result() + "</code></pre>"));	//	return value is item text
			}
			else if (boost::algorithm::equals(filter, "C++")) {

				cyng::filter::cpp filter(1, line_numbers, tag, input.size());
				for (auto pos = boost::u8_to_u32_iterator<std::string::const_iterator>(start); pos != boost::u8_to_u32_iterator<std::string::const_iterator>(stop); ++pos) {
					filter.put(*pos);
				}
				ctx.push(cyng::make_object("<pre>" + filter.get_result() + "\n</pre>"));	//	return value is item text
			}
			else {
				ctx.push(cyng::make_object("<pre><code>" + input + "</code></pre>"));	//	return value is item text
			}
		}

		void generator::update_meta(cyng::param_map_t const& data)
		{
			//
			//	update meta data 
			//
			for (auto const& e : data)
			{
				if (boost::algorithm::equals(e.first, "language") || boost::algorithm::equals(e.first, "lang"))	{ 
					language_ = cyng::io::to_str(e.second);
				}
				else {
					meta_.insert(e);
				}
			}
		}

		void generator::fun_generate_file(context& ctx)
		{
			cyng::vector_t frame = ctx.get_frame();

#ifdef _DEBUG
			//	["C:\projects\cyng\build\Debug\out.html",<h1 class="header.1" id="d6....]
			//std::cout
			//	<< "\n***info: generate.file("
			//	<< cyng::io::to_str(frame)
			//	<< ")"
			//	<< std::endl;
#endif

			const boost::filesystem::path p = cyng::value_cast(frame.at(0), boost::filesystem::path());

			if (verbosity_ > 1)
			{
				std::cout
					<< "***info: serialize "
					<< frame.size()
					<< " objects into file "
					<< p
					<< std::endl;
			}

			std::ofstream ofs(p.string(), std::ios::out | std::ios::trunc);
			if (!ofs.is_open())
			{
				std::cerr
					<< "***error cannot open output file "
					<< p
					<< std::endl;
			}
			else
			{
				//
				//	provide a slug if not defined yet.
				//
				slug();

				//
				//	write output file
				//
				generate(ofs, ++frame.begin(), frame.end());
			}
		}

		void generator::fun_generate_meta(context& ctx)
		{
			cyng::vector_t frame = ctx.get_frame();

#ifdef _DEBUG
			//	["C:\projects\cyng\build\Debug\out.json"]
			std::cout
				<< "\n***info: generate.meta("
				<< cyng::io::to_str(frame)
				<< ")"
				<< std::endl;
#endif
			const boost::filesystem::path p = cyng::value_cast(frame.at(0), boost::filesystem::path());
			meta(p);

		}

		void generator::generate(std::ostream& os, cyng::vector_t::const_iterator begin, cyng::vector_t::const_iterator end)
		{
			if (!body_only_)
			{
				//
				//	generate complete HTML file
				//	
				os 
					<< "<!doctype html>"
					<< std::endl
					<< "<html lang=\""
					<< language_
					<< "\">"
					<< std::endl
					<< "<head>"
					<< std::endl
					<< "\t<meta charset=\"utf-8\" />"
					<< std::endl
					;

				for (auto const& e : meta_)
				{
					if (boost::algorithm::equals(e.first, "title"))
					{
						os
							<< "\t<title>"
 							<< cyng::io::to_str(e.second)
							<< "</title>"
							<< std::endl
							//	@see http://ogp.me/
							<< "\t<meta name=\"og:title\" content=\""
							<< cyng::io::to_str(e.second)
							<< "\" />"
							<< std::endl
							<< "\t<meta name=\"og:type\" content=\"article\" />"
							<< std::endl
							;

					}
					else
					{
						os
							<< "\t<meta name=\""
							<< e.first
							<< "\" content=\""
 							<< cyng::io::to_str(e.second)
							<< "\" />"
							<< std::endl
							;
					}
				}

				//
				//	layout optimization for block quotes
				//
				os
					<< "\t<style>"
					<< std::endl
					<< "\t\tbody { font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; }"
					<< std::endl
					<< "\t\tblockquote > p { margin-bottom: 1px; }"
					<< std::endl
					<< "\t\tpre { background-color: whitesmoke; }"
					<< std::endl

					<< "\t\tblockquote {"
					<< std::endl
					<< "\t\t\tborder-left: 4px solid #eee;"
					<< std::endl
					<< "\t\t\tpadding-left: 10px;"
					<< std::endl
					<< "\t\t\tcolor: #777;"
					<< std::endl
					<< "\t\t\tmargin: 16px 20px;"
					<< std::endl
					<< "\t\t}"
					<< std::endl

					<< "\t\timg {"
					<< std::endl
					<< "\t\t\tmax-width: 95%;"
					<< std::endl
					<< "\t\t\t border: 2px solid black;"
					<< std::endl
					<< "\t\t}"
					<< std::endl

					<< "\t</style>"
					<< std::endl
					;
				
				os
					<< "</head>"
					<< std::endl
					;

			}

			//
			//	generate body
			//
			os
				<< "<body>"
				<< std::endl
				;
			std::for_each(begin, end, [this, &os](cyng::object const& obj) {
				os << cyng::io::to_str(obj) << std::endl;
				//os << this->backpatch(cyng::to_string(obj));
			});
			os
				<< "</body>"
				<< std::endl
				;

			if (!body_only_)
			{
				//
				//	generate complete HTML file
				//	
				os
					<< "</html>"
					<< std::endl
					;
			}
		}

		boost::filesystem::path generator::resolve_path(std::string const& s) const
		{
			boost::filesystem::path p(s);
			for (auto dir : includes_)
			{
				if (boost::filesystem::exists(dir / p))
				{
					return boost::filesystem::path(dir / p);
				}
			}
			return boost::filesystem::path();
		}

		void generator::run(vector_t const& prg, std::chrono::milliseconds compile_time)
		{
			if (verbosity_ > 2)
			{
				std::cout
					<< "***info: start generator with "
					<< prg.size()
					<< " op codes"
					<< std::endl;
			}
			meta_.emplace("compile-time", cyng::make_object(compile_time));
			vm_.async_run(vector_t(prg));
		}

		std::size_t generator::meta(boost::filesystem::path const& out) const
		{
			if (verbosity_ > 1)
			{
				std::cout
					<< "***info: generate meta file "
					<< out
					<< " with "
					<< meta_.size()
					<< " entries"
					<< std::endl;
			}

			std::ofstream file(out.string(), std::ios::out | std::ios::trunc);
			if (!file.is_open())
			{
				std::cerr
					<< "***error cannot open file "
					<< out
					<< std::endl;
			}
			else
			{
				if (verbosity_ > 3)
				{
					cyng::json::write(std::cout, cyng::make_object(meta_));
					std::cout << std::endl;
					//	cyng::serialize_json_pretty(std::cout, make_object(meta_), cyng::io::custom_callback());
				}
// 				cyng::serialize_json(file, meta_, cyng::io::custom_callback());
				cyng::json::write(file, cyng::make_object(meta_));

			}
			return meta_.size();
		}

		std::string generator::generate_header(std::size_t level, std::string const& txt, boost::uuids::uuid tag)
		{
			if (level == 0)
			{
				std::cerr
					<< "***error: header indentation level is zero ("
					<< txt
					<< ")"
					<< std::endl;
				return "error";
			}

			//
			//	update numeration
			//
			std::ptrdiff_t diff = level - numeration_.size();
			if (diff == 1)
			{
				//
				//	increase indention
				//
				numeration_.push_back(1);
			}
			else if (diff == 0)
			{
				//
				//	indention unchanged
				//
				numeration_.at(level - 1)++;
			}
			else if (diff == -1)
			{
				//
				//	decrease indention
				//
				numeration_.at(level - 1)++;
				numeration_.pop_back();
			}
			else
			{
				std::cerr
					<< "***error: header with wrong indentation level: "
					<< diff
					<< " - "
					<< txt
					<< std::endl;
			}


			//
			//	update structure
			//
			auto pos = structure_.emplace(std::piecewise_construct,
				std::forward_as_tuple(tag),
				std::forward_as_tuple(element::HEADER, txt, numeration_));

			if (pos.second)
			{
				std::stringstream ss;
				ss
					<< "<h"
					<< level
					<< " class=\"header."
					<< level
					<< "\" id=\""
					<< tag
					<< "\">"
					<< pos.first->second.to_str()
					<< "</h"
					<< level
					<< ">"
					;
				const std::string node = ss.str();
				if (verbosity_ > 1)
				{
					std::cout
						<< "***info: header("
						<< pos.first->second.to_str()
						<< ")"
						<< std::endl;
				}
				return node;

			}

			std::cerr
				<< "***error: cannot insert new header "
				<< txt
				<< std::endl;
			return "error";

		}

		void generator::slug()
		{
			auto pos = meta_.find("slug");
			if (pos == meta_.end()) {

				//
				//	build a slug from title
				//
				auto const title = get_title();
				std::string slug;
				for (auto const& c : title) {
					if ((c > 47 && c < 58) || (c > 96 && c < 123)) {
						slug += c;
					}
					else if (c > 64 && c < 91) {
						slug += (c + 32);
					}
					else if (c == '-' || c == ' ') {
						if (!slug.empty() && slug.back() != '-') {
							slug += '-';
						}
					}
				}

				if (verbosity_ > 1)
				{
					std::cout
						<< "***info: auto generated slug: "
						<< slug
						<< std::endl;
				}

				//
				//	update meta data
				//
				meta_.emplace("slug", cyng::make_object(slug));
			}
		}

		std::string generator::get_title()
		{
			auto pos = meta_.find("title");
			return (pos == meta_.end())
				? get_filename()
				: cyng::io::to_str(pos->second)
				;
		}

		std::string generator::get_filename()
		{
			auto pos = meta_.find("file-name");
			return (pos == meta_.end())
				? "NO FILENAME"
				: cyng::io::to_str(pos->second)
				;
		}


		//
		//	free functions
		//
		std::string accumulate(cyng::vector_reader const& reader, std::size_t start, std::size_t end)
		{
			BOOST_ASSERT_MSG(start < reader.size(), "out of range");
			std::string str;
			str.reserve((end - start) * 5);	//	estimation
			for (std::size_t idx = start; idx < end; ++idx)
			{
				auto const s = cyng::io::to_str(reader.get(idx));
				//const auto s = value_cast<std::string>(reader.get(idx), "");
				if ((idx != start) && !(s == "." || s == "," || s == ":" || s == "?" || s == "!"))
				{
					str += " ";
				}
				str += s;
			}
			return str;
		}

		std::string accumulate(cyng::vector_reader const& reader
			, std::size_t start
			, std::size_t end
			, std::string tag)
		{
			if (start == reader.size())	return "";	//	no content

			BOOST_ASSERT_MSG(start < reader.size(), "out of range");
			BOOST_ASSERT_MSG(start < end, "invalid range");
			std::string str;
			str.reserve((end - start) * 5);	//	estimation
			str = '<' + tag + '>';
			for (std::size_t idx = start; idx < end; ++idx)
			{
				const auto s = value_cast<std::string>(reader.get(idx), "");
				if ((idx != start) && !(s == "." || s == "," || s == ":" || s == "?" || s == "!"))
				{
					str += " ";
				}
				str += s;
			}
			return str + '<' + '/' + tag + '>';

		}

		std::string get_extension(boost::filesystem::path const& p)
		{
			if (p.empty())
			{
				return "";
			}
			std::string s = p.extension().string();
			return s.substr(1, s.size() - 1);
		}

		//
		//	element
		//
		element::element(type t, std::string const& s, std::vector<std::size_t> const& chapter)
			: type_(t)
			, text_(s)
			, chapter_(chapter)
		{}

		std::string element::level() const
		{
			std::stringstream ss;

			//
			// handle first element
			//
			auto pos = chapter_.begin();
			if (pos != chapter_.end())
			{
				ss << *pos;
				++pos;
			}

			//
			// join all other elements
			//
			std::for_each(pos, chapter_.end(), [&ss](std::size_t n) {
				ss << '.' << n;
			});

			return ss.str();
		}

		std::string element::to_str() const
		{
			switch (type_) {
			case FIGURE: return level() + ": " + text_;
			default:
				break;
			}
			return level() + "&nbsp;" + text_;
		}

		std::size_t element::depth() const
		{
			return chapter_.size();
		}

		//element& element::operator=(element const& other)
		//{
		//	if (this != &other)
		//	{
		//		//const type type_;
		//		//const std::string text_;
		//		//const std::vector<std::size_t>	chapter_;

		//	}
		//	return *this;
		//}

		//	comparison
		bool operator==(element const& lhs, element const& rhs)
		{
			return (lhs.type_ == rhs.type_)
				? (lhs.chapter_ == rhs.chapter_)
				: false
				;
		}
		bool operator<(element const& lhs, element const& rhs)
		{
			return (lhs.type_ == rhs.type_)
				? (lhs.chapter_ < rhs.chapter_)
				: (lhs.type_ < rhs.type_)
				;
		}
		bool operator!=(element const& lhs, element const& rhs)
		{
			return !(lhs == rhs);
		}
		bool operator>(element const& lhs, element const& rhs)
		{
			//	note the reversed notation
			return rhs < lhs;
		}
		bool operator<=(element const& lhs, element const& rhs)
		{
			return !(lhs > rhs);
		}
		bool operator>=(element const& lhs, element const& rhs)
		{
			return !(lhs < rhs);
		}


		//	comparison
		bool operator==(element_t const& lhs, element_t const& rhs)
		{
			return lhs.second == rhs.second;
		}
		bool operator<(element_t const& lhs, element_t const& rhs)
		{
			return lhs.second < rhs.second;
		}
		bool operator!=(element_t const& lhs, element_t const& rhs)
		{
			return !(lhs == rhs);
		}
		bool operator>(element_t const& lhs, element_t const& rhs)
		{
			//	note the reversed notation
			return rhs < lhs;
		}
		bool operator<=(element_t const& lhs, element_t const& rhs)
		{
			return !(lhs > rhs);
		}
		bool operator>=(element_t const& lhs, element_t const& rhs)
		{
			return !(lhs < rhs);
		}

	}	//	docscript
}	//	cyng


