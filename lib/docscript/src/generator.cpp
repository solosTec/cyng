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
				//std::cout
				//	<< "\n***info: now("
				//	<< cyng::io::to_literal(frame)
				//	<< ")"
				//	<< std::endl;
#endif
				//
				//	produce result value
				//
				ctx.push(cyng::make_now());
				//ctx.set_return_value(cyng::make_object(std::chrono::system_clock::now()), 0);
            });

            vm_.register_function("meta", 2, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				//	[00000001,%(("file-size":106),("last-write-time":2018-10-25 22:17:08.00000000))]
				//	
				//	* function type
				//	* parameter set
				//
				std::cout
					<< "\n***info: meta("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;

#endif
				const auto reader = make_reader(frame);
 				param_map_t data;
				data = value_cast<param_map_t>(reader.get(1), data);
				this->update_meta(data);

				//
				// function meta has no return values
				//
            });

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


// 				ctx.squeeze(cyng::vector_t{ make_object(op) });

				//
				// function meta has no return values
				//

            });

            vm_.register_function("title", 1, [this](context& ctx) {

				//	[00000001,Introduction,into,docScript]
				//	
				//	* function type (0 ... 3)
				//	* title chunks
				//
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				//	[3idx,true,"docScript","into","Introduction"]
				std::cout
					<< "\n***info: title("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;

#endif
				const cyng::vector_reader reader(frame);

				std::string title;
				for (std::size_t idx = 1u; idx < frame.size(); ++idx)
				{
					if (idx > 1) {
						title += " ";
					}
					title += value_cast<std::string>(reader.get(idx), "");
				}

				if (verbosity_ > 1)
				{
					std::cout
						<< "***info: title("
						<< title
						<< ")"
						<< std::endl;
				}

				meta_.emplace("title", cyng::make_object(title));

				//
				// function meta has no return values
				//
            });

            vm_.register_function("contents", 2, [this](context& ctx) {

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
			});

            vm_.register_function("header", 2, [this](context& ctx) {

				//	[00000001,%(("level":user-defined),("tag":79bf3ba0-2362-4ea5-bcb5-ed93844ac59a),("title":Second Header)),true]
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
				auto const level = value_cast<std::size_t>(reader[1].get("level"), 0);
				auto const stag = value_cast<std::string>(reader[1].get("tag"), boost::uuids::to_string(uuid_gen_()));
				auto const txt = value_cast<std::string>(reader[1].get("title"), "NO TITLE");
				auto const tag = name_gen_(stag);


				const std::string node = generate_header(level, txt, tag);
				ctx.push(cyng::make_object(node));
            });

            vm_.register_function("header.1", 1, [this](context& ctx) {

				//	[00000001,First,Header]
				//	
				//	* function type
				//	* params
				//
				const cyng::vector_t frame = ctx.get_frame();

#ifdef _DEBUG
				std::cout
					<< "\n***info: header.1("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;
#endif
				auto const reader = make_reader(frame);
				auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
				auto const txt = accumulate(reader, 1, frame.size());
				auto const node = generate_header(1, txt, uuid_gen_());
				ctx.push(cyng::make_object(node));

            });

            vm_.register_function("header.2", 3, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();

#ifdef _DEBUG
				std::cout
					<< "\n***info: header.2("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;
#endif
				auto const reader = make_reader(frame);
				auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
				auto const txt = accumulate(reader, 1, frame.size());
				auto const node = generate_header(1, txt, uuid_gen_());
				ctx.push(cyng::make_object(node));

			});

            vm_.register_function("header.3", 3, [this](context& ctx) {

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
				auto const txt = accumulate(reader, 1, frame.size());
				auto const node = generate_header(1, txt, uuid_gen_());
				ctx.push(cyng::make_object(node));

			});

            vm_.register_function("header.4", 3, [this](context& ctx) {

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
				auto const txt = accumulate(reader, 1, frame.size());
				auto const node = generate_header(1, txt, uuid_gen_());
				ctx.push(cyng::make_object(node));

			});

            vm_.register_function("paragraph", 2, [this](context& ctx) {

				//	[00000001,159,Lorem,ipsum,dolor,sit,amet,...sit,amet,.]
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				std::cout
					<< "\n***info: paragraph("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;
#endif
				auto const reader = make_reader(frame);
				auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
				auto const size = value_cast<std::size_t>(reader.get(1), 0u);
				BOOST_ASSERT(size == frame.size() - 2);
				const std::string node = accumulate(reader, 2, frame.size(), "p");
				ctx.push(cyng::make_object(node));

            });

            vm_.register_function("bold", 2, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				//	[00000002,ipsum]
				std::cout
					<< "\n***info: bold("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;
#endif
				auto const reader = make_reader(frame);
				auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
				const std::string node = accumulate(reader, 1, frame.size(), "b");

				if (verbosity_ > 3)
				{
					std::cout
						<< "***info: bold("
						<< node
						<< ")"
						<< std::endl;
				}

				ctx.push(cyng::make_object(node));

            });

            vm_.register_function("emphasise", 2, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();

#ifdef _DEBUG
				//	[00000002,ipsum]
				std::cout
					<< "\n***info: emphasise("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;
#endif
				auto const reader = make_reader(frame);
				auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
				const std::string node = accumulate(reader, 1, frame.size(), "em");

				if (verbosity_ > 3)
				{
					std::cout
						<< "***info: emphasise("
						<< node
						<< ")"
						<< std::endl;
				}

				ctx.push(cyng::make_object(node));
			});

            vm_.register_function("color", 3, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				//	[00000002,%(("red":spiced up)),false]
				std::cout
					<< "\n***info: color("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;

#endif
				auto const reader = make_reader(frame);
				auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
				const auto map = value_cast(reader.get(1), param_map_t());
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

            });

            vm_.register_function("link", 3, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				//	[00000000,%(("text":LaTeX),("url":https://www.latex-project.org/)),false]
				std::cout
					<< "\n***info: link("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;
#endif
				auto const reader = make_reader(frame);
				auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type

				const std::string url = value_cast<std::string>(reader[1].get("url"), "");
				const std::string node = "<a href=\""
					+ url
					+ "\" title=\""
					+ value_cast<std::string>(reader[1].get("title"), url)
					+ "\">"
					+ value_cast<std::string>(reader[1].get("text"), "")
					+ "</a>"
					;

				ctx.push(cyng::make_object(node));

            });

			//<figure>
			//  <img src="img_pulpit.jpg" alt="The Pulpit Rock" width="304" height="228">
			//  <figcaption>Fig1. - A view of the pulpit rock in Norway.</figcaption>
			//</figure>
            vm_.register_function("figure", 3, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				//	[00000001,%(("caption":figure with caption),("source":LogoSmall.jpg)),true]
				std::cout
					<< "\n***info: figure("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;
#endif
				
				auto const reader = make_reader(frame);
				auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type

				const std::string source = value_cast<std::string>(reader[1].get("source"), "");
				const boost::filesystem::path p = resolve_path(source);
				std::ifstream file(p.string(), std::ios::binary | std::ios::ate);
				if (!file.is_open())
				{
					std::cerr
						<< "***error cannot open figure file "
						<< source
						<< std::endl;
					ctx.push(cyng::make_object("cannot open file " + source));
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
					std::string const alt = value_cast<std::string>(reader[1].get("alt"), "");
					std::string const cap = value_cast<std::string>(reader[1].get("caption"), "");
					auto const tag = value_cast(reader[1].get("tag"), uuid_gen_());

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
            });


            vm_.register_function("quote", 3, [this](context& ctx) {
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				//	[00000004,%(("source":Earl Wilson),("url":https://www.brainyquote.com/quotes/quotes/e/earlwilson385998.html)),true]
				std::cout
					<< "\n***info: quote("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;
#endif
				auto const reader = make_reader(frame);
				auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
				std::string const source = value_cast<std::string>(reader[1].get("source"), "");
				std::string const url = value_cast<std::string>(reader[1].get("url"), "https://example.org");
				auto const tag = value_cast(reader[1].get("tag"), uuid_gen_());

				std::stringstream ss;
				ss
					<< std::endl
					<< "<blockquote cite=\""
					<< url
					<< "\">"
					<< accumulate(reader, 3, frame.size())
					<< std::endl
					<< "<footer>- <cite>"
					<< source
					<< "</cite></footer>"
					<< std::endl
					<< "</blockquote>"
					;

				const std::string node = ss.str();
				ctx.push(cyng::make_object(node));

            });

			vm_.register_function("cite", 3, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				//	[00000003,%(("source":Earl Wilson),("url":https://www.brainyquote.com/quotes/quotes/e/earlwilson385998.html)),true,<p> Science may never come up with a better office communication system, than the coffee break.</p>]
				std::cout
					<< "\n***info: cite("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;
#endif
				auto const reader = make_reader(frame);
				auto const ft = value_cast<std::uint32_t>(reader.get(0), 0);	//	function type
				auto const source = value_cast<std::string>(reader[1].get("source"), "");
				auto const url = value_cast<std::string>(reader[1].get("url"), "https://example.org");
				auto const tag = value_cast(reader[1].get("tag"), uuid_gen_());

				std::stringstream ss;
				ss
					<< std::endl
					<< "<blockquote cite=\""
					<< url
					<< "\">"
					<< accumulate(reader, 2, frame.size())
					<< std::endl
					<< "<footer>- <cite>"
					<< source
					<< "</cite></footer>"
					<< std::endl
					<< "</blockquote>"
					;

				const std::string node = ss.str();
				ctx.push(cyng::make_object(node));

			});

            vm_.register_function("generate", 1, [this](context& ctx) {
				cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				//	["C:/projects/cyng/Debug/out.html"path,null,null]
				std::cout
					<< "\n***info: generate("
					// << cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;

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
					this->generate(ofs, ++frame.begin(), frame.end());
				}
            });
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

		void generator::run(vector_t const& prg)
		{
			if (verbosity_ > 2)
			{
				std::cout
					<< "***info: start generator with "
					<< prg.size()
					<< " op codes"
					<< std::endl;
			}
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
// 					cyng::serialize_json_pretty(std::cout, make_object(meta_), cyng::io::custom_callback());
				}
// 				cyng::serialize_json(file, meta_, cyng::io::custom_callback());
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
				const auto s = value_cast<std::string>(reader.get(idx), "");
				if ((idx != start) && !(s == "." || s == "," || s == ":"))
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
			BOOST_ASSERT_MSG(start < reader.size(), "out of range");
			BOOST_ASSERT_MSG(start < end, "invalid range");
			std::string str;
			str.reserve((end - start) * 5);	//	estimation
			str = '<' + tag + '>';
			for (std::size_t idx = start; idx < end; ++idx)
			{
				const auto s = value_cast<std::string>(reader.get(idx), "");
				if ((idx != start) && !(s == "." || s == "," || s == ":"))
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


