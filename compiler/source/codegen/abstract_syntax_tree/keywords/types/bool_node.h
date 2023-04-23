#pragma once
#include "../../node.h"

namespace channel {
	/**
	 * \brief AST node, represents the u8 type keyword.
	 */
	class bool_node : public node {
	public:
		bool_node(u64 line_number, bool value);
		bool accept(visitor& visitor, value*& out_value) override;
		std::string get_node_name() const override;

		bool get_value() const;
	private:
		bool m_value;
	};
}