#include "operator_binary.h"

namespace channel {
	operator_binary::operator_binary(
		const token_position& position,
		const node_ptr& left_expression_node,
		const node_ptr& right_expression_node
	) : node(position),
	m_left_expression_node(left_expression_node),
	m_right_expression_node(right_expression_node) {}

	const node_ptr& operator_binary::get_left_expression_node() const {
		return m_left_expression_node;
	}

	const node_ptr& operator_binary::get_right_expression_node() const {
		return m_right_expression_node;
	}
}