#include "sfml_app.h"

#include <iostream>
#include <thread>

#define ALIVE_CELL '1'
#define DEAD_CELL '0'


SfmlApp::SfmlApp(
	std::pair<unsigned int, unsigned int> window_size,
	std::pair<unsigned int, unsigned int> cell_size
)
	: window_({ window_size.first, window_size.second }, "My window")
	, cell_size_(cell_size)
{
}

SfmlApp::~SfmlApp()
{
}

std::string getMessageForActiveStatus(bool active)
{
	if (active)
	{
		return "Simulation is ACTIVE. Press <SPACE> to toggle the simulation. Press <ESC> to exit.";
	}

	return "Simulation is PAUSED. Press <SPACE> to toggle the simulation. Press <ESC> to exit.";
}

void SfmlApp::init()
{
	/*this->living_cell_color_ = sf::Color(Alive_Color[0], Alive_Color[1], Alive_Color[2]);
	this->dead_cell_color_ = sf::Color(Dead_Color[0], Dead_Color[1], Dead_Color[2]);*/

	if (font_.loadFromFile("../../data/OpenSans-Regular.ttf"))
	{
		gui_text_.setFont(font_);
		gui_text_.setString(getMessageForActiveStatus(true));
		gui_text_.move(10, 2);
		gui_text_.setCharacterSize(24);
		gui_text_.setOutlineColor(sf::Color::White);
	}
	else
	{
		// error...cannot render GUI
	}

	// Note: you can also override the window size after it is initialized.
	// This could be useful to allow changing the resolution and the world size after the initialization
	// but is not really a priority for this exercise.
	//this->window.setSize(sf::Vector2u(1200, 600));

	// Initialize the vertices
	size_t max_width = static_cast<size_t>(window_.getView().getSize().x);
	size_t max_height = static_cast<size_t>(window_.getView().getSize().y);

	// Store the world size for later use.
	world_size_.first = max_width / cell_size_.first - 1;
	world_size_.second = max_height / cell_size_.second - 1;

	// Initialize the world map with vertices.
	for (size_t cell_y = 0; cell_y < max_height / cell_size_.first - 1; cell_y++)
	{
		for (size_t cell_x = 0; cell_x < max_width / cell_size_.second - 1; cell_x++)
		{
			this->addVertexQuad(cell_x, cell_y, cell_size_.first, cell_size_.second);
		}
	}

	size_t win_width = static_cast<size_t>(window_.getView().getSize().x);
	size_t win_height = static_cast<size_t>(window_.getView().getSize().y);

	size_t number_of_cells_x = (win_width / cell_size_.first);  
	size_t number_of_cells_y = (win_height / cell_size_.second); 


	//init the matrix based on how many squares are going to fit in the window
	//also we add an extra layer of dead cells so the border cells can be treated like the others
	std::string default_value = "0";
	for (size_t index_lines = 0; index_lines < (number_of_cells_y + 1 ); ++index_lines)
	{
		matrix.push_back(default_value);
		for (size_t index_columns = 1; index_columns < (number_of_cells_x + 1); ++index_columns)
		{
			matrix[index_lines].append(default_value);
		}
	}
}

void SfmlApp::run()
{
	unsigned time_elapsed_since_update = 0;
	// TODO: it would be nice to make this configurable in the future.
	//duration_in_millis_between_updates = 1000; default value

	// run the program as long as the window is open
	while (window_.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window_.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				window_.close();
			}

			// quick close via the ESC key.
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window_.close();
					return;
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					// Toggle simulation
					simulation_active = !simulation_active;
					gui_text_.setString(getMessageForActiveStatus(simulation_active));
				}
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					size_t view_width = static_cast<size_t>(window_.getView().getSize().x);
					size_t view_height = static_cast<size_t>(window_.getView().getSize().y);
					size_t win_width = static_cast<size_t>(window_.getSize().x);
					size_t win_height = static_cast<size_t>(window_.getSize().y);

					unsigned clicked_cell_x = event.mouseButton.x * view_width / (cell_size_.first * win_width);
					unsigned clicked_cell_y = event.mouseButton.y * view_height / (cell_size_.second * win_height);

					// TODO: maybe update a world matrix?
					update_matrix_cell_and_UI(clicked_cell_x, clicked_cell_y);
				}
			}
		}

		if (time_elapsed_since_update > duration_in_millis_between_updates)
		{
			updateWorld();
			time_elapsed_since_update -= duration_in_millis_between_updates;
		}

		// clear the window with black color
		window_.clear(sf::Color::Black);

		render();

		// end the current frame
		window_.display();

		// don't consume too many cpu cycles
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(10));
		if (simulation_active)
		{
			time_elapsed_since_update += 10;
		}
	}
}

