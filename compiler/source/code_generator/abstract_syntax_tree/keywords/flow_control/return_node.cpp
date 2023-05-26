#include "return_node.h"

namespace channel {
	return_node::return_node(
		const token_position& position,
		const node_ptr& return_expression_node
	) : node(position),
	m_return_expression_node(return_expression_node) {}

	acceptation_result return_node::accept(code_generator& visitor, const code_generation_context& context) {
		return visitor.visit_return_node(*this, context);
	}

	void return_node::print(u64 depth, const std::wstring& prefix, bool is_last) {
		print_node_name(depth, prefix, "return statement", is_last);
		console::out << "\n";

		const std::wstring new_prefix = get_new_prefix(depth, prefix, is_last);

		// print the inner statement
		m_return_expression_node->print(depth + 1, new_prefix, true);
	}

	const node_ptr& return_node::get_return_expression_node() const {
		return m_return_expression_node;
	}
}