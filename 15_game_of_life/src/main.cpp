#include <iostream>
#include <vector>
#include <string>

#define ALIVE_CELL '1'
#define DEAD_CELL '0'



enum Shape
{
	BLOCK = 1, BOAT = 2, BLINKER = 3, BEACON = 4, PENTADECATHLON = 5, GLIDER = 6, PULSAR = 7

};

void getUserInput(size_t &input_Columns, size_t &input_Lines); //asks the user for the size of the matrix
void init_matrix(std::vector<std::string> &matrix_to_init, size_t lines, size_t columns);
void print_matrix(std::vector<std::string> &matrix);
int get_number_alive_neighbours_for_cell(std::vector<std::string>& matrix, size_t x, size_t y);
void create_next_generation(std::vector<std::string> &matrix);
int get_Tick_from_User();
void game_Master(std::vector<std::string> &matrix); //starts the game
Shape get_Shape_From_User(int &x, int &y);

void DrawShape(Shape desired_shape, int x, int y, std::vector<std::string>& matrix);


//functions that return a matrix with the specific shape

std::vector<std::string> draw_Block();
std::vector<std::string> draw_Boat();
std::vector<std::string> draw_Blinker();
std::vector<std::string> draw_Beacon();
std::vector<std::string> draw_Pentadecathlon();
std::vector<std::string> draw_Glider();
std::vector<std::string> draw_Pulsar();



int main()
{
	std::vector<std::string> matrix;
	size_t lines;
	size_t columns;


	getUserInput(columns, lines);
	
	init_matrix(matrix, lines, columns);

	game_Master(matrix);



	return 0;
}


void getUserInput(size_t &input_Columns, size_t &input_Lines)
{
	while(true)
	{
		int x = 0;
		int y = 0;

		std::cout << "Enter the the desired dimension in the format X Y (the dimension can't be smaller than 20x20" << std::endl;
		
		if (std::cin.peek() == '\n') //if the user dosen't enter input assign a default dimension
		{
			input_Columns = 20;
			input_Lines = 20;
			return;
		}
		
		
		std::cin >> x >> y;

		//validate the input
		if (std::cin.good())
		{
			if (x < 20 || y < 20)
			{
				std::cout << "The dimensions are too small!" << std::endl;
			}
			
			else
			{
				input_Columns = x;
				input_Lines = y;
				
				return;
			}
		}

		else
		{
			std::cout << "The input was invalid" << std::endl;
			std::cin.clear(); //clear the error flag

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore the  whole input stream
			
		}

	}

}

void init_matrix(std::vector<std::string> &matrix_to_init, size_t lines, size_t columns)
{
	std::string default_value = "0";
	//init lines
	for (size_t line_index = 0; line_index < (lines + 2); ++line_index) //adds an extra border of dead cells so the initial border cells can be treated
																		//like the others
	{
		matrix_to_init.push_back(default_value); 
		
		//init columns
		for (size_t index_columns = 1; index_columns < (columns + 2); ++index_columns) //index starts at 1 because we already have a 0 from the lines
		{
			matrix_to_init[line_index].append(default_value);
		}
	}
	std::cout << std::endl;
}


