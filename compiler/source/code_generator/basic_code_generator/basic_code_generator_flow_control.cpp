#include "basic_code_generator.h"

#include "code_generator/abstract_syntax_tree/keywords/flow_control/return_node.h"
#include "code_generator/abstract_syntax_tree/keywords/flow_control/if_else_node.h"
#include "code_generator/abstract_syntax_tree/keywords/flow_control/while_node.h"
#include "code_generator/abstract_syntax_tree/keywords/flow_control/for_node.h"
#include "code_generator/abstract_syntax_tree/keywords/flow_control/break_node.h"

namespace channel {
	acceptation_result basic_code_generator::visit_return_node(
		return_node& node, 
		const code_generation_context& context
	) {
		(void)context; // suppress C4100

		// evaluate the expression of the return statement
		acceptation_result return_value_result = node.get_return_expression_node()->accept(
			*this, 
			{}
		);

		if (!return_value_result.has_value()) {
			return return_value_result; // return on failure
		}

		// get the return type of the current function
		const llvm::Function* parent_function = m_llvm_context->get_builder().GetInsertBlock()->getParent();
		const type function_return_type = m_function_registry.get_function(
			parent_function->getName().str()
		)->get_return_type();

		// upcast the return value to match the function's return type
		llvm::Value* upcasted_return_value = cast_value(
			return_value_result.value(),
			function_return_type, 
			node.get_declared_position()
		);

		// generate the LLVM return instruction with the upcasted value
		m_llvm_context->get_builder().CreateRet(
			upcasted_return_value
		);

		// return the value of the expression (use the upcasted value's type)
		return std::make_shared<value>(
			"__return", 
			function_return_type, 
			upcasted_return_value
		);
	}

	acceptation_result basic_code_generator::visit_if_else_node(
		if_else_node& node, 
		const code_generation_context& context
	) {
		(void)context; // suppress C4100

		llvm::BasicBlock* entry_block = m_llvm_context->get_builder().GetInsertBlock();
		llvm::Function* parent_function = entry_block->getParent();
		llvm::BasicBlock* end_block = llvm::BasicBlock::Create(
			m_llvm_context->get_context(),
			"", 
			parent_function
		);

		// get condition and branch nodes
		const auto& condition_nodes = node.get_condition_nodes();
		const auto& branch_nodes = node.get_branch_nodes();

		// determine if there is a trailing else block
		const bool has_trailing_else = condition_nodes.back() == nullptr;
		const u64 condition_node_count = has_trailing_else ? condition_nodes.size() - 2 : condition_nodes.size() - 1;

		// initialize condition and branch blocks with their respective sizes
		std::vector<llvm::BasicBlock*> condition_blocks(condition_node_count);
		std::vector<llvm::BasicBlock*> branch_blocks(branch_nodes.size());

		// create condition blocks
		for (u64 i = 0; i < condition_node_count; ++i) {
			condition_blocks[i] = llvm::BasicBlock::Create(
				m_llvm_context->get_context(),
				"",
				parent_function
			);
		}

		// create branch blocks
		for (u64 i = 0; i < branch_nodes.size(); ++i) {
			branch_blocks[i] = llvm::BasicBlock::Create(
				m_llvm_context->get_context(), 
				"", 
				parent_function
			);
		}

		// accept the first condition
		acceptation_result condition_value_result = condition_nodes[0]->accept(
			*this,
			code_generation_context(type(type::base::boolean, 0))
		);

		if(!condition_value_result.has_value()) {
			return condition_value_result; // return on failure
		}

		// create a conditional branch based on the first condition
		m_llvm_context->get_builder().CreateCondBr(
			condition_value_result.value()->get_value(),
			branch_blocks[0],
			condition_blocks.empty() ? (has_trailing_else ? branch_blocks.back() : end_block) : condition_blocks[0]
		);

		// process remaining conditions and create appropriate branches
		for (u64 i = 0; i < condition_node_count; ++i) {
			m_llvm_context->get_builder().SetInsertPoint(condition_blocks[i]);

			condition_value_result = condition_nodes[i + 1]->accept(
				*this,
				code_generation_context(type(type::base::boolean, 0))
			);

			if(!condition_value_result.has_value()) {
				return condition_value_result; // return on failure
			}

			m_llvm_context->get_builder().CreateCondBr(
				condition_value_result.value()->get_value(),
				branch_blocks[i + 1],
				i < condition_node_count - 1 ? condition_blocks[i + 1] : branch_blocks.back()
			);
		}

		// save the previous scope
		scope_ptr prev_scope = m_scope;

		// process branch nodes and create appropriate inner statements
		for (u64 i = 0; i < branch_nodes.size(); ++i) {
			m_llvm_context->get_builder().SetInsertPoint(branch_blocks[i]);
			m_scope = std::make_unique<scope>(prev_scope);

			for (const auto& statement : branch_nodes[i]) {
				acceptation_result statement_result = statement->accept(
					*this,
					{}
				);

				if(!statement_result.has_value()) {
					return statement_result; // return on failure
				}
			}

			if(!m_llvm_context->get_builder().GetInsertBlock()->getTerminator()) {
				m_llvm_context->get_builder().CreateBr(end_block);
			}
		}

		// restore the previous scope and set the insert point to the end block
		m_scope = prev_scope;
		m_llvm_context->get_builder().SetInsertPoint(end_block);
		return nullptr;
	}

