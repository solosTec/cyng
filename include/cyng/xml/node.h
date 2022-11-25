/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_XML_NODE_H
#define CYNG_XML_NODE_H

#include <cyng/obj/object.h>

#include <memory>
#include <string>

namespace cyng {
    namespace xml {
        class document;
        /**
         * XML Node
         */
        class node {
            friend class document;

          public:
            node() noexcept;
            node(node &&) noexcept;
            ~node();

            bool empty() const;

            node get_child(std::string const &);

            /**
             * add a node with an value
             */
            node add_leaf(std::string const &, std::string const &);
            bool set_attribute(std::string const &name, std::string const &value);
            bool del_attribute(std::string const &name);

          private:
            struct impl;
            std::unique_ptr<impl> node_;
        };

        /**
         * XML document
         */
        class document {
          public:
            document() noexcept;
            document(document &&) noexcept;
            ~document();

            bool empty() const;
            node read_file(std::string const &);
            node read_file(std::string const &file_name, std::string const &root_name);

            friend std::ostream &operator<<(std::ostream &, document const &);
            std::string to_string() const;

          private:
            struct impl;
            std::unique_ptr<impl> node_;
        };

    } // namespace xml
} // namespace cyng
#endif
