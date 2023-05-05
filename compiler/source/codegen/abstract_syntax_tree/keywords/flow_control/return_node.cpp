#include "return_node.h"

namespace channel {
	return_node::return_node(
		const token_position& position,
		const node_ptr& return_expression_node
	) : node(position),
	m_return_expression_node(return_expression_node) {}

	bool return_node::accept(visitor& visitor, value_ptr& out_value) {
		LOG_NODE_NAME(return_node);
		return visitor.visit_return_node(*this, out_value);
	}

	void return_node::print(int depth, const std::wstring& prefix, bool is_last) {
		print_value(depth, prefix, "return statement", is_last);
		console::out << "\n";

		const std::wstring new_prefix = get_new_prefix(depth, prefix, is_last);

		// print the inner statement
		m_return_expression_node->print(depth + 1, new_prefix, true);
	}

	const node_ptr& return_node::get_return_expression_node() const {
		return m_return_expression_node;
	}
}