void SfmlApp::setCellColor(unsigned cell_x, unsigned cell_y, sf::Color color)
{
	auto index = (cell_y * this->world_size_.first + cell_x) * 4;
	if (index >= cell_vertices_.size())
	{
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		cell_vertices_[index + i].color = color;
	}
}

void SfmlApp::addVertexQuad(unsigned cell_x, unsigned cell_y, unsigned width, unsigned height)
{
	sf::Vertex topLeft;
	sf::Vertex topRight;
	sf::Vertex bottomLeft;
	sf::Vertex bottomRight;

	float pixel_x = static_cast<float>(cell_x * width);
	float pixel_y = static_cast<float>(cell_y * height);

	topLeft.position = { pixel_x,			 pixel_y };
	topRight.position = { pixel_x + width,   pixel_y };
	bottomLeft.position = { pixel_x,           pixel_y + height };
	bottomRight.position = { pixel_x + width,   pixel_y + height };

	topLeft.color = dead_cell_color_;
	topRight.color = dead_cell_color_;
	bottomLeft.color = dead_cell_color_;
	bottomRight.color = dead_cell_color_;

	cell_vertices_.push_back(topLeft);
	cell_vertices_.push_back(bottomLeft);
	cell_vertices_.push_back(bottomRight);
	cell_vertices_.push_back(topRight);
}

void SfmlApp::render()
{
	// draw everything here...
	window_.draw(cell_vertices_.data(), cell_vertices_.size(), sf::Quads);

	// Add any GUI info.
	window_.draw(gui_text_);
}

void SfmlApp::updateWorld()
{
	// TODO: feel free to add function arguments as deemed necessary.
	if (simulation_active)
	{
		create_next_generation();
		update_UI_with_next_generation();
	}

}


void SfmlApp::update_matrix_cell_and_UI(unsigned cell_x, unsigned cell_y)
{
		// by incrementing the coordinates with 1 we avoid messing the extra layer of dead cells in the matrix 

	if (cell_x < (matrix[1].size() - 2) && cell_y < (matrix.size() - 2)) //by pressing the black lines on the UI the user can mess the extra border of cells, so
																		// we don't let him do that
	{
		if (matrix[cell_y + 1][cell_x + 1] == DEAD_CELL)
		{
			matrix[cell_y + 1][cell_x + 1] = ALIVE_CELL;
			setCellColor(cell_x, cell_y, living_cell_color_);
		}

		else
		{
			matrix[cell_y + 1][cell_x + 1] = DEAD_CELL;
			setCellColor(cell_x, cell_y, dead_cell_color_);
		}
	}

}

int SfmlApp::get_number_alive_neighbours_for_cell(size_t x, size_t y)
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

