# scrabble-bf

Command-line tool to brute-force try all words from a word list on a
Scrabble/Wordfeud board and find the highest scoring move.

## Usage

    scrabble-bf <word-list-file> <scoring-file> <board-file> <tiles>

`<word-list-file>` should be the word list to use, with one (lower-case) word
per line. I recommend the [the Enable list][1].

[1]: http://www.puzzlers.org/pub/wordlists/enable1.txt

`<board-file>` should be a file containing the board layout, including all words
that have already been placed. It should be a plain text file, with one
character per tile:

| Character | Tile                         |
| --------- | ---------------------------- |
| `a`-`z`   | letter                       |
| `.`       | empty                        |
| `*`       | star tile in middle of board |
| `2`       | double letter value          |
| `3`       | tripple letter value         |
| `@`       | double word count            |
| `#`       | tripple word count           |

See the file [`example-board`](example-board) for an example board.

Start with a (copy of) an [empty board](empty-scrabble-board), and replace the
tiles with letters as you play.

`<scoring-file>` should be a file containing the values of all letters, and
(optionally) the bonus score for when all tiles are used. It should be a plain
text file, whith every line containing the letter followed by its value
(separated by whitespace). Use `?` to give the empty tile a value, and use `!`
to specify the value of a bonus. Scoring files [for English Scrabble][2] and
[for English Wordfeud][3] are included.

[2]: english-scrabble-scoring
[3]: english-wordfeud-scoring

`<tiles>` are the tiles you have available to place on the board. Use `?` for
the blank tile.

Example:

    scrabble-bf enable1.txt english-scrabble-scoring example-board 'ieds?pj'

## Output

The output of the tool is a list of all possible moves, sorted by their score.
Each line has five columns: The score, the row and column numbers of the first
letter of the word, the direction (horizontal or vertical), and the word.

For example:

    ...
    32  2 12 H edit
    32  1  1 V joe
    34 15  8 H deep
    35 15  5 H spade
    40 15  8 H deeps
    47 15  6 H jade

Here, the word 'jade', when placed horizontally starting at row 15 and column 6,
will give you 47 points.

## Copyright

Copyright &copy; 2016 Mara Bos

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.
