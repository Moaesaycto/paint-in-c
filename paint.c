// Assignment 1 20T3 COMP1511: CS Paint
// paint.c
//
// This program was written by STEPHEN LERANTGES (z5319858)
// on 2000-10-25
//
// Version 1.0.0 (2020-10-04): Assignment released.

#include <stdio.h>
#include <stdlib.h>

#define N_ROWS 20
#define N_COLS 36

// Shades (assuming your terminal has a black background).
#define BLACK 0
#define DRAW_LINE 1
#define FILL_RECTANGLE 2
#define CHANGE_COLOUR 3
#define COPY_PASTE 4
#define RECORD_MACRO 5
#define RUN_MACRO 6
#define SAVE_STATE 7

#define MACRO_MAX 50

#define DARK 1
#define GREY 2
#define LIGHT 3
#define WHITE 4

#define TRUE (1==1)
#define FALSE !TRUE

// Provided helper functions:
// Display the canvas.
void displayCanvas(int canvas[N_ROWS][N_COLS]);
// Clear the canvas by setting every pixel to be white.
void clearCanvas(int canvas[N_ROWS][N_COLS]);


// Tests to make sure at least one point is on the canvas
int inRange(int row1, int col1, int row2, int col2);
// Tests to make sure the point is on the canvas
int singleInRange(int row, int col);
// Returns the minimum value of two integers
int min(int num1, int num2);
// Draws a line
void createLine(int canvas[N_ROWS][N_COLS], int row1, int col1, int row2, int col2, int colour);
//Draws a filled rectangle
void filledRectangle(int canvas[N_ROWS][N_COLS], int row1, int col1, int row2, int col2, int colour);
// Changes colour of brush
int changeColour(int colour);
// Copy and paste
void copyPaste(int canvas[N_ROWS][N_COLS], int colour);
// Records a macro
void recordMacro(int macroLength, int *macroCommands, int canvas[N_ROWS][N_COLS], int colour);
// Plays back the recorded macro
void runMacro(int canvas[N_ROWS][N_COLS], int *macroCommands, int rowShift, int colShift, int colour);
// Saves the canvas as an array
void saveArray(int save[N_ROWS][N_COLS], int canvas[N_ROWS][N_COLS]);


// MAIN FUNCTION STARTS HERE

int main(void) {
    int canvas[N_ROWS][N_COLS];
    clearCanvas(canvas);
	int command, colour = BLACK;
	int macroCommands[MACRO_MAX + 1] = {0};
	int saveVal = 0;
	int save1[N_ROWS][N_COLS], save2[N_ROWS][N_COLS], save3[N_ROWS][N_COLS];
	int save4[N_ROWS][N_COLS], save5[N_ROWS][N_COLS];

	while (scanf("%d", &command) != EOF) {

		if (command == DRAW_LINE) {
            int row1, col1, row2, col2;
	        scanf("%d %d %d %d", &row1, &col1, &row2, &col2);
			createLine(canvas, row1, col1, row2, col2, colour);
		}

		if (command == FILL_RECTANGLE) {
            int row1, col1, row2, col2;
	        scanf("%d %d %d %d", &row1, &col1, &row2, &col2);
			filledRectangle(canvas, row1, col1, row2, col2, colour);
		}

		if (command == CHANGE_COLOUR) {
			colour = changeColour(colour);
		}

		if (command == COPY_PASTE) {
			copyPaste(canvas, colour);
		}
		int macroLength;
		if (command == RECORD_MACRO) {
			scanf("%d", &macroLength);
			macroCommands[0] = macroLength;
			int element = 1;
			while (element < 5 * macroLength + 1) {
				scanf("%d", &macroCommands[element]);
				element++;
			}
			runMacro(canvas, macroCommands, 0, 0, colour);
		}
		if (command == RUN_MACRO) {
			int rowShift, colShift;
			scanf("%d %d", &rowShift, &colShift);
			runMacro(canvas, macroCommands, rowShift, colShift, colour);
		}

		if (command == SAVE_STATE) {
			if (saveVal > 4) {
				saveVal = 4;
				saveArray(save1, save2);
				saveArray(save2, save3);
				saveArray(save3, save4);
				saveArray(save4, save5);
			}
			if (saveVal == 4) {
				saveArray(save5, canvas);
				saveVal++;
			}
			if (saveVal == 3) {
				saveArray(save4, canvas);
				saveVal++;
			}
			if (saveVal == 2) {
				saveArray(save3, canvas);
				saveVal++;
			}
			if (saveVal == 1) {
				saveArray(save2, canvas);
				saveVal++;
			}
			if (saveVal == 0) {
				saveArray(save1, canvas);
				saveVal++;
			}
		}
	}

	if (saveVal >= 1) {
		displayCanvas(save1);
		printf("\n");
	}
	if (saveVal >= 2) {
		displayCanvas(save2);
		printf("\n");
	}
	if (saveVal >= 3) {
		displayCanvas(save3);
		printf("\n");
	}
	if (saveVal >= 4) {
		displayCanvas(save4);
		printf("\n");
	}
	if (saveVal >= 5) {
		displayCanvas(save5);
		printf("\n");
	}

	displayCanvas(canvas);

    return 0;
}


