#include "string_node.h"

namespace channel {
	string_node::string_node(u64 line_number, const std::string& value)
		: node(line_number), m_value(value) {}

	bool string_node::accept(visitor& visitor, value*& out_value) {
		LOG_NODE_NAME(string_node);
		return visitor.visit_keyword_string_node(*this, out_value);
	}

	std::string string_node::get_node_name() const {
		return "string_node";
	}

	const std::string& string_node::get_value() const {
		return m_value;
	}
}