#pragma once
#include "code_generator/abstract_syntax_tree/node.h"

namespace sigma {
	/**
	 * \brief AST node, represents a string literal.
	 */
	class string_node : public node {
	public:
		string_node(
			const file_position& location,
			const std::string& value
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

		const std::string& get_value() const;
	private:
		std::string m_value;
	};
}