// HELPER FUNCTIONS

// Checking to see if at least one point is in the range of the canvas
int inRange(int row1, int col1, int row2, int col2) {
	if (row1 < 0 || row1 > N_ROWS - 1) {
		if (row2 < 0 || row2 > N_ROWS - 1) {
			return FALSE;
		} 
		if (col2 < 0 || col2 > N_COLS - 1) {
			return FALSE;
		} 
	}
	if (col1 < 0 || col1 > N_COLS - 1) {
		if (row2 < 0 || row2 > N_ROWS - 1) {
			return FALSE;
		} 
		if (col2 < 0 || col2 > N_COLS - 1) {
			return FALSE;
		}
	}
    return TRUE;
}



// Checking to see if the position of ONE point is on the canvas
int singleInRange(int row, int col) {

	if (row < 0 || row > N_ROWS - 1) {
		return FALSE;
	}

	if (col < 0 || col > N_COLS - 1) {
		return FALSE;
	}

	return TRUE;
}



// Returns minimum value of 2 integers
int min(int num1, int num2) {

	if (num1 < num2) {
		return num1;
	}
	return num2;

}



// #####################################################
// ## MAIN COMMANDS FOR THE DRAWING TOOLS START HERE: ##
// #####################################################
//
// COMMAND 1 – STRAIGHT LINES (Horizontal, Vertical and Diagonal)

void createLine(int canvas[N_ROWS][N_COLS], int row1, 
				int col1, int row2, int col2, int colour) {

	if (!inRange(row1, col1, row2, col2)) {	
		return;
	}
	int count = 0;

    // Horizontal Line
	if (row1 == row2) {
		while (count <= abs(col2 - col1)) {
			if (singleInRange(row1, min(col1, col2) + count)) {
				canvas[row1][min(col1, col2) + count] = colour;
			}
			count++;
		}
	}
    // Vertical Line
	if (col1 == col2) {
		while (count <= abs(row2 - row1)) {
			if (singleInRange(min(row1, row2) + count, col1)) {
				canvas[min(row1, row2) + count][col1] = colour;
			}
			count++;
		}
	}

    // Diagonal Lines
    // Positive Gradient	
	if (col1 - col2 == row1 - row2) {
		while (count <= abs(row1 - row2)) {
			if (singleInRange(min(row1, row2) + count, min(col1, col2) + count)) {
				canvas[min(row1, row2) + count][min(col1, col2) + count] = colour;
			}
			count++;
		}
	}
    // Negative Gradient
	if (col1 - col2 == row2 - row1) {
		while (count <= abs(row1 - row2)) {
			if (singleInRange(min(row1, row2) + count, 
							  col1 + col2 - min(col1, col2) - count)) {
				canvas[min(row1, row2) + count]
					  [col1 + col2 - min(col1, col2) - count] = colour;
			}
			count++;
		}
	}
}



