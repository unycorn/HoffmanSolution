// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <chrono>

FILE * output = fopen("C:/Users/djhar/Desktop/myfile.txt", "w");

// Six (6) means empty
const int orient[7][3] = {
	// x,  y,  z
	{ 2, 1, 0 }, //0
	{ 2, 0, 1 }, //1
	{ 1, 2, 0 }, //2
	{ 1, 0, 2 }, //3
	{ 0, 2, 1 }, //4
	{ 0, 1, 2 }, //5
	{ -1,-1,-1 }
};

int positions[3][3][3];

int mightFit(int(*arr)[3][3], int z, int y, int x) {
	// Test if empty block is passed in (will never happen)
	if (arr[z][y][x] == 6) {
		return -1;
	}

	// Test for same-length sides
	if (orient[arr[z][y][x]][0] == orient[arr[z][y][(x + 1) % 3]][0] || orient[arr[z][y][x]][0] == orient[arr[z][y][(x + 2) % 3]][0]) {
		return 0;
	}
	if (orient[arr[z][y][x]][1] == orient[arr[z][(y + 1) % 3][x]][1] || orient[arr[z][y][x]][1] == orient[arr[z][(y + 2) % 3][x]][1]) {
		return 0;
	}
	if (orient[arr[z][y][x]][2] == orient[arr[(z + 1) % 3][y][x]][2] || orient[arr[z][y][x]][2] == orient[arr[(z + 2) % 3][y][x]][2]) {
		return 0;
	}

	// Test for hanging blocks
	if (x == 0 && y == 0) {
		if (orient[arr[z][y][x]][0] > orient[arr[z - 1][y][x]][0] && orient[arr[z - 1][y][x + 1]][2] > orient[arr[z - 1][y][x]][2]) {
			return 0;
		}
		if (orient[arr[z][y][x]][1] > orient[arr[z - 1][y][x]][1] && orient[arr[z - 1][y + 1][x]][2] > orient[arr[z - 1][y][x]][2]) {
			return 0;
		}
	}
	if (x == 2 && y == 0) {
		if (orient[arr[z][y][x]][0] > orient[arr[z - 1][y][x]][0] && orient[arr[z - 1][y][x - 1]][2] > orient[arr[z - 1][y][x]][2]) {
			return 0;
		}
		if (orient[arr[z][y][x]][1] > orient[arr[z - 1][y][x]][1] && orient[arr[z - 1][y + 1][x]][2] > orient[arr[z - 1][y][x]][2]) {
			return 0;
		}
	}
	if (x == 2 && y == 2) {
		if (orient[arr[z][y][x]][0] > orient[arr[z - 1][y][x]][0] && orient[arr[z - 1][y][x - 1]][2] > orient[arr[z - 1][y][x]][2]) {
			return 0;
		}
		if (orient[arr[z][y][x]][1] > orient[arr[z - 1][y][x]][1] && orient[arr[z - 1][y - 1][x]][2] > orient[arr[z - 1][y][x]][2]) {
			return 0;
		}
	}
	if (x == 0 && y == 2) {
		if (orient[arr[z][y][x]][0] > orient[arr[z - 1][y][x]][0] && orient[arr[z - 1][y][x + 1]][2] > orient[arr[z - 1][y][x]][2]) {
			return 0;
		}
		if (orient[arr[z][y][x]][1] > orient[arr[z - 1][y][x]][1] && orient[arr[z - 1][y - 1][x]][2] > orient[arr[z - 1][y][x]][2]) {
			return 0;
		}
	}
	return 1;
}

/* Tests if blocks can work ignoring rows with empty
     spots and likely hanging blocks */
int doesFit(int (*arr)[3][3]) {
	char first, second, third;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			if (arr[x][y][0] != 6 && arr[x][y][1] != 6 && arr[x][y][2] != 6) {
				first = orient[ arr[0][y][x] ][2];
				second = orient[ arr[1][y][x] ][2];
				third = orient[ arr[2][y][x] ][2];
				if (first == second || second == third || third == first) {
					return 0;
				}
			}
		}
	}
	for (int y = 0; y < 3; y++) {
		for (int z = 0; z < 3; z++) {
			if (arr[0][y][z] != 6 && arr[1][y][z] != 6 && arr[2][y][z] != 6) {
				first = orient[arr[z][y][0]][0];
				second = orient[arr[z][y][1]][0];
				third = orient[arr[z][y][2]][0];
				if (first == second || second == third || third == first) {
					return 0;
				}
			}
		}
	}
	for (int z = 0; z < 3; z++) {
		for (int x = 0; x < 3; x++) {
			if (arr[x][0][z] != 6 && arr[x][1][z] != 6 && arr[x][2][z] != 6) {
				first = orient[arr[z][0][x]][1];
				second = orient[arr[z][1][x]][1];
				third = orient[arr[z][2][x]][1];
				if (first == second || second == third || third == first) {
					return 0;
				}
			}
		}
	}
	return 1;
}

void printArr(int(*arr)[3][3]) {
	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++) {
			for (int c = 0; c < 3; c++) {
				fprintf(output, "%d ", arr[a][b][c]);
			}
			fprintf(output, "\n");
		}
		fprintf(output, "\n");
	}
	fprintf(output, "------------\n\n");
}

void decrease(int &x, int &y, int &z) {
	if (x == 0) {
		x = 2;
		if (y == 0) {
			y = 2;
			if (z == 0) {
				z = 2;
				printf("We're in too deep!");
			}
			else {
				z--;
			}
		}
		else {
			y--;
		}
	}
	else {
		x--;
	}
}

int iterateSolutions() {
	int counter = 0;
	int fits;

	for (int z = 0; z < 3; z++) {
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				for (int i = 0; i < 6; i++) {
					// Kill infinite loop
					counter++;
					if (counter > 20000) {
						return 0;
					}

					positions[z][y][x] = i;
					printArr(positions);
					fits = mightFit(positions, z, y, x);
					if (fits == 0) {
						if (i == 5) {
							// Go back by 1 place
							positions[z][y][x] = 6;
							decrease(x, y, z);
							while (positions[z][y][x] == 5) {
								decrease(x, y, z);
							}
						}

						i = positions[z][y][x];
					}
					else {
						break;
					}
				}
			}
		}
	}
}

int main() {
	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++) {
			for (int c = 0; c < 3; c++) {
				positions[a][b][c] = 6;
			}
		}
	}

	// Run and measure time
	auto t1 = std::chrono::high_resolution_clock::now();
	iterateSolutions();
	auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	// Final Output
	if (doesFit(positions)) {
		printf("Solution found in %d ms!\n\n", duration/1000);

		// Output solution to file and console
		fprintf(output, "------------\nSOLUTION\n------------\n\n");
		for (int a = 0; a < 3; a++) {
			for (int b = 0; b < 3; b++) {
				for (int c = 0; c < 3; c++) {
					fprintf(output, "%d ", positions[a][b][c]);
					printf("%d ", positions[a][b][c]);
				}
				fprintf(output, "\n");
				printf("\n");
			}
			fprintf(output, "\n");
			printf("\n");
		}
		fprintf(output, "------------\n\n");
		printf("\n");
	}
	else {
		printf("Solution not found :(\n\n");
	}
	fclose(output);
	system("pause");
    return 0;
}

