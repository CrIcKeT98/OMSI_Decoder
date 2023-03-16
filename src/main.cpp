#include "Decoder.h"
#include "OMSI_utils.h"
#include "boost/program_options.hpp"

void checkExtension(std::string& s) {
	std::string buff(s.end() - 4, s.end());

	if (buff != ".o3d")
		s += ".o3d";
}

int main(int argc, char* argv[]) {
	using namespace std;
	namespace opt = boost::program_options;

	opt::options_description desc("Usage");

	desc.add_options()
		("p", opt::value<std::string>(), "Path to .o3d file")
		("help", "Print help message");

	if (argc < 2) {
		cout << desc << endl;
		return 0;
	}

	opt::variables_map vm;
	opt::store(opt::parse_command_line(argc, argv, desc), vm);
	opt::notify(vm);

	if (vm.count("help")) {
		cout << desc << endl;
		return 0;
	}

	string file { vm["p"].as<string>() };
	checkExtension(file);

	if (!checkFile(file))
		return -1;

	if (!decodeFile(file))
		return -1;
}