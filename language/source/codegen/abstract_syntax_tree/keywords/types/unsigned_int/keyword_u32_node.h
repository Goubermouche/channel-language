#pragma once
#include "../../../node.h"

namespace channel {
	/**
	 * \brief AST node, represents the u32 type keyword.
	 */
	class keyword_u32_node : public node {
	public:
		keyword_u32_node(u64 line_index, u64 value);
		bool accept(visitor& visitor, value*& out_value) override;
		std::string get_node_name() const override;

		u64 get_value() const;
	private:
		u64 m_value;
	};
}