// COMMAND 2 – FILLED RECTANGLE
void filledRectangle(int canvas[N_ROWS][N_COLS], int row1, 
					 int col1, int row2, int col2, int colour) {
	if (!inRange(row1, col1, row2, col2)) {
		return;
	}
	int countRow = 0, countCol = 0;
	while (countRow <= abs(row1 - row2)) {
		while (countCol <= abs(col1 - col2)) {
			if (singleInRange(min(row1, row2) + countRow, min(col1, col2) + countCol)) {
				canvas[min(row1, row2) + countRow][min(col1, col2) + countCol] = colour;
			}
			countCol++;
		}
		countRow++;
		countCol = 0;
	}
}



// COMMAND 3 – CHANGING COLOUR OF BRUSH

int changeColour(int colour) {
	int colourNew;
	scanf("%d", &colourNew);
	if (colourNew < BLACK || colourNew > WHITE) {
		return colour;
	}
	return colourNew;
}



// COMMAND 4 – COPY AND PASTE

void copyPaste(int canvas[N_ROWS][N_COLS], int colour) {
	int rowTL, colTL, rowBR, colBR, destRow, destCol;
	scanf("%d %d %d %d %d %d", &rowTL, &colTL, &rowBR, &colBR, &destRow, &destCol);
	if (!inRange(rowTL, colTL, rowBR, colBR)) {
		return;
	}
	int countRow = 0, countCol = 0;
	int copyCanvas[N_ROWS][N_COLS];
	clearCanvas(copyCanvas);
	while (countRow <= abs(rowTL - rowBR)) {
		while (countCol <= abs(colTL - colBR)) {
			copyCanvas[countRow][countCol] = canvas[min(rowTL, rowBR) + countRow]
												   [min(colTL, colBR) + countCol];
			countCol++;
		}
		countRow++;
		countCol = 0;
	}
	countRow = 0;
	while (countRow <= abs(rowTL - rowBR)) {
		while (countCol <= abs(colTL - colBR)) {
			if (singleInRange(destRow + countRow, destCol + countCol)) {
				canvas[destRow + countRow][destCol + countCol] 
				= copyCanvas[countRow][countCol];
			}
			countCol++;
		}
		countRow++;
		countCol = 0;
	}
}


// COMMAND 6 – PLAY A MACRO

void runMacro(int canvas[N_ROWS][N_COLS], int *macroCommands, 
			  int rowShift, int colShift, int colour) {
	int count = 0, command, row1, col1, row2, col2;
    int macroLength = macroCommands[0];
	while (count < macroLength) {
		command = macroCommands[1 + 5 * count];
		row1 = macroCommands[2 + 5 * count];
		col1 = macroCommands[3 + 5 * count];
		row2 = macroCommands[4 + 5 * count];
		col2 = macroCommands[5 + 5 * count];
		if (command == DRAW_LINE) {
			createLine(canvas, row1 + rowShift, col1 + colShift, 
					   row2 + rowShift, col2 + colShift, colour);
		}
		if (command == FILL_RECTANGLE) {
			filledRectangle(canvas, row1 + rowShift, col1 + colShift, 
							row2 + rowShift, col2 + colShift, colour);
		}
		count++;
	}
}

void saveArray(int save[N_ROWS][N_COLS], int canvas[N_ROWS][N_COLS]) {

	int countRow = 0, countCol = 0;
	while (countRow < N_ROWS) {
		while (countCol < N_COLS) {
			save[countRow][countCol] = canvas[countRow][countCol];
			countCol++;
		}
		countCol = 0;
		countRow++;
	}
}



// Displays the canvas, by printing the integer value stored in
// each element of the 2-dimensional canvas array.
//
// You should not need to change the displayCanvas function.
void displayCanvas(int canvas[N_ROWS][N_COLS]) {
    int row = 0;
    while (row < N_ROWS) {
        int col = 0;
        while (col < N_COLS) {
            printf("%d ", canvas[row][col]);
            col++;
        }
        row++;
        printf("\n");
    }
}


// Sets the entire canvas to be blank, by setting each element in the
// 2-dimensional canvas array to be WHITE (which is #defined at the top
// of the file).
//
// You should not need to change the clearCanvas function.
void clearCanvas(int canvas[N_ROWS][N_COLS]) {
    int row = 0;
    while (row < N_ROWS) {
        int col = 0;
        while (col < N_COLS) {
            canvas[row][col] = WHITE;
            col++;
        }
        row++;
    }
}
