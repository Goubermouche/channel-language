#pragma once
#include "../../../node.h"

namespace channel {
	/**
	 * \brief AST node, represents the i64 type keyword.
	 */
	class i64_node : public node {
	public:
		i64_node(u64 line_number, i64 value);
		bool accept(visitor& visitor, value*& out_value) override;
		void print(int depth, const std::wstring& prefix, bool is_last) override;

		i64 get_value() const;
	private:
		i64 m_value;
	};
}
