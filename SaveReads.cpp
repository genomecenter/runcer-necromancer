// SaveReads.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <cassert>
#include <filesystem>
#include "zstr.hpp"

using namespace std;
namespace fs = std::filesystem;

typedef uint64_t index_t;


struct SequenceData {
	string letters;
	string quality;
};

index_t ReadNumberAfterChar(string& line, char ch, int len) {
	auto start_pos = line.find(ch) + 1;
	string num_str = line.substr(start_pos, len);
	return strtoul(num_str.data(), nullptr, 10);
}

index_t ParseIndex(string& line) {
	return ReadNumberAfterChar(line, 'C', 3) * 10000000000UL + ReadNumberAfterChar(line, 'R', 10);
}

class ReadIndex {
public:
	unordered_map<index_t, int64_t> index_map;
	ifstream file;

	ReadIndex(fs::path& path) {
		file.open(path, ios::binary);
		MakeIndex();
	}

	void MakeIndex() {
		string line;
		uint32_t line_number = 0;
		while (true)
		{
			bool read_status = static_cast<bool>(getline(file, line));
			if (!read_status) {
				break;
			}
			if ((line_number % 4) == 0) {
				streampos pos = file.tellg();
				index_map[ParseIndex(line)] = pos;
			}

			line_number++;
		}

		file.clear();
	}

	bool GetSequence(index_t index, SequenceData& result) {
		auto index_pair = index_map.find(index);

		if (index_pair != index_map.end()) {
			file.seekg(index_pair->second);
			getline(file, result.letters); // letters
			getline(file, result.quality); // +
			getline(file, result.quality); // actual quality
			file.clear();
			return true;
		} else {
			result.letters = "";
			result.quality = "";
			return false;
		}
	}
};


struct MatchStats {
	uint64_t not_found = 0;
	uint64_t found = 0;
};

template<class InStream, class OutStream>
MatchStats FixFile(InStream& input_file, OutStream& output_file, ReadIndex& index) {
	string line;
	bool read_status;
	SequenceData seq_data;
	MatchStats match_stats;

	while (true) {
		read_status = static_cast<bool>(getline(input_file, line));
		if (!read_status) {
			break;
		}
		output_file << line << "\n";

		bool found = index.GetSequence(ParseIndex(line), seq_data);
		if (found) {
			match_stats.found++;
		}
		else {
			match_stats.not_found++;
		}

		getline(input_file, line);
		output_file << seq_data.letters << line << "\n";

		getline(input_file, line);
		output_file << line << "\n";

		getline(input_file, line);
		output_file << seq_data.quality << line << "\n";
	}

	return match_stats;
}

int main(int argc, char* argv[])
{
	assert(argc > 1);
	fs::path filepath = argv[1];
	ReadIndex index(filepath);

	std::cout << "Index build" << std::endl;

	fs::path output_dir = "fixed";
	if (!fs::exists(output_dir)) {
		fs::create_directory(output_dir);
	}

	for (int i = 2; i < argc; i++) {
		filepath = argv[i];
		zstr::ifstream input_file(filepath.string());

		fs::path output_filepath = output_dir;
		output_filepath /= filepath;

		zstr::ofstream output_file(output_filepath.string());
		auto match_stats = FixFile(input_file, output_file, index);

		std::cout << filepath << " fixed" << std::endl;
		std::cout << match_stats.found << " reads matched" << std::endl;
		std::cout << match_stats.not_found << " reads didn't match" << std::endl;
		std::cout << std::endl;
	}
}
