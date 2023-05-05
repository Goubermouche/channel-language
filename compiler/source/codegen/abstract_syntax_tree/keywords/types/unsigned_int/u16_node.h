#pragma once
#include "../../../node.h"

namespace channel {
	/**
	 * \brief AST node, represents a u16 literal.
	 */
	class u16_node : public node {
	public:
		u16_node(
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