	acceptation_result basic_code_generator::visit_while_node(
		while_node& node, 
		const code_generation_context& context
	) {
		(void)context; // suppress C4100

		llvm::BasicBlock* entry_block = m_llvm_context->get_builder().GetInsertBlock();
		llvm::Function* parent_function = entry_block->getParent();

		llvm::BasicBlock* end_block = llvm::BasicBlock::Create(
			m_llvm_context->get_context(),
			"",
			parent_function
		);

		llvm::BasicBlock* condition_block = llvm::BasicBlock::Create(
			m_llvm_context->get_context(), 
			"", 
			parent_function
		);

		llvm::BasicBlock* loop_body_block = llvm::BasicBlock::Create(
			m_llvm_context->get_context(),
			"", 
			parent_function
		);

		m_llvm_context->get_builder().CreateBr(condition_block);

		// condition block
		m_llvm_context->get_builder().SetInsertPoint(condition_block);

		// accept all statements in the loop body
		m_llvm_context->get_builder().SetInsertPoint(loop_body_block);

		// accept the condition node
		acceptation_result condition_value_result = node.get_loop_condition_node()->accept(
			*this,
			code_generation_context(type(type::base::boolean, 0))
		);

		if(!condition_value_result.has_value()) {
			return condition_value_result; // return on failure
		}

		m_llvm_context->get_builder().CreateCondBr(
			condition_value_result.value()->get_value(),
			loop_body_block,
			end_block
		);

		// save the previous scope
		scope_ptr prev_scope = m_scope;
		m_scope = std::make_shared<scope>(prev_scope, end_block);

		for(channel::node* n : node.get_loop_body_nodes()) {
			acceptation_result statement_result = n->accept(
				*this, 
				{}
			);

			if(!statement_result.has_value()) {
				return statement_result; // return on failure
			}
		}

		// restore the previous scope and set the insert point to the end block
		m_scope = prev_scope;

		// only add a terminator block if we don't have one
		if (!m_llvm_context->get_builder().GetInsertBlock()->getTerminator()) {
			m_llvm_context->get_builder().CreateBr(condition_block);
		}

		m_llvm_context->get_builder().SetInsertPoint(end_block);
		return nullptr;
	}

