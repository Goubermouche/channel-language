#include "declaration_node.h"

namespace channel {
	declaration_node::declaration_node(const std::string& name, node* expression)
		: m_name(name), m_expression(expression) {}

	const std::string& declaration_node::get_name() const {
		return m_name;
	}

	node* declaration_node::get_expression() const	{
		return m_expression;
	}
}