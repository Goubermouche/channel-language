#include "operator_modulo_node.h"

namespace channel {
	operator_modulo_node::operator_modulo_node(u64 line_number, node* left, node* right)
		: operator_binary(line_number, left, right) {}

	bool operator_modulo_node::accept(visitor& visitor, value*& out_value) {
		LOG_NODE_NAME(operator_modulo_node);
		return visitor.visit_operator_modulo_node(*this, out_value);
	}

	std::string operator_modulo_node::get_node_name() const {
		return "operator_modulo";
	}
}