	acceptation_result basic_code_generator::visit_for_node(
		for_node& node,
		const code_generation_context& context
	) {
		(void)context; // suppress C4100

		llvm::BasicBlock* entry_block = m_llvm_context->get_builder().GetInsertBlock();
		llvm::Function* parent_function = entry_block->getParent();

		llvm::BasicBlock* end_block = llvm::BasicBlock::Create(
			m_llvm_context->get_context(), 
			"", 
			parent_function
		);

		llvm::BasicBlock* condition_block = llvm::BasicBlock::Create(
			m_llvm_context->get_context(),
			"",
			parent_function
		);

		llvm::BasicBlock* increment_block = llvm::BasicBlock::Create(
			m_llvm_context->get_context(), 
			"", 
			parent_function
		);

		llvm::BasicBlock* loop_body_block = llvm::BasicBlock::Create(
			m_llvm_context->get_context(), 
			"", 
			parent_function
		);

		// save the previous scope
		scope_ptr prev_scope = m_scope;
		m_scope = std::make_shared<scope>(prev_scope, end_block);

		// create the index expression
		acceptation_result index_expression_result = node.get_loop_initialization_node()->accept(
			*this,
			{}
		);

		if (!index_expression_result.has_value()) {
			return index_expression_result; // return on failure
		}

		m_llvm_context->get_builder().CreateBr(condition_block);

		// accept the condition block
		m_llvm_context->get_builder().SetInsertPoint(condition_block);

		acceptation_result condition_value_result = node.get_loop_condition_node()->accept(
			*this, 
			{}
		);

		if (!condition_value_result.has_value()) {
			return condition_value_result;
		}

		// check if the conditional operator evaluates to a boolean
		if (condition_value_result.value()->get_type().get_base() != type::base::boolean ||
			condition_value_result.value()->get_type().is_pointer()) {
			return std::unexpected(
				error::emit<4010>(
					node.get_declared_position(),
					condition_value_result.value()->get_type()
				)
			);
		}

		m_llvm_context->get_builder().CreateCondBr(
			condition_value_result.value()->get_value(),
			loop_body_block,
			end_block
		);

		// create the increment block
		m_llvm_context->get_builder().SetInsertPoint(increment_block);
		for (channel::node* n : node.get_post_iteration_nodes()) {
			acceptation_result statement_result = n->accept(*this, {});
			if (!statement_result.has_value()) {
				return statement_result; // return on failure
			}
		}

		m_llvm_context->get_builder().CreateBr(condition_block);

		// create the loop body block
		m_llvm_context->get_builder().SetInsertPoint(loop_body_block);

		// accept all inner statements
		for (channel::node* n : node.get_loop_body_nodes()) {
			acceptation_result statement_result = n->accept(*this, {});
			if (!statement_result.has_value()) {
				return statement_result; // return on failure
			}
		}

		// restore the previous scope and set the insert point to the end block
		m_scope = prev_scope;

		// only add a terminator block if we don't have one
		if (!m_llvm_context->get_builder().GetInsertBlock()->getTerminator()) {
			m_llvm_context->get_builder().CreateBr(increment_block);
		}

		m_llvm_context->get_builder().SetInsertPoint(end_block);
		return nullptr;
	}

	acceptation_result basic_code_generator::visit_break_node(
		break_node& node, 
		const code_generation_context& context
	) {
		(void)context; // suppress C4100

		llvm::BasicBlock* end_block = m_scope->get_loop_end_block();
		if (end_block == nullptr) {
			// emit an error if there's no enclosing loop to break from
			return std::unexpected(
				error::emit<4011>(
					node.get_declared_position()
				)
			);
		}

		// only add a terminator block if we don't have one
		if (!m_llvm_context->get_builder().GetInsertBlock()->getTerminator()) {
			m_llvm_context->get_builder().CreateBr(end_block);
		}

		// create a new basic block for the remaining loop body and set it as the current insert point
		llvm::Function* parent_function = end_block->getParent();
		llvm::BasicBlock* continue_block = llvm::BasicBlock::Create(
			m_llvm_context->get_context(), 
			"", 
			parent_function
		);

		m_llvm_context->get_builder().SetInsertPoint(continue_block);
		return nullptr;
	}
}