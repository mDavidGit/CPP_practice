#pragma once

// header for vector
#include <vector>
// header for utility
#include <utility>
// SFML headers
#include "SFML/Graphics.hpp"

#include <SimpleINI.h>

class SfmlApp
{
private:
	sf::RenderWindow window_;
	std::vector<sf::Vertex> cell_vertices_;
	std::pair<unsigned int, unsigned int> cell_size_;
	std::pair<unsigned int, unsigned int> world_size_;

	sf::Color living_cell_color_;
	sf::Color dead_cell_color_;
	sf::Text gui_text_;
	sf::Font font_;

	bool simulation_active = true;
	unsigned duration_in_millis_between_updates = 1000;

	std::vector<unsigned int> values_from_INI;

	//game of life methods
	std::vector<std::string> matrix;
	void update_matrix_cell_and_UI(unsigned cell_x, unsigned cell_y);
	int get_number_alive_neighbours_for_cell(size_t x, size_t y);
	void create_next_generation();
	void update_UI_with_next_generation();

	//INI file reading methods
	void assign_Default_Values();
	bool parse_INI_Input_to_Int(const char* input_Value);
	void read_Values_From_INI(std::string ini_file_path);
	void assign_INI_Values();
	


public:
	/**
		Constructor.
		can provide some initialization / sizes to use.
		These could be moved to a separate configuration object at some point.
	*/
	SfmlApp(
		std::pair<unsigned int, unsigned int> window_size, 
		std::pair<unsigned int, unsigned int> cell_size
	);

	//by calling this constructor we load the configuration from a specified INI file
	SfmlApp(std::string ini_file_path);

	~SfmlApp();

	void init();
	void run();
	void render();

	void addVertexQuad(unsigned cell_x, unsigned cell_y, unsigned width, unsigned height);
	void setCellColor(unsigned cell_x, unsigned cell_y, sf::Color color);
	void updateWorld();
};