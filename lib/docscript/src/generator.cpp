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
				ctx.set_return_value(cyng::make_object(std::chrono::system_clock::now()), 0);
            });

            vm_.register_function("meta", 3, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				//	[%(("file-size":2962),("last-write-time":2017-10-23 18:19:32.00000000)),true,1])
				std::cout
					<< "\n***info: meta("
					<< cyng::io::to_str(frame)
					<< ")"
					<< std::endl;

#endif
				const auto reader = make_reader(frame);
				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
// 				const std::size_t size = reader.get_index(0);
 				param_map_t data;
//  				data = reader.get(2, data);
				data = value_cast<param_map_t>(reader.get(2), data);
 				//BOOST_ASSERT_MSG(size == 1, "internal error (meta)");
				if (size == 1) {
					this->update_meta(data);
				}
				else {

				}

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

				//	[1idx,true,"Introduction into docScript"]
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				//	[3idx,true,"docScript","into","Introduction"]
				std::cout
					<< "\n***info: title("
					//<< cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;

#endif
				const cyng::vector_reader reader(frame);
				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
// 				const std::size_t size = reader.get_index(0);
				BOOST_ASSERT_MSG(size + 2 == frame.size(), "internal error (title)");

				std::string title;
				for (std::size_t idx = size + 1; idx > 1; idx--)
				{
					title += value_cast<std::string>(reader.get(idx), "");
// 					title += reader.get_string(idx);
					if (idx > 2)
					{
						title += " ";
					}
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
					//<< cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;

#endif
				const cyng::vector_reader reader(frame);
            });

            vm_.register_function("header", 3, [this](context& ctx) {

				//	[1idx,true,%(("level":"1"),("tag":"79bf3ba0-2362-4ea5-bcb5-ed93844ac59a"),("title":"Basics"))]
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				std::cout
					<< "\n***info: header("
					// << cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;

#endif
				const cyng::vector_reader reader(frame);

				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
				const std::string txt = value_cast<std::string>(reader[2].get("title"), "NO TITLE");
				const std::size_t level = value_cast<std::size_t>(reader[2].get("level"), 0);
				const std::string stag = value_cast<std::string>(reader[2].get("tag"), boost::uuids::to_string(uuid_gen_()));
				const boost::uuids::uuid tag = name_gen_(stag);

				const std::string node = generate_header(level, txt, tag);
				ctx.set_return_value(make_object(node), 0);
            });

            vm_.register_function("header.1", 3, [this](context& ctx) {
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				std::cout
					<< "\n***info: header.1("
					// << cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;

#endif
				const cyng::vector_reader reader(frame);

				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
// 				if (cyng::primary_type_test<cyng::param_map_t>(reader.get_object(2)))
				{
					BOOST_ASSERT(size == 1);
					const std::string txt = value_cast<std::string>(reader[2].get("title"), "NO TITLE");
					const std::string stag = value_cast<std::string>(reader[2].get("tag"), boost::uuids::to_string(uuid_gen_()));
					const boost::uuids::uuid tag = name_gen_(stag);

					const std::string node = generate_header(1, txt, tag);
					ctx.set_return_value(make_object(node), 0);
				}
// 				else
// 				{
// 					const std::string txt = accumulate(reader, size + 1, 1);
// 					const std::string node = generate_header(1, txt, uuid_gen_());
// 					ctx.set_return_value(make_object(node), 0);
// 				}
            });

            vm_.register_function("header.2", 3, [this](context& ctx) {

				//	[1idx,true,"Examples"]
				//	[3idx, true, "DocScript", "of", "Examples"]
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				std::cout
					<< "\n***info: header.2("
					// << cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;

#endif
				const cyng::vector_reader reader(frame);

				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
// 				if (cyng::primary_type_test<cyng::param_map_t>(reader.get_object(2)))
				{
					BOOST_ASSERT(size == 1);
					const std::string txt = value_cast<std::string>(reader[2].get("title"), "NO TITLE");
					const std::string stag = value_cast<std::string>(reader[2].get("tag"), boost::uuids::to_string(uuid_gen_()));
					const boost::uuids::uuid tag = name_gen_(stag);

					const std::string node = generate_header(2, txt, tag);
					ctx.set_return_value(make_object(node), 0);
				}
// 				else
// 				{
// 					const std::string txt = accumulate(reader, size + 1, 1);
// 					const std::string node = generate_header(2, txt, uuid_gen_());
// 					ctx.set_return_value(make_object(node), 0);
// 				}
            });

            vm_.register_function("header.3", 3, [this](context& ctx) {
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				std::cout
					<< "\n***info: header.3("
					// << cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;

#endif
				const cyng::vector_reader reader(frame);

				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
// 				if (cyng::primary_type_test<cyng::param_map_t>(reader.get_object(2)))
				{
					BOOST_ASSERT(size == 1);
					const std::string txt = value_cast<std::string>(reader[2].get("title"), "NO TITLE");
					const std::string stag = value_cast<std::string>(reader[2].get("tag"), boost::uuids::to_string(uuid_gen_()));
					const boost::uuids::uuid tag = name_gen_(stag);

					const std::string node = generate_header(3, txt, tag);
					ctx.set_return_value(make_object(node), 0);
				}
// 				else
// 				{
// 					const std::string txt = accumulate(reader, size + 1, 1);
// 					const std::string node = generate_header(3, txt, uuid_gen_());
// 					ctx.set_return_value(make_object(node), 0);
// 				}
            });

            vm_.register_function("header.4", 3, [this](context& ctx) {
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				std::cout
					<< "\n***info: header.4("
					// << cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;

#endif
				const cyng::vector_reader reader(frame);

				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
// 				if (cyng::primary_type_test<cyng::param_map_t>(reader.get_object(2)))
				{
					BOOST_ASSERT(size == 1);
					const std::string txt = value_cast<std::string>(reader[2].get("title"), "NO TITLE");
					const std::string stag = value_cast<std::string>(reader[2].get("tag"), boost::uuids::to_string(uuid_gen_()));
					const boost::uuids::uuid tag = name_gen_(stag);

					const std::string node = generate_header(4, txt, tag);
					ctx.set_return_value(make_object(node), 0);
				}
// 				else
// 				{
// 					const std::string txt = accumulate(reader, size + 1, 1);
// 					const std::string node = generate_header(4, txt, uuid_gen_());
// 					ctx.set_return_value(make_object(node), 0);
// 				}
            });

            vm_.register_function("paragraph", 1, [this](context& ctx) {

				//	[131idx,".","power","more","you", ..."]
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				std::cout
					<< "\n***info: paragraph("
					// << cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;
#endif
				const cyng::vector_reader reader(frame);

				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
				BOOST_ASSERT_MSG(size < frame.size(), "internal error (paragraph)");

				const std::string node = "\n<p>" + accumulate(reader, size, 0) + "</p>";
				ctx.set_return_value(make_object(node), 0);

            });

            vm_.register_function("bold", 3, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				//	[3idx,false,"here","bold","everything"]
				std::cout
					<< "\n***info: bold("
					// << cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;

#endif
				const cyng::vector_reader reader(frame);
				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
				//BOOST_ASSERT_MSG(size + 2 == frame.size(), "internal error (bold)");

				const std::string node = "<b>"
					+ accumulate(reader, size + 1, 1)
					+ "</b>"
					;
				//if (verbosity_ > 2)
				//{
				//	std::cout
				//		<< "***info: bold("
				//		<< node
				//		<< ")"
				//		<< std::endl;
				//}

				ctx.set_return_value(make_object(node), 0);

            });

            vm_.register_function("emphasise", 3, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				//	[1idx,"program"]
				//std::cout
				//	<< "\n***info: emphasise("
				//	<< cyng::io::to_literal(frame)
				//	<< ")"
				//	<< std::endl;

#endif
				const cyng::vector_reader reader(frame);
				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
				//BOOST_ASSERT_MSG(size + 2 == frame.size(), "internal error (italic)");

				const std::string node = "<em>"
					+ accumulate(reader, size + 1, 1)
					+ "</em>"
					;
				//if (verbosity_ > 2)
				//{
				//	std::cout
				//		<< "***info: italic("
				//		<< node
				//		<< ")"
				//		<< std::endl;
				//}

				ctx.set_return_value(make_object(node), 0);

            });

            vm_.register_function("color", 3, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				//	[1idx,true,%(("red":"spiced up"))]
				//std::cout
				//	<< "\n***info: color("
				//	<< cyng::io::to_literal(frame)
				//	<< ")"
				//	<< std::endl;

#endif
				const cyng::vector_reader reader(frame);
				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
				const auto map = value_cast<param_map_t>(reader.get(2), param_map_t());
				//BOOST_ASSERT_MSG(map.size() == 1, "internal error (color)");

				std::stringstream ss;
				ss
					<< "<span style=\"color:"
					<< map.begin()->first
					<< "\">"
// 					<< cyng::to_string(map.begin()->second)
					<< "</span>"
					;
				const std::string node = ss.str();
				if (verbosity_ > 2)
				{
					std::cout
						<< "***info: color("
						<< node
						<< ")"
						<< std::endl;
				}

				//ctx.set_return_value_invoke(make_object(node), 0);
				ctx.set_return_value(make_object(node), 0);

            });

            vm_.register_function("link", 3, [this](context& ctx) {

				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG

				//	[1idx,false,%(("text":"LaTeX"),("url":"https://www.latex-project.org/"))]
				std::cout
					<< "\n***info: link("
					// << cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;

#endif
				const cyng::vector_reader reader(frame);
				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
				//const auto map = reader.get(2, cyng::param_map_t());
				BOOST_ASSERT_MSG(size == 1, "internal error (link)");

				const std::string url = value_cast<std::string>(reader[2].get("url"), "");
				const std::string node = "<a href=\""
					+ url
					+ "\" title=\""
					+ value_cast<std::string>(reader[2].get("title"), url)
					+ "\">"
					+ value_cast<std::string>(reader[2].get("text"), "")
					+ "</a>"
					;

				ctx.set_return_value(make_object(node), 0);
				//ctx.set_return_value_invoke(make_object(node), 0);

            });

			//<figure>
			//  <img src="img_pulpit.jpg" alt="The Pulpit Rock" width="304" height="228">
			//  <figcaption>Fig1. - A view of the pulpit rock in Norway.</figcaption>
			//</figure>
            vm_.register_function("figure", 3, [this](context& ctx) {
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				//	[1idx,false,%(("caption":"figure with caption"),("source":"LogoSmall.jpg"))]
				std::cout
					<< "\n***info: figure("
					// << cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;
#endif
				const cyng::vector_reader reader(frame);
				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
				//BOOST_ASSERT_MSG(size + 2 == frame.size(), "internal error (quote)");

				const std::string source = value_cast<std::string>(reader[2].get("source"), "");
				const boost::filesystem::path p = resolve_path(source);
				std::ifstream file(p.string(), std::ios::binary | std::ios::ate);
				if (!file.is_open())
				{
					std::cerr
						<< "***error cannot open figure file "
						<< source
						<< std::endl;
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

					//
					//	encode image as base 64
					//
					const std::string alt = value_cast<std::string>(reader[2].get("alt"), "");
					const std::string cap = value_cast<std::string>(reader[2].get("caption"), "");

					std::stringstream ss;
					ss
						<< std::endl
						<< "<figure>"
						<< "<img alt=\""
						<< alt
						<< "\" src=\"data:image/"
						<< get_extension(p)
						<< ";base64,"
 						<< cyng::crypto::base64_encode(buffer.data(), buffer.size())
						<< "\" />\n<figcaption>"
						<< cap
						<< "</figcaption>\n"
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
					ctx.set_return_value(make_object(node), 0);
				}
            });


            vm_.register_function("quote", 3, [this](context& ctx) {
				const cyng::vector_t frame = ctx.get_frame();
#ifdef _DEBUG
				//	[1idx,true,%(("source":"Earl Wilson"),("url":"https://www.brainyquote.com/quotes/quotes/e/earlwilson385998.html"))]
				std::cout
					<< "\n***info: quote("
					// << cyng::io::to_literal(frame)
					<< ")"
					<< std::endl;
#endif
				const cyng::vector_reader reader(frame);
				const std::size_t size = value_cast<std::size_t>(reader.get(0), 0);
// 				const std::size_t size = reader.get_index(0);

				const std::string source = value_cast<std::string>(reader[size + 1].get("source"), "source");
				const std::string url = value_cast<std::string>(reader[size + 1].get("url"), "");

				std::stringstream ss;
				ss
					<< std::endl
					<< "<blockquote cite=\""
					<< url
					<< "\">"
					<< accumulate(reader, size, 1)
					<< std::endl
					<< "<footer>- <cite>"
					<< source
					<< "</cite></footer>"
					<< std::endl
					<< "</blockquote>"
					;

				const std::string node = ss.str();
				ctx.set_return_value(cyng::make_object(node), 0);

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
// 				if (!cyng::primary_type_test<boost::filesystem::path>(frame.at(0)))
// 				{
// 					std::cerr
// 						<< "***error: input parameter for function generate() is not of type filesystem::path "
// 						<< std::endl;
// 
// 				}
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
					//serialize(file, ++frame.begin(), frame.end());
				}

            });


		}

		void generator::update_meta(cyng::param_map_t const& data)
		{
			//
			//	update meta data 
			//
			for (auto e : data)
			{
				meta_.insert(e);
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
					<< "<!DOCTYPE html>"
					<< std::endl
					<< "<html xmlns=\"http://www.w3.org/1999/xhtml\">"
					<< std::endl
					<< "<head>"
					<< std::endl
					<< "\t<meta charset=\"utf-8\" />"
					<< std::endl
					;

				for (auto e : meta_)
				{
					if (boost::algorithm::equals(e.first, "title"))
					{
						os
							<< "\t<title>"
// 							<< cyng::to_string(e.second)
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
// 							<< cyng::to_string(e.second)
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
// 				os << cyng::to_string(obj);
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
					<< std::endl
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
						//<< node
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
			//BOOST_ASSERT_MSG(start < reader.size(), "out of range");
			std::string str;
			for (std::size_t idx = start; idx > end; idx--)
			{
				const auto s = value_cast<std::string>(reader.get(idx), "");
// 				const auto s = reader.get_string(idx);
				if ((idx > 1) && (idx != start) && !(s == "." || s == "," || s == ":"))
				{
					str += " ";
				}
				str += s;
			}
			return str;
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