void print_matrix(std::vector<std::string>& matrix)
{
	//prints the matrix without the extra border of dead cells
	for (size_t index_lines = 1; index_lines < (matrix.size() - 1); ++index_lines)
	{
		for (size_t index_columns = 1; index_columns < (matrix[index_lines].size() - 1); ++index_columns)
		{
			std::cout << matrix[index_lines][index_columns] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

}

//x and y parameters represent the coordinates of the cell which we want to know the number of alive neighbours
int get_number_alive_neighbours_for_cell(std::vector<std::string>& matrix, size_t x, size_t y)
{
	int alive_neighbours = 0;


	//check the top neighbouring cells
	for (size_t current_x = x; current_x < (x + 3); ++current_x)
	{
		if (matrix[y - 1][current_x - 1] == ALIVE_CELL) //minus one so it starts from the top left corner of the cell and stops at the 
														//top right corner
		{
			
			++alive_neighbours;
		}
	}

	//check the bottom neighbouring cells
	for (size_t current_x = x; current_x < (x + 3); ++current_x)
	{
		if (matrix[y + 1][current_x - 1] == ALIVE_CELL)
		{
			++alive_neighbours;
		}
	}

	//check the cell to the left
	if (matrix[y][x - 1] == ALIVE_CELL)
	{
		++alive_neighbours;
	}

	//check the cell to the right
	if (matrix[y][x + 1] == ALIVE_CELL)
	{
		++alive_neighbours;
	}

	return alive_neighbours;
}


void create_next_generation(std::vector<std::string>& matrix)
{
	std::vector<std::string> next_generation_matrix = matrix;

	int alive_neighbours = 0;
	for (size_t line_index = 1; line_index < (matrix.size() - 1); ++line_index)
	{
		for (size_t column_index = 1; column_index < (matrix[line_index].size() - 1); ++column_index)
		{
			alive_neighbours = get_number_alive_neighbours_for_cell(matrix, column_index, line_index);


			//game rules
			if (matrix[line_index][column_index] == ALIVE_CELL && alive_neighbours < 2)
			{
				next_generation_matrix[line_index][column_index] = DEAD_CELL;
			}

			else if (matrix[line_index][column_index] == ALIVE_CELL && alive_neighbours > 3)
			{
				next_generation_matrix[line_index][column_index] = DEAD_CELL;
			}

			else if (matrix[line_index][column_index] == DEAD_CELL && alive_neighbours == 3)
			{
				next_generation_matrix[line_index][column_index] = ALIVE_CELL;
			}

		}
	}

	matrix = next_generation_matrix;
}


void DrawShape(Shape desired_shape, int x, int y, std::vector<std::string>& matrix)
{
	std::vector<std::string> shape;

	int current_x = x; 
	
	
		if (desired_shape == Shape::BLOCK)
		{
			shape = draw_Block();
		}

		else if (desired_shape == Shape::BOAT)

		{
			shape = draw_Boat();
		}

		else if (desired_shape == Shape::BLINKER)

		{
			shape = draw_Blinker();
		}

		else if (desired_shape == Shape::BEACON)
		{
			shape = draw_Beacon();
		}

		else if (desired_shape == Shape::PENTADECATHLON)
		{
			shape = draw_Pentadecathlon();
		}

		else if (desired_shape == Shape::GLIDER)
		{
			shape = draw_Glider();

		}else if (desired_shape == Shape::PULSAR)
		{
			shape = draw_Pulsar();
		}

		//write the shape to the matrix

		for (size_t index_lines = 0; index_lines < shape.size(); ++index_lines)
		{
			for (size_t index_colums = 0; index_colums < shape[0].size(); ++index_colums)
			{
				matrix[y][current_x] = shape[index_lines][index_colums];

				if((matrix[1].size() - 2) > current_x) //iteration ignores the extra set of dead cells
				{
					++current_x;
				}
					
			}

			current_x = x; //reset the x coordinate to the initial state

			if((matrix.size() - 2) > y)
			{
				++y; //go to the next line if the size allows it
			}
				

		}
	}




int get_Tick_from_User()
{
	int input_tick = 0;

	while (true)
	{
		std::cout << "Enter the number of ticks (how many generations will be created)" << std::endl;
		std::cin >> input_tick;

		if (std::cin.good())
		{
			return input_tick;
		}

		else
		{
			std::cout << "The input was invalid" << std::endl;

			std::cin.clear(); //clear the error flag

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore the  whole input stream

		}
	}

}


void game_Master(std::vector<std::string> &matrix)
{
	int input_tick = get_Tick_from_User();
	int current_tick = 0;
	int input_x, input_y;
	Shape input_Shape = get_Shape_From_User(input_x, input_y);

	//validate the input 
	while (input_x > (matrix[1].size() - 2) || input_y > (matrix.size() - 2) || input_x == 0 || input_y == 0)
	{
		std::cout << "Coordinates out of bounds" << std::endl;
		Shape input_Shape = get_Shape_From_User(input_x, input_y);
	}
	


	DrawShape(input_Shape, input_x, input_y, matrix);

	while (current_tick < input_tick)
	{
		std::cout  << std::endl << "Generation Number: " << current_tick << std::endl;

		print_matrix(matrix);
		create_next_generation(matrix);
		++current_tick;
	}

}

Shape get_Shape_From_User(int &x, int &y)
{
	int shape;


	while (true)
	{
		std::cout << "Enter the index for the desired shape and the coordinates in the format INDEX X Y)" << std::endl;
		std::cout << std::endl << "Available Shapes:" << std::endl;

		std::cout << "1 - BLOCK" << std::endl;
		std::cout << "2 - BOAT" << std::endl;
		std::cout << "3 - BLINKER" << std::endl;
		std::cout << "4 - BEACON" << std::endl;
		std::cout << "5 - PENTADECATHLON" << std::endl;
		std::cout << "6 - GLIDER" << std::endl;
		std::cout << "7 - PULSER" << std::endl;
					
		std::cin >> shape >> x >> y;

		if (std::cin.good())
		{
			switch (shape)
			{
			case Shape::BEACON:
				return BEACON;
				break;

			case Shape::BLINKER:
				return BLINKER;
				break;

			case Shape::BLOCK:
				return BLOCK;
				break;

			case Shape::BOAT:
				return BOAT;
				break;

			case Shape::GLIDER:
				return GLIDER;
				break;

			case Shape::PENTADECATHLON:
				return PENTADECATHLON;
				break;
			
			case Shape::PULSAR:
				return PULSAR;
				break;
			
			default:
				break;
			}
		}

		else
		{
			std::cout << "The input was invalid" << std::endl;

			std::cin.clear(); //clear the error flag

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore the  whole input stream

		}
	}
}



std::vector<std::string> draw_Block()
{
	std::vector<std::string> beacon_shape;
	std::string alive_Cell = "1";

	beacon_shape.push_back(alive_Cell);
	beacon_shape.push_back(alive_Cell);

	beacon_shape[0].append(alive_Cell);
	beacon_shape[1].append(alive_Cell);


	return beacon_shape;
}

std::vector<std::string> draw_Boat()
{
	std::vector<std::string> boat_shape;
	std::string first_line = "110";
	std::string second_line = "101";
	std::string third_line = "010";

	boat_shape.push_back(first_line);
	boat_shape.push_back(second_line);
	boat_shape.push_back(third_line);


	return boat_shape;
}


std::vector<std::string> draw_Blinker()
{
	std::vector<std::string> blinker_shape;
	std::string blink = "111";
	blinker_shape.push_back(blink);

	return blinker_shape;


}
std::vector<std::string> draw_Beacon()
{
	std::vector<std::string> beacon_shape;

	std::string first_line = "1100";
	std::string second_line = "1000";
	std::string third_line = "0001";
	std::string fourth_line = "0011";

	beacon_shape.push_back(first_line);
	beacon_shape.push_back(second_line);
	beacon_shape.push_back(third_line);
	beacon_shape.push_back(fourth_line);

	return beacon_shape;
	
}

std::vector<std::string> draw_Pentadecathlon()
{
	std::vector<std::string> Pentadecathlon_shape;

	std::string first_line = "0010000100"; //first line is also the third one
	std::string second_line = "1101111011";

	Pentadecathlon_shape.push_back(first_line);
	Pentadecathlon_shape.push_back(second_line);
	Pentadecathlon_shape.push_back(first_line);

	return Pentadecathlon_shape;
}

std::vector<std::string> draw_Glider()
{
	std::vector<std::string> Glider_shape;

	std::string first_line =  "010";
	std::string second_line = "001";
	std::string third_line =  "111";

	Glider_shape.push_back(first_line);
	Glider_shape.push_back(second_line);
	Glider_shape.push_back(third_line);

	return Glider_shape;
}

std::vector<std::string> draw_Pulsar()
{
	std::vector<std::string> pulsar;
	std::string first_line =       "00000100000100000"; //same as 14 and 15 same as 2
	std::string third_line =       "00000110001100000";
	std::string fourth_line =      "00000000000000000"; // same as 8 and 12
	std::string fifth_line =       "01110011011001110";
	std::string sixth_line =       "00010101010101000";
	std::string seventh_line =     "00000110001100000";
	std::string nineth_line =      "00000110001100000";
	std::string tenth_line =       "00010101010101000";
	std::string eleventh_line =    "01110011011001110";
	std::string thirteenth_line =  "00000110001100000";
	std::string fourteenth_line =  "00000110001100000";

	pulsar.push_back(first_line);
	pulsar.push_back(first_line);
	pulsar.push_back(third_line);
	pulsar.push_back(fourth_line);
	pulsar.push_back(fifth_line);
	pulsar.push_back(sixth_line);
	pulsar.push_back(seventh_line);
	pulsar.push_back(fourth_line);
	pulsar.push_back(nineth_line);
	pulsar.push_back(tenth_line);
	pulsar.push_back(eleventh_line);
	pulsar.push_back(fourth_line);
	pulsar.push_back(thirteenth_line);
	pulsar.push_back(first_line);
	pulsar.push_back(first_line);

	return pulsar;
}