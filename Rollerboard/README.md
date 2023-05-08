# Rollerboard

The puzzle Roller-Board consists of a 2D rectangular grid of cells, each of which is labelled
either ‘0’ or ‘1’:<br />

![image](https://user-images.githubusercontent.com/98363908/168406277-7e7fd56a-59aa-458e-9596-eb5931fa2770.png)

The challenge is to roll one row or column at a time, so that the board is returned to its ‘correct’
state:<br />

![image](https://user-images.githubusercontent.com/98363908/168406294-69125258-943a-4c4b-b1e4-43cd1dcfaf23.png)

having all 1s on the top row, and every other cell being a 0. Each ‘move’ can be either:
• Roll a column one place up - i.e. the cells in this column all move up one, and the cell at
the top ‘rolls around’ and reappears at the bottom of this column.
• Roll a column one place down - i.e. the cells in this column all move down one, and the
cell at the bottom ‘rolls around’ and reappears at the top of this column.
• Roll a row one place left - i.e. the cells in this row all move left one, and the cell on the
left ‘rolls around’ and reappears on the right of this row.
• Roll a row one place right - i.e. the cells in this row all right one, and the cell on the right
‘rolls around’ and reappears on the left of this row.

Here’s an example of a board:<br />
![image](https://user-images.githubusercontent.com/98363908/168406342-575631b5-78b0-4f99-bbbb-d64d13c6f4ab.png)<br />
and how to solve it:<br />
![image](https://user-images.githubusercontent.com/98363908/168406350-79ecb645-c90b-4338-9c00-a2a880803be9.png)

This program reads in a roller-board file (specified on the command line), and shows the ‘moves’ to solve it. Such a file looks something like :<br />
![image](https://user-images.githubusercontent.com/98363908/168406378-20e8f67b-b90b-43cf-ab61-9a2aa9f0bb8f.png)

The first line has two numbers; the height of the board (number of rows) and then the width (number of columns).
In the remainder of the file, the number of 1s must be equal to the width of the board, and only the characters ‘0’ and ‘1’ are valid. You may assume that the maximum height and width of a board is 6.
