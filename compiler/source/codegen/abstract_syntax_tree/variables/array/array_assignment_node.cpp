#include "array_assignment_node.h"

namespace channel {
	array_assignment_node::array_assignment_node(u64 line_number, node* array_base, const std::vector<node*>& index_nodes, node* expression_node)
		: node(line_number), m_array_base_node(array_base), m_array_element_index_nodes(index_nodes), m_expression_node(expression_node) {}

	bool array_assignment_node::accept(visitor& visitor, value*& out_value) {
		LOG_NODE_NAME(array_assignment_node);
		return visitor.visit_array_assignment_node(*this, out_value);
	}

	void array_assignment_node::print(int depth, const std::wstring& prefix, bool is_last) {
		print_value(depth, prefix, L"ArrayAssignment", "\n", is_last);
		const std::wstring new_prefix = get_new_prefix(depth, prefix, is_last);

		// print the inner statements
		m_array_base_node->print(depth + 1, new_prefix, false);

		for (u64 i = 0; i < m_array_element_index_nodes.size(); ++i) {
			m_array_element_index_nodes[i]->print(depth + 1, new_prefix, false);
		}

		m_expression_node->print(depth + 1, new_prefix, true);
	}

	node* array_assignment_node::get_array_base_node() const	{
		return m_array_base_node;
	}

	const std::vector<node*>& array_assignment_node::get_array_element_index_nodes() const {
		return m_array_element_index_nodes;
	}

	node* array_assignment_node::get_expression_node() const {
		return m_expression_node;
	}
}