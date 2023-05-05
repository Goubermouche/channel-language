#pragma once
#include "../../../node.h"

namespace channel {
	/**
	 * \brief AST node, represents a u64 literal.
	 */
	class u64_node : public node {
	public:
		u64_node(
			const token_position& position,
			u64 value
		);

		bool accept(visitor& visitor, value_ptr& out_value) override;
		void print(int depth, const std::wstring& prefix, bool is_last) override;

		u64 get_value() const;
	private:
		u64 m_value;
	};
}
