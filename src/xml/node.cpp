#include <cyng/xml/node.h>
//#include <cyng/obj/factory.hpp>
#include <cyng/xml/reader.hpp>

#include <sstream>

#include <pugixml.hpp>

#include <boost/core/ignore_unused.hpp>

namespace cyng 
{
	namespace xml 
	{
		//
		//	document
		//
		struct document::impl {
			pugi::xml_document doc;
		};
		document::document()  noexcept
			: node_(std::make_unique<impl>())
		{}

		document::document(document&& p) noexcept
			: node_(std::move(p.node_))
		{}

		document::~document()
		{}

		bool document::empty() const {
			return !node_;
		}

		node document::read_file(std::string const& p) {
			node n;
			pugi::xml_parse_result result = node_->doc.load_file(p.c_str());
			if (result) {
				n.node_ = std::make_unique<node::impl>(node_->doc.first_child());
			}
			return n;
		}

		node document::read_file(std::string const& p, std::string const& name) {
			node n;
			pugi::xml_parse_result result = node_->doc.load_file(p.c_str());
			if (result) {
				n.node_ = std::make_unique<node::impl>(node_->doc.child(name.c_str()));
			}
			return n;
		}

		std::string document::to_str() const {
			std::stringstream ss;
			ss << *this;
			return ss.str();
		}

		std::ostream& operator<<(std::ostream& os, document const& doc) {
			if (!doc.empty()) {
				doc.node_->doc.save(os, "\t", pugi::format_default | pugi::format_no_declaration);
			}
			return os;
		}

		//
		//	node
		//
		struct node::impl {
			pugi::xml_node node;
		};

		node::node()  noexcept
			: node_(nullptr) 
		{}

		node::node(node&& p) noexcept 
			: node_(std::move(p.node_))
		{}

		node::~node()
		{}

		bool node::empty() const {
			return !node_;
		}


		node node::get_child(std::string const& name) {
			node n;
			if (!empty()) {
				n.node_ = std::make_unique<impl>(node_->node.child(name.c_str()));
			}
			return n;
		}

		node node::add_leaf(std::string const& name, std::string const& value) {
			node n;
			if (!empty()) {
				auto node = node_->node.prepend_child(name.c_str());
				node.append_child(pugi::node_pcdata).set_value(value.c_str());
				n.node_ = std::make_unique<impl>(node);
			}
			return n;
		}

		bool node::set_attribute(std::string const& name, std::string const& value) {
			if (!empty()) {
				auto a = node_->node.attribute(name.c_str());
				if (!a) {
					node_->node.prepend_attribute(name.c_str()) = value.c_str();
				}
				else {
					a.set_value(value.c_str());
				}
				return true;
			}
			return false;
		}

		bool node::del_attribute(std::string const& name) {
			return (!empty()) 
				? node_->node.remove_attribute(name.c_str())
				: false
				;
		}
	}
}