void SfmlApp::create_next_generation()
{
	std::vector<std::string> next_generation_matrix = matrix;

	int alive_neighbours = 0;
	for (size_t line_index = 1; line_index < (matrix.size() - 1); ++line_index)
	{
		for (size_t column_index = 1; column_index < (matrix[line_index].size() - 1); ++column_index)
		{
			alive_neighbours = get_number_alive_neighbours_for_cell(column_index, line_index);


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


void SfmlApp::update_UI_with_next_generation()
{
	//coordinates for the UI
	int y_ui = 0; 
	int x_ui = 0;

	//index_lines and index_columns are the coordinates for the matrix
	for (size_t index_lines = 1; index_lines < (matrix.size() ); ++index_lines) 
	{
		
		for (size_t index_columns = 1; index_columns < (matrix[index_lines].size()); ++index_columns)
		{
			if (matrix[index_lines][index_columns] == ALIVE_CELL)
			{
				setCellColor(x_ui, y_ui, living_cell_color_);
			}

			else
			{
				setCellColor(x_ui, y_ui, dead_cell_color_);
			}
			++x_ui;
		}
		y_ui++;
		x_ui = 0; //after we are done writing all the columns of a line, go to the next line and reset the column coordinate to 0
	}
}


SfmlApp::SfmlApp(std::string ini_file_path)
{
	
	read_Values_From_INI(ini_file_path);

	assign_INI_Values();


	window_.create(sf::VideoMode(world_size_.first, world_size_.second), "Ini constructor");
	
}



bool SfmlApp::parse_INI_Input_to_Int(const char* value)
{
	char* end = nullptr;
	int parsed_Value;

	parsed_Value =  strtol(value, &end, 10);

	//if a value is not a numeric one, return false
	if (value == end)
	{	
		return false;
	}
	
	else
	{
		values_from_INI.push_back(parsed_Value);
		return true;
	}
	
}

//default values in case something goes wrong when we try to get the info from the file
void SfmlApp::assign_Default_Values()
{
	world_size_.first = 1200;
	world_size_.second = 600;

	cell_size_.first = 5;
	cell_size_.second = 5;

	duration_in_millis_between_updates = 1000;

	living_cell_color_ = sf::Color(40, 160, 20);
	dead_cell_color_ = sf::Color(25, 23, 21);
}

void SfmlApp::read_Values_From_INI(std::string ini_file_path)
{
	CSimpleIniA input_stream;
	CSimpleIniA::TNamesDepend sections;
	CSimpleIniA::TNamesDepend keys;
	CSimpleIniA::TNamesDepend::const_iterator iterator_sections;
	CSimpleIniA::TNamesDepend::const_iterator iterator_keys;

	const char* current_section = nullptr;
	const char* current_key = nullptr;
	const char* value = nullptr;

	SI_Error file_error = input_stream.LoadFile(ini_file_path.c_str());

	if (file_error < 0)
	{
		//if the file can't be loaded assign some default values

		assign_Default_Values();
	}

	else
	{
		input_stream.GetAllSections(sections);
		sections.sort(CSimpleIniA::Entry::LoadOrder());


		for (iterator_sections = sections.begin(); iterator_sections != sections.end(); ++iterator_sections)
		{
			current_section = iterator_sections->pItem;

			input_stream.GetAllKeys(current_section, keys);
			keys.sort(CSimpleIniA::Entry::LoadOrder());

			for (iterator_keys = keys.begin(); iterator_keys != keys.end(); ++iterator_keys) //iterate through all the keys
			{
				current_key = iterator_keys->pItem;

				value = input_stream.GetValue(current_section, current_key);

				if (!parse_INI_Input_to_Int(value))
				{
					//if the parser indentified a non valid input abort the reading an assign default values
					assign_Default_Values();
					return;
				}

				
			}

			
		}

	}
}
//assign the values from the INI file
void SfmlApp::assign_INI_Values()
{
	if  (values_from_INI.size() < 11)
	{
		assign_Default_Values();
	
	}
	else
	{
		world_size_.first = values_from_INI[0];
		world_size_.second = values_from_INI[1];

		cell_size_.first = values_from_INI[2];
		cell_size_.second = values_from_INI[3];

		duration_in_millis_between_updates = values_from_INI[4];

		living_cell_color_ = sf::Color(values_from_INI[5], values_from_INI[6], values_from_INI[7]);
		dead_cell_color_ = sf::Color(values_from_INI[8], values_from_INI[9], values_from_INI[10]);

		
		
	}
}