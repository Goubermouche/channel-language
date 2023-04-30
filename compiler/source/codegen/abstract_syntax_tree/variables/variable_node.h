#pragma once
#include "../node.h"

namespace channel {
	/**
	 * \brief AST node, gets the value of a named variable
	 */
	class variable_node : public node {
	public:
		variable_node(u64 line_number, const std::string& variable_identifier);
		bool accept(visitor& visitor, value*& out_value) override;
		void print(int depth, const std::wstring& prefix, bool is_last) override;

		const std::string& get_variable_identifier() const;
	private:
		std::string m_variable_identifier;
	};
}