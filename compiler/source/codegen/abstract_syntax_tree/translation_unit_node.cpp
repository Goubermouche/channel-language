#include "translation_unit_node.h"

namespace channel {
	translation_unit_node::translation_unit_node(const std::vector<node*>& nodes)
		: node(0), m_nodes(nodes) {}

	bool translation_unit_node::accept(visitor& visitor, value*& out_value) {
		LOG_NODE_NAME(function_call_node);
		return visitor.visit_translation_unit_node(*this, out_value);
	}

	void translation_unit_node::print(int depth, const std::wstring& prefix, bool is_last) {
		print_value(depth, prefix, L"TranslationUnitDecl", "\n", is_last);
		const std::wstring new_prefix = get_new_prefix(depth, prefix, is_last);

		for (u64 i = 0; i < m_nodes.size(); ++i) {
			m_nodes[i]->print(depth + 1, new_prefix, i == m_nodes.size() - 1);
		}
	}

	const std::vector<node*>& translation_unit_node::get_nodes() const {
		return m_nodes;
	}
}