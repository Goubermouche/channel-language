#pragma once
#include "operator_binary.h"

namespace channel {
	class operator_addition_node : public operator_binary {
	public:
		operator_addition_node(node* left, node* right);
		void accept(visitor& visitor) override;
	};
}
