#pragma once
#include "code_generator/abstract_syntax_tree/operators/binary/operator_binary_base.h"

namespace sigma {
	/**
	 * \brief AST node, represents the less than binary operator.
	 */
	class operator_less_than_node : public operator_binary_base {
	public:
		operator_less_than_node(
			const file_position& location,
			const node_ptr& left_expression_node,
			const node_ptr& right_expression_node
		);

		expected_value accept(
			code_generator& visitor,
			const code_generation_context& context
		) override;

		void print(
			u64 depth,
			const std::wstring& prefix,
			bool is_last
		) override;
	};
}
