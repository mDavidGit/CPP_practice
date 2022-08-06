#include <iostream>
#include <string>
#include <vector>

/**
	Define a simple matrix.
*/
class Matrix
{
private:
	size_t column_count;
	size_t line_count;
	char **matrix; //define the matrix

	// TODO: store the data
	// hints: you can use std::string, std::vectors + string, char**, vector<vector<char>>, etc
public:
	Matrix(size_t numColumnsX, size_t numLinesY)
		// TODO: add functionality
	{
		column_count = numColumnsX;
		line_count = numLinesY;

		matrix = new char*[line_count]; // build the lines

		for (size_t i = 0; i < numLinesY; ++i)
		{
			matrix[i] = new char[column_count]; // build the columns
		}
	}

	// Set an entire line
	void setLine(size_t line_number, const std::string& data)
	{
		if(line_number < line_count) //check if line_number is within the matrix bounds
		{
			for (size_t current_column = 0; current_column < column_count; current_column++) //iterate through all the columns of a line
				
			{
				if (current_column < data.size()) //check if the current cell is within the size of the input string
					{
						matrix[line_number][current_column] = data[current_column]; //set the specified data cell by cell
					}

				else
					{
						matrix[line_number][current_column] = '-'; //if the input string is too short a default value will be assigned							  //to the current cell
					}
			
			}
			
		}
		
	}

	//returns the value of a given cell
	char getCellXY(size_t x, size_t y)
	{
		if ( (y < line_count) && (x < column_count) )
		{
			
			return matrix[y][x]; //if the input is valid return the specified value
		}
		else
		{
			return '\0'; //if the input is out of the matrix bounds return null 
		}
		
	}
	//OPTIONAL
	// 
	// 
	//char getCellXY(size_t x, size_t y, char cell_content)
	//{
	//	// TODO: add functionality
	//	return 0;
	//}

	/**
		Sets the cell content for a specific cell identified by its coordinates (X and Y)
		@param x The horizontal coordinate of the cell to set. Index starts at zer0.
		@param y The vertical coordinate of the cell to set. Index starts at zer0.
		@param cell_content A cell char to set.

		E.g.
		 X axis: 0 -------- ... -------> Width
		 Y axis
		 0       0,0  1,0                W-1, 0
		 |       0,1  1,1                     
		 |       0,2                      
		 .                             
		 .                             
		 .                             
		 |       0, H-1     ...          W-1, H-1           
		 V

		 Height
	*/
	void setCellXY(size_t x, size_t y, char cell_content)
	{
		// TODO: add functionality

		if ( (y < line_count) && (x < column_count) ) //check if the given coordinates are valid
		{
			matrix[y][x] = cell_content; //assign the value to the specified cell
		}
			
	}

	void print()
	{
		// print all lines and columns
		// TODO: add functionality
		for (size_t y = 0; y < line_count; ++y) //iterate through all the lines
		{
			for (size_t x = 0; x < column_count; ++x) //iterate through all the columns
			{
				std::cout << matrix[y][x];	//print the value
			}
			std::cout << std::endl;	//formatting
		}

		std::cout << std::endl;	//formatting
	}


	//free the memory space after the object goes out of scope
	~Matrix()
	{
		for (size_t index = line_count; index < line_count; ++index)
		{
			delete[] matrix[index];
		}

		delete[] matrix;

	}

};

int main()
{
	// Create a matrix with the width of 20 and height of 10.
	Matrix matrix(20, 10);


	matrix.setLine(0, "X-----X----X-----XX-");
	matrix.setLine(1, "--X-----------------");
	matrix.setLine(2, "-----X--------------");
	matrix.setLine(3, "--------X-----------");
	matrix.setLine(4, "-----------X--------");
	matrix.setLine(5, "--------------X-----");
	matrix.setLine(6, "-----------------X--");
	matrix.setLine(7, "-------------------X");
	matrix.setLine(8, "------------------X-");
	matrix.setLine(9, "-----------------X--");

	matrix.print();
	// Would print
/*
X-----X----X-----XX-
--X-----------------
-----X--------------
--------X-----------
-----------X--------
--------------X-----
-----------------X--
-------------------X
------------------X-
-----------------X--
*/
	matrix.setCellXY(2, 1, '-');
	matrix.print();
	// Would print
/*
X-----X----X-----XX-
--------------------
-----X--------------
--------X-----------
-----------X--------
--------------X-----
-----------------X--
-------------------X
------------------X-
-----------------X--
*/

	matrix.setCellXY(3, 7, 'O');
	matrix.print();
/*
X-----X----X-----XX-
--------------------
-----X--------------
--------X-----------
-----------X--------
--------------X-----
-----------------X--
---O---------------X
------------------X-
-----------------X--
*/


	//prints the value of the specified cell
	std::cout << "The value is: "  <<  matrix.getCellXY(17, 0);

	// This should silently fail (not trigger an error): cell Y=11 invalid due to limited height.
	matrix.setCellXY(3, 11, 'O');
	return 0;
}