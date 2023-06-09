#pragma once
#include "utility/macros.h"
#include "llvm_wrappers/type.h"
#include "utility/containers/compile_time_hash_map.h"

template<>
struct std::formatter<sigma::type> {
	static auto parse(format_parse_context& ctx) {
		return ctx.begin();
	}

	static auto format(sigma::type e, format_context& ctx) {
		return format_to(ctx.out(), "{}", e.to_string());
	}
};

template<>
struct std::formatter<sigma::token> {
	static auto parse(format_parse_context& ctx) {
		return ctx.begin();
	}

	static auto format(sigma::token e, format_context& ctx) {
		return format_to(ctx.out(), "{}", token_to_string(e));
	}
};

namespace sigma {
	class diagnostic_message {
	public:
		virtual ~diagnostic_message() = default;
		virtual void print() const = 0;

		diagnostic_message(std::string message, u64 code);
	protected:
		std::string m_message;
		u64 m_code;
	};
}