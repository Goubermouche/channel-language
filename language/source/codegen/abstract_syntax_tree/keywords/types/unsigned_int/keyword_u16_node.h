#pragma once
#include "../../../node.h"

namespace channel {
	/**
	 * \brief AST node, represents the u16 type keyword.
	 */
	class keyword_u16_node : public node {
	public:
		keyword_u16_node(u64 value);
		value* accept(visitor& visitor) override;
		std::string get_node_name() const override;

		u64 get_value() const;
	private:
		u64 m_value;
	};
}