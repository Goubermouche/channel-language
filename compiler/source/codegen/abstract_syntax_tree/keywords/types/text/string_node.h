#pragma once
#include "../../../node.h"

namespace channel {
	/**
	 * \brief AST node, represents the i64 type keyword.
	 */
	class string_node : public node {
	public:
		string_node(u64 line_number, const std::string& value);
		bool accept(visitor& visitor, value*& out_value) override;
		std::string get_node_name() const override;

		const std::string& get_value() const;
	private:
		std::string m_value;
	};
}
