
#include "source/compiler/compiler.h"

using namespace sigma::types;

/**
 * \brief Runs the compiler. Generates an exe from the given file (\a argv[1]) at the given location (\a argv[2])
 * \param argc Argument count
 * \param argv Argument values
 * \return Status code.
 */
i32 main(i32 argc, char* argv[]) {
	sigma::console::init();

	// required arguments:
	// executable path (automatic)
	// source file path
	// target executable directory
	// if(argc != 3) {
	// 	sigma::console::out
	// 		<< sigma::color::red
	// 		<< "invalid argument count\n"
	// 		<< sigma::color::white;
	// 	return 1;
	// }

	// initialize the compiler 
	sigma::compiler_settings settings;
	settings.optimization_level = sigma::optimization_level::high;
	settings.size_optimization_level = sigma::size_optimization_level::high;
	settings.vectorize = true;

	sigma::compiler compiler(settings);

	// check for compilation errors
	if(const auto compilation_result = compiler.compile(".\\test\\main.ch", ".\\test\\")) {
		compilation_result.value()->print();
		return 1;
	}

	return 0;
}
