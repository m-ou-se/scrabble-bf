# scrabble-bf

Command-line tool to brute-force try all words from a word list on a
Scrabble/Wordfeud board and find the highest scoring move.

## Usage

    scrabble-bf <word-list-file> <board-file> <tiles>

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

See the file [`empty-board`](empty-board) for an empty Wordfeud board. Start
with a (copy of) an empty board, and replace the tiles with letters as you play.

`<tiles>` are the tiles you have available to place on the board. Use `?` for
the blank tile.

Example:

    scrabble-bf enable1.txt my-board 'tu?zdse'

## Output

The output of the tool is a list of all possible moves, sorted by their score.
Each line has five columns: The score, the row and column numbers of the first
letter of the word, the direction (horizontal or vertical), and the word.

For example:

    ...
    18  9  4 H unite
    19  6  4 H neat
    23 10  8 V auntie
    27 10 11 V minute
    ...

Here, the word 'minute', when placed vertically starting at row 10 and column
11, will give you 27 points.

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
