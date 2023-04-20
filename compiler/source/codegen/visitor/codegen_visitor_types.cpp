#include "codegen_visitor.h"

// signed integer
#include "../abstract_syntax_tree/keywords/types/signed_int/i8_node.h"
#include "../abstract_syntax_tree/keywords/types/signed_int/i16_node.h"
#include "../abstract_syntax_tree/keywords/types/signed_int/i32_node.h"
#include "../abstract_syntax_tree/keywords/types/signed_int/i64_node.h"
// unsigned integer
#include "../abstract_syntax_tree/keywords/types/unsigned_int/u8_node.h"
#include "../abstract_syntax_tree/keywords/types/unsigned_int/u16_node.h"
#include "../abstract_syntax_tree/keywords/types/unsigned_int/u32_node.h"
#include "../abstract_syntax_tree/keywords/types/unsigned_int/u64_node.h"
// floating point
#include "../abstract_syntax_tree/keywords/types/floating_point/f32_node.h"
#include "../abstract_syntax_tree/keywords/types/floating_point/f64_node.h"
// text
#include "../abstract_syntax_tree/keywords/types/text/char_node.h"
#include "../abstract_syntax_tree/keywords/types/text/string_node.h"

namespace channel {
	bool codegen_visitor::visit_keyword_i8_node(i8_node& node, value*& out_value) {
		out_value = new value("__i8", type::i8, llvm::ConstantInt::get(m_context, llvm::APInt(8, node.get_value(), true)));
		return true;
	}

	bool codegen_visitor::visit_keyword_i16_node(i16_node& node, value*& out_value) {
		out_value = new value("__i16", type::i16, llvm::ConstantInt::get(m_context, llvm::APInt(16, node.get_value(), true)));
		return true;
	}

	bool codegen_visitor::visit_keyword_i32_node(i32_node& node, value*& out_value) {
		out_value = new value("__i32", type::i32, llvm::ConstantInt::get(m_context, llvm::APInt(32, node.get_value(), true)));
		return true;
	}

	bool codegen_visitor::visit_keyword_i64_node(i64_node& node, value*& out_value) {
		out_value = new value("__i64", type::i64, llvm::ConstantInt::get(m_context, llvm::APInt(64, node.get_value(), true)));
		return true;
	}

	bool codegen_visitor::visit_keyword_u8_node(u8_node& node, value*& out_value) {
		out_value = new value("__u8", type::u8, llvm::ConstantInt::get(m_context, llvm::APInt(8, node.get_value(), false)));
		return true;
	}

	bool codegen_visitor::visit_keyword_u16_node(u16_node& node, value*& out_value) {
		out_value = new value("__u16", type::u16, llvm::ConstantInt::get(m_context, llvm::APInt(16, node.get_value(), false)));
		return true;
	}

	bool codegen_visitor::visit_keyword_u32_node(u32_node& node, value*& out_value) {
		out_value = new value("__u32", type::u32, llvm::ConstantInt::get(m_context, llvm::APInt(32, node.get_value(), false)));
		return true;
	}

	bool codegen_visitor::visit_keyword_u64_node(u64_node& node, value*& out_value) {
		out_value = new value("__u64", type::u64, llvm::ConstantInt::get(m_context, llvm::APInt(64, node.get_value(), false)));
		return true;
	}

	bool codegen_visitor::visit_keyword_f32_node(f32_node& node, value*& out_value) {
		out_value = new value("__f32", type::f32, llvm::ConstantFP::get(m_context, llvm::APFloat(node.get_value())));
		return true;
	}

	bool codegen_visitor::visit_keyword_f64_node(f64_node& node, value*& out_value) {
		out_value = new value("__f64", type::f64, llvm::ConstantFP::get(m_context, llvm::APFloat(node.get_value())));
		return true;
	}

	bool codegen_visitor::visit_keyword_char_node(char_node& node, value*& out_value) {
		out_value = new value("__char", type::char_type, llvm::ConstantInt::get(m_context, llvm::APInt(8, static_cast<u64>(node.get_value()), false)));
		return true;
	}

	bool codegen_visitor::visit_keyword_string_node(string_node& node, value*& out_value) {
		// note: + 1 is for the null termination character
		const u64 string_length = node.get_value().size() + 1;

		// allocate memory for the string literal on the stack
		llvm::ArrayType* array_type = llvm::ArrayType::get(type_to_llvm_type(type::char_type, m_context), string_length);
		llvm::AllocaInst* stack_string_literal = m_builder.CreateAlloca(array_type);

		// store the string literal in the allocated memory
		llvm::Constant* string_constant = llvm::ConstantDataArray::getString(m_context, node.get_value());
		m_builder.CreateStore(string_constant, stack_string_literal);

		// bit cast the pointer to the allocated memory into a char* (i8*)
		llvm::Value* string_literal_ptr = m_builder.CreateBitCast(stack_string_literal, type_to_llvm_type(type::char_pointer, m_context));

		out_value = new value("__string", type::char_pointer, string_literal_ptr);
		return true;
	}
}