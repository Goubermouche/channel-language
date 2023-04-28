#include "codegen_visitor.h"

#include "../abstract_syntax_tree/keywords/flow_control/return_node.h"
#include "../abstract_syntax_tree/keywords/flow_control/if_else_node.h"
#include "../abstract_syntax_tree/keywords/flow_control/while_node.h"
#include "../abstract_syntax_tree/keywords/flow_control/for_node.h"

namespace channel {
	bool codegen_visitor::visit_return_node(return_node& node, value*& out_value) {
		// evaluate the expression of the return statement
		value* return_value;
		if (!node.get_return_expression_node()->accept(*this, return_value)) {
			return false;
		}

		// get the return type of the current function
		const llvm::Function* parent_function = m_builder.GetInsertBlock()->getParent();
		const type function_return_type = m_functions[parent_function->getName().str()]->get_return_type();

		// upcast the return value to match the function's return type
		llvm::Value* upcasted_return_value;
		if(!cast_value(upcasted_return_value, return_value, function_return_type, node.get_declaration_line_number())) {
			return false;
		}

		// generate the LLVM return instruction with the upcasted value
		m_builder.CreateRet(upcasted_return_value);

		// return the value of the expression (use upcasted value's type)
		out_value = new value("__return", function_return_type, upcasted_return_value);
		return true;
	}

	bool codegen_visitor::visit_if_else_node(if_else_node& node, value*& out_value) {
        llvm::BasicBlock* entry_block = m_builder.GetInsertBlock();
        llvm::Function* parent_function = entry_block->getParent();
        // create the block for the end of the if-else statement
        llvm::BasicBlock* after_if_else_block = llvm::BasicBlock::Create(m_context, "", parent_function);

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
            condition_blocks[i] = llvm::BasicBlock::Create(m_context, "", parent_function);
        }

        // create branch blocks
        for (u64 i = 0; i < branch_nodes.size(); ++i) {
            branch_blocks[i] = llvm::BasicBlock::Create(m_context, "", parent_function);
        }

        // accept the first condition
        value* condition_value;
        if (!condition_nodes[0]->accept(*this, condition_value)) {
            return false;
        }

        // create a conditional branch based on the first condition
        m_builder.CreateCondBr(
            condition_value->get_value(),
            branch_blocks[0],
            condition_blocks.empty() ? (has_trailing_else ? branch_blocks.back() : after_if_else_block) : condition_blocks[0]
        );

        // process remaining conditions and create appropriate branches
        for (u64 i = 0; i < condition_node_count; ++i) {
            m_builder.SetInsertPoint(condition_blocks[i]);

            if (!condition_nodes[i + 1]->accept(*this, condition_value)) {
                return false;
            }

            m_builder.CreateCondBr(
                condition_value->get_value(),
                branch_blocks[i + 1],
                i < condition_node_count - 1 ? condition_blocks[i + 1] : branch_blocks.back()
            );
        }

        // save the previous scope
        scope* prev_scope = m_scope;

        // process branch nodes and create appropriate inner statements
        for (u64 i = 0; i < branch_nodes.size(); ++i) {
            m_builder.SetInsertPoint(branch_blocks[i]);
            m_scope = new scope(prev_scope);

            for (const auto& statement : branch_nodes[i]) {
                value* temp_statement_value;
                if (!statement->accept(*this, temp_statement_value)) {
                    return false;
                }
            }

            // create a branch to the end block
            m_builder.CreateBr(after_if_else_block);
        }

        // restore the previous scope and set the insert point to the end block
        m_scope = prev_scope;
        m_builder.SetInsertPoint(after_if_else_block);
        out_value = nullptr;
        return true;
	}

    bool codegen_visitor::visit_while_node(while_node& node, value*& out_value) {
        llvm::BasicBlock* entry_block = m_builder.GetInsertBlock();
        llvm::Function* parent_function = entry_block->getParent();
        // create the block for the end of the if-else statement
        llvm::BasicBlock* after_if_else_block = llvm::BasicBlock::Create(m_context, "", parent_function);

        llvm::BasicBlock* condition_block = llvm::BasicBlock::Create(m_context, "cond", parent_function);
        llvm::BasicBlock* loop_body_block = llvm::BasicBlock::Create(m_context, "body", parent_function);

        m_builder.CreateBr(condition_block);

        // condition block
        m_builder.SetInsertPoint(condition_block);

        // accept the condition node
        value* condition_value;
        if (!node.get_loop_condition_node()->accept(*this, condition_value)) {
            return false;
        }

        m_builder.CreateCondBr(
            condition_value->get_value(),
            loop_body_block,
            after_if_else_block
        );

        // accept all statements in the loop body
        m_builder.SetInsertPoint(loop_body_block);

        // save the previous scope
        scope* prev_scope = m_scope;
        m_scope = new scope(prev_scope);

        for(channel::node* n : node.get_statement_nodes()) {
            value* temp_value;
            if(!n->accept(*this, temp_value)) {
                return false;
            }
        }

        // restore the previous scope and set the insert point to the end block
        m_scope = prev_scope;

        m_builder.CreateBr(
            condition_block
        );
        
        m_builder.SetInsertPoint(after_if_else_block);
        out_value = nullptr;
        return true;
    }

    bool codegen_visitor::visit_for_node(for_node& node, value*& out_value) {
        return false;
    }
}