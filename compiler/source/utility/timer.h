#pragma once
#include "utility/macros.h"

namespace sigma {
	class timer {
	public:
		void start();
		double elapsed() const;
	private:
		std::chrono::steady_clock::time_point m_start;
	};
}
