#include "lexer.h"
#include "utility/filesystem.h"

namespace sigma {
	token_list::token_list(std::vector<token_data> tokens)
		: m_tokens(tokens) {}


	void token_list::print_tokens() const {
		for (const token_data& t : m_tokens) {
			console::out << std::left << std::setw(40) << token_to_string(t.get_token());

			if (!t.get_value().empty()) {
				// the value string may contain escape sequences 
				console::out << escape_string(t.get_value());
			}

			console::out << '\n';
		}
	}

	const token_data& token_list::get_token() {
		m_peek_token_index++;
		return  m_tokens[m_main_token_index++];
	}

	const token_data& token_list::peek_token() {
		return m_tokens[m_peek_token_index++];
	}

	void token_list::synchronize_indices() {
		m_peek_token_index = m_main_token_index;
	}

	error_result lexer::set_source_filepath(
		const filepath& path
	) {
		// check if the file exists, and if it has been opened successfully
		auto file_read_result = detail::read_file(path);
		if(!file_read_result.has_value()) {
			return file_read_result.error();
		}

		m_source_path = path;
		m_accessor = detail::string_accessor(file_read_result.value());
		return {};
	}

	token_list lexer::get_token_list() const {
		return { m_tokens };
	}
}