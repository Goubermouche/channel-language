#include "operator_equals_node.h"

namespace channel {
	operator_equals_node::operator_equals_node(
		u64 line_number,
		const node_ptr& left_expression_node,
		const node_ptr& right_expression_node
	) : operator_binary(
		line_number,
		left_expression_node,
		right_expression_node
	) {}

	bool operator_equals_node::accept(visitor& visitor, value_ptr& out_value) {
		LOG_NODE_NAME(operator_logical_disjunction_node);
		return visitor.visit_operator_equals_node(*this, out_value);
	}

	void operator_equals_node::print(int depth, const std::wstring& prefix, bool is_last) {
		print_value(depth, prefix, "binary operator", is_last);
		console::out
			<< '\''
			<< AST_NODE_OPERATOR_COLOR
			<< "=="
			<< color::white
			<< "'\n";

		const std::wstring new_prefix = get_new_prefix(depth, prefix, is_last);

		// print the inner statements
		get_left_expression_node()->print(depth + 1, new_prefix, false);
		get_right_expression_node()->print(depth + 1, new_prefix, true);
	}
}