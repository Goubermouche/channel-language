#include "break_node.h"

namespace sigma {
	break_node::break_node(const file_position& location)
		: node(location) {}

	expected_value break_node::accept(code_generator& visitor, const code_generation_context& context) {
		return visitor.visit_break_node(*this, context);
	}

	void break_node::print(u64 depth, const std::wstring& prefix, bool is_last) {
		print_node_name(depth, prefix, "break statement", is_last);
		console::out << "\n";
	}
}