#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

int letter_score(char c) {
	// Letter scores used by Wordfeud for English.
	switch (c) {
		case 'a': return 1;
		case 'b': return 4;
		case 'c': return 4;
		case 'd': return 2;
		case 'e': return 1;
		case 'f': return 4;
		case 'g': return 3;
		case 'h': return 4;
		case 'i': return 1;
		case 'j': return 10;
		case 'k': return 5;
		case 'l': return 1;
		case 'm': return 3;
		case 'n': return 1;
		case 'o': return 1;
		case 'p': return 4;
		case 'q': return 10;
		case 'r': return 1;
		case 's': return 1;
		case 't': return 1;
		case 'u': return 2;
		case 'v': return 4;
		case 'w': return 4;
		case 'x': return 8;
		case 'y': return 4;
		case 'z': return 10;
		default: return 0;
	}
}

char tile(std::vector<std::string> const & board, int row, int col) {
	if (row < 0 || std::size_t(row) >= board.size()) return ' ';
	if (col < 0 || std::size_t(col) >= board[row].size()) return ' ';
	return board[row][col];
}

int scrabble_score(
	std::vector<std::string> const & board,
	std::vector<std::string> const & words,
	std::string letters,
	std::string const & word,
	int row, int col, bool vert
) {
	int score = 0; // For the word itself, to be multiplied by the multiplier
	int multiplier = 1;
	int extra_score = 0; // For crossing words and the used-all-letters-bonus.

	if (vert) {
		if (std::isalpha(tile(board, row - 1, col))) return -1;
		if (std::isalpha(tile(board, row + word.size(), col))) return -1;
	} else {
		if (std::isalpha(tile(board, row, col - 1))) return -1;
		if (std::isalpha(tile(board, row, col + word.size()))) return -1;
	}

	bool attached = false;
	bool placed_tile = false;

	for (char ch : word) {
		char b = board[row][col];
		int tile_score = letter_score(ch);
		int tile_word_multiplier = 1;
		if (b == ch) {
			attached = true;
		} else if (std::isalpha(b)) {
			return -1;
		} else {
			auto i = letters.find(ch);
			if (i == std::string::npos) {
				i = letters.find('?');
				if (i == std::string::npos) return -1;
				tile_score = 0;
			}
			letters[i] = ' ';
			placed_tile = true;
			if (b == '*') attached = true;
			if (b == '@') tile_word_multiplier = 2;
			if (b == '#') tile_word_multiplier = 3;
			if (b == '2') tile_score *= 2;
			if (b == '3') tile_score *= 3;
			std::string word_across;
			if (vert) {
				word_across.reserve(board[row].size());
				int s = col;
				while (std::isalpha(tile(board, row, s - 1))) --s;
				while (s < col) word_across += board[row][s++];
				word_across += ch;
				while (std::isalpha(tile(board, row, ++s))) word_across += tile(board, row, s);
			} else {
				word_across.reserve(board.size());
				int s = row;
				while (std::isalpha(tile(board, s - 1, col))) --s;
				while (s < row) word_across += board[s++][col];
				word_across += ch;
				while (std::isalpha(tile(board, ++s, col))) word_across += tile(board, s, col);
			}
			if (word_across.size() > 1) {
				if (!std::binary_search(words.begin(), words.end(), word_across)) return -1;
				attached = true;
				int word_across_score = 0;
				for (char c : word_across) word_across_score += letter_score(c);
				word_across_score += tile_score - letter_score(ch);
				extra_score += word_across_score * tile_word_multiplier;
			}
		}
		score += tile_score;
		multiplier *= tile_word_multiplier;
		++(vert ? row : col);
	}

	if (!attached) {
		// Word doesn't use any existing tiles, isn't adjacent to any existing tile,
		// nor does it go over a '*' tile.
		return -1;
	}

	if (!placed_tile) {
		// Didn't place any of new letters, the word was already completely on the board.
		return -1;
	}

	if (letters.find_first_not_of(' ') == std::string::npos) {
		// Add 40 points when all our letters are used.
		extra_score += 40;
	}

	return score * multiplier + extra_score;
}

int main(int argc, char * * argv) {

	if (argc != 4) {
		std::clog << "Usage: " << argv[0] << " <word-list-file> <board-file> <tiles>" << std::endl;
		return 1;
	}

	std::vector<std::string> words;

	{
		std::ifstream f(argv[1]);
		for (std::string line; std::getline(f, line); ) {
			if (line.back() == '\n') line.pop_back();
			if (!line.empty()) words.push_back(std::move(line));
		}
		if (f.bad()) {
			std::clog << "Error while reading word list." << std::endl;
			return 1;
		}
	}

	std::sort(words.begin(), words.end());

	std::vector<std::string> board;

	{
		std::ifstream f(argv[2]);
		for (std::string line; std::getline(f, line); ) {
			if (line.back() == '\n') line.pop_back();
			if (!line.empty()) board.push_back(std::move(line));
			if (board.size() > 1) {
				if (board.back().size() != board.front().size()) {
					std::clog << "Error: board not rectangular." << std::endl;
					return 1;
				}
			}
		}
		if (f.bad()) {
			std::clog << "Error while reading board." << std::endl;
			return 1;
		}
	}

	std::string letters = argv[3];

	int const width = board.front().size();
	int const height = board.size();

	std::multimap<int, std::tuple<int, int, bool, char const *>> possibilities;

	for (auto const & word : words) {
		for (int row = 0; row < height; ++row)
		for (int col = 0; col <= width - int(word.size()); ++col) {
			int s = scrabble_score(board, words, letters, word, row, col, false);
			if (s >= 0) possibilities.insert({s, {row, col, false, word.c_str()}});
		}
		for (int row = 0; row <= height - int(word.size()); ++row)
		for (int col = 0; col < width; ++col) {
			int s = scrabble_score(board, words, letters, word, row, col, true);
			if (s >= 0) possibilities.insert({s, {row, col, true, word.c_str()}});
		}
	}

	for (auto const & p : possibilities) {
		std::cout
			<< std::setw(4) << p.first << ' ' // score
			<< std::setw(2) << (std::get<0>(p.second) + 1) << ' ' // row
			<< std::setw(2) << (std::get<1>(p.second) + 1) << ' ' // column
			<< (std::get<2>(p.second) ? 'V' : 'H') << ' ' // orientation
			<< std::get<3>(p.second) << std::endl;
	}
}
