#include "operator_post_increment_node.h"

namespace channel {
	operator_post_increment_node::operator_post_increment_node(
		const token_position& position,
		const node_ptr& expression_node
	) : operator_unary_base(position, expression_node) {}

	acceptation_result operator_post_increment_node::accept(code_generator& visitor, const code_generation_context& context) {
		return visitor.visit_operator_post_increment_node(*this, context);
	}

	void operator_post_increment_node::print(u64 depth, const std::wstring& prefix, bool is_last) {
		print_node_name(depth, prefix, "unary operator", is_last);
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