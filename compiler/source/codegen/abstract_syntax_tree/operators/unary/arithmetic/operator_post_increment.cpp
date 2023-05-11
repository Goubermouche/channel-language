#include "operator_post_increment.h"

namespace channel {
	operator_post_increment::operator_post_increment(
		const token_position& position,
		const node_ptr& expression_node
	) : operator_unary(position, expression_node) {}

	acceptation_result operator_post_increment::accept(visitor& visitor, const codegen_context& context) {
		return visitor.visit_operator_post_increment_node(*this, context);
	}

	void operator_post_increment::print(int depth, const std::wstring& prefix, bool is_last) {
		print_value(depth, prefix, "unary operator", is_last);
		console::out
			<< '\''
			<< AST_NODE_OPERATOR_COLOR
			<< "postfix"
			<< color::white
			<< "' '"
			<< AST_NODE_OPERATOR_COLOR
			<< "++"
			<< color::white
			<< "'\n";

		const std::wstring new_prefix = get_new_prefix(depth, prefix, is_last);

		// print the inner statement
		get_expression_node()->print(depth + 1, new_prefix, true);
	}
}
