#pragma once
#include "codegen/abstract_syntax_tree/node.h"

namespace channel {
	/**
	 * \brief AST node, represents a boolean literal.
	 */
	class bool_node : public node {
	public:
		bool_node(
			const token_position& position,
			bool value
		);

		bool accept(visitor& visitor, value_ptr& out_value, codegen_context context) override;
		void print(int depth, const std::wstring& prefix, bool is_last) override;

		bool get_value() const;
	private:
		bool m_value;
	};
}