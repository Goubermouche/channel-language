#include "char_node.h"

namespace sigma {
	char_node::char_node(
		const file_position& location,
		char value
	) : node(location),
	m_value(value)	{}

	expected_value char_node::accept(code_generator& visitor, const code_generation_context& context)	{
		return visitor.visit_keyword_char_node(*this, context);
	}

	void char_node::print(u64 depth, const std::wstring& prefix, bool is_last) {
		print_node_name(
			depth,
			prefix, 
			"character literal", 
			is_last
		);

		console::out
			<< '\''
			<< AST_NODE_TYPE_COLOR
			<< "char"
			<< color::white 
			<< "' '"
			<< AST_NODE_TEXT_LITERAL_COLOR
			<< escape_string(std::string(1, m_value))
			<< color::white
			<< "'\n";
	}

	char char_node::get_value() const {
		return m_value;
	}
}