/******************************************************************************
* Ex #0: This program get through the argument vector name of input file.
*          The program read the input file and creates a 2d-dynamic array.
* =============================================================================
* Written by  	: Muhammad Egbariya - 305140931 & Mohamad Amer - 315336115
* login	   		: muhammadeg / mohamadam
* Course/Year   : OS1 2019
* Year	   		: B
* =============================================================================
* Input: input file.
* -----------------------------------------------------------------------------
* Output: The row in the created dynamic array that all of it's elements are
*         are greater respectively from the elements on the same column
******************************************************************************/

//-------------------------- Include section ----------------------------------
#include <stdio.h>
#include <stdlib.h>

//=============================================================================
//------------------------- struct section ------------------------------------
typedef struct {
	int _num_of_lines;
	int **_the_data;
	int *_lines_len;
} Data;

//=============================================================================
//-------------------------- prototype section ----------------------------------
void read_data(FILE *input_File, Data *_data);
int max_line_number(const Data *_data);
void free_all(Data *_data);
void check_alloc_1d(int* array);
void check_alloc_2d(int** array);

//=============================================================================
//-------------------------- prototype function -------------------------------
int main(int argc, char **argv) {

	int Line_number = -1;
	FILE *input_File;

	if (argc == 2) {

		input_File = fopen(argv[1], "r");

		if (input_File == NULL) {
			fprintf(stderr, "%s", "can not open the File /n");
			exit(EXIT_FAILURE);
		}
		Data* _data = (Data*) malloc(sizeof(Data) * 1);
		read_data(input_File, _data);

		Line_number = max_line_number(_data);
		printf("%d\n", Line_number);

		free_all(_data);
		fclose(input_File);
	} else {
		fprintf(stderr, "%s", " can not open the input file /n ");
		exit(EXIT_FAILURE);

	}
	return EXIT_SUCCESS;
}
//=============================================================================
//-------------------------- read_data function ---------------------------
void read_data(FILE *input_File, Data *_data) {

	//==================== variables ======================
	int i = 1, row_size = 0, curr_col = 0, curr_row = 0;

	int * **_data_m = &_data->_the_data, * *_lines_len = &_data->_lines_len,
			*_num_of_lines = &(_data->_num_of_lines);

	//==================== allocate array's ===============
	_num_of_lines = 0;
	*_lines_len = malloc(sizeof(int*) * 0);
	check_alloc_1d(_lines_len);
	*_data_m = malloc(sizeof(int**) * 0);
	check_alloc_2d(_data_m);
	while (!feof(input_File)) {

		//===== reallocate array's to new size ===============
		if (curr_row != 0) {
			*_lines_len = realloc(*_lines_len, (2 * i) * sizeof(int*));
			*_data_m = realloc(*_data_m, (2 * i) * sizeof(int**));
		} else {
			*_lines_len = realloc(*_lines_len, 2 * i * sizeof(int*));
			*_data_m = realloc(*_data_m, 2 * i * sizeof(int**));
		}

		//================ fill lines_len array ==============
		fscanf(input_File, "%d", &(*_lines_len)[curr_row]);
		row_size = (*_lines_len)[curr_row];
		(*_data_m)[curr_row] = (int*) malloc(sizeof(int) * row_size);

		//================ fill data[curr_row] ===============
		for (curr_col = 0; curr_col < row_size; curr_col++) {
			fscanf(input_File, "%d", &(*_data_m)[curr_row][curr_col]);
		}
		i++;
		curr_row++;

	}

	//====== number of lines ======
	_data->_num_of_lines = curr_row;

}
//=============================================================================
//======================== find the the row with max values in col's ==========
int max_line_number(const Data *_data) {
	int bool_check = 0, max_line, row1, row2, col, bool = 0, max_row_size = 0;

	/* check each index in row with the same index in the other row and find
	 * the row with max value in index's */
	for (row1 = 0; row1 < _data->_num_of_lines; row1++) {
		for (row2 = 0; row2 < _data->_num_of_lines; row2++) {
			for (col = 0; col < _data->_lines_len[row1]; col++) {
				if (col < _data->_lines_len[row2]) {
					if (_data->_the_data[row2][col]
							<= _data->_the_data[row1][col]) {
						bool_check = 1;
						continue;
					} else {
						bool_check = 0;
						break;
					}
				}
				//=== if 1 index in the row smaller than the other index
				//=== stop and check other row's in the data
				if (bool_check == 0)
					break;

			}
			if (bool_check == 0)
				break;
		}
		if (bool_check == 1) {
			if (max_row_size < col) {
				bool = bool_check;
				max_line = row1;
				max_row_size = col;
			}
		}
	}
	//====== if the row didn't found, return -1============
	if (bool == 0)
		return -1;

	return max_line;
}

//=============================================================================
void check_alloc_1d(int* array) {
	if (array == NULL) {
		fputs("Cannot allocate memory", stderr);
		exit(EXIT_FAILURE);
	}
}

//=============================================================================
void check_alloc_2d(int** array) {
	if (array == NULL) {
		fputs("Cannot allocate memory", stderr);
		exit(EXIT_FAILURE);
	}
}

//=============================================================================
void free_all(Data * data) {
	int row;
	for (row = 0; row < data->_num_of_lines; row++)
		free(data->_the_data[row]);

	free(data->_the_data);
	free(data->_lines_len);
}
