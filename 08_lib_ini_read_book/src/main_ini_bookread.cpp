// Includes

// C++ system headers
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// 3rd party libs headers
#include <SimpleIni.h>

#define book_name_key "name" //expected key for the name of the book
#define book_author_key "author" //expected key for the author of the book

/**
	Define a simple book.
*/
class Book
{
public:
	std::string name;
	std::string authors;

	void print()
	{
		std::cout << std::endl << this->name << std::endl;
		std::cout << "\t(by " << this->authors << ")" << std::endl;
	}
};

/**
	Reads a vector of books from an INI file.
	Note: the INI file shall contain a general books section with the number of books contained,
	and another INI section for each book, named [book.N], where N is a number/counter

	[books]
	count = 2;

	[book.1]
	name=The origin of truth
	author=Gusti
	[book.2]
	name  = Arhanghelul Raul
	author=Ovidiu Eftimie

	@param file_name The name of the file to read from (must include path).
	@return Vector of books.
*/
std::vector<Book> readBooksFromIniFile(const std::string& file_name)
{
	std::vector<Book> results;
	Book input_book;

	const char* current_section; //holds the current section 
	const char* input_name_value;	//holds the name of the book
	const char* input_author_value; //holds the author of the book

	CSimpleIniA input_stream;
	CSimpleIniA::TNamesDepend sections;
	CSimpleIniA::TNamesDepend keys;
	CSimpleIniA::TNamesDepend::const_iterator iterator_sections;
	CSimpleIniA::TNamesDepend::const_iterator iterator_keys;

	// TODO: BEGIN read the file -------------------------------------

	SI_Error file_error = input_stream.LoadFile(file_name.c_str());

	if (file_error < 0)
	{
		std::cout << "Can't open the file" << std::endl; //show a message if the file can't be opened
	}

	else 
	{
		//get all the sections
		input_stream.GetAllSections(sections);

		//sort the sections in the order they have been loaded
		sections.sort(CSimpleIniA::Entry::LoadOrder());

		//iterate through all the sections
		for (iterator_sections = sections.begin(); iterator_sections != sections.end(); ++iterator_sections)
		{
			current_section = iterator_sections->pItem; // for better understanding the code

			//get the value for the expected key
			input_name_value = input_stream.GetValue(current_section, book_name_key);
			input_author_value = input_stream.GetValue(current_section, book_author_key);

			if (input_name_value == nullptr || input_author_value == nullptr) //check if the section had the expected keys
			{
				continue;	//go to the next section if the current one didn't had one of the keys
			}

			else
			{
				input_book.name = input_name_value;	//asign name of the book
				input_book.authors = input_author_value;	//assign author of the book

				results.push_back(input_book);	//put the object in the container
			}

		}
		
		
	}



	// E.g. Book myBook;
	//		// build the section name (E.g. book.1)
	//		std::stringstream ss;
	//		ss << "book." << (i + 1);
	//		// Copy the stream to a string you can use
	//		std::string section_name(ss.str());

	//		...
	//		results.emplace_back(myBook);

	// TODO: END read file and add to results vector ------------------
	

	return results;
}

//prints the data from the INI file as it is (i got confused on the required printing format so i made this function too)
void print_INI_File(std::string file_name)
{
	CSimpleIniA input_stream;
	CSimpleIniA::TNamesDepend sections;
	CSimpleIniA::TNamesDepend keys;
	CSimpleIniA::TNamesDepend::const_iterator iterator_sections;
	CSimpleIniA::TNamesDepend::const_iterator iterator_keys;

	// TODO: BEGIN read the file -------------------------------------

	SI_Error file_error = input_stream.LoadFile(file_name.c_str());

	if (file_error < 0)
	{
		std::cout << "Can't open the file" << std::endl;
	}

	else
	{
		//load all the sections from the file
		input_stream.GetAllSections(sections);

		//sort the sections in the order they have been loaded
		sections.sort(CSimpleIniA::Entry::LoadOrder());

		//load every key from the current section
		for (iterator_sections = sections.begin(); iterator_sections != sections.end(); ++iterator_sections)
		{
			input_stream.GetAllKeys(iterator_sections->pItem,keys); //get all the keys

			std::cout << std::endl << "[" << iterator_sections->pItem << "]" << std::endl;	//print the current section

			keys.sort(CSimpleIniA::Entry::LoadOrder()); //sort the keys in the order they have been loaded
			

			for (iterator_keys = keys.begin(); iterator_keys != keys.end(); ++iterator_keys) //iterate through all the keys
			{
				//print the key and value loaded from the file
				std::cout << iterator_keys->pItem << " = " << input_stream.GetValue(iterator_sections->pItem,
					iterator_keys->pItem) << std::endl;
				
			}

		}

	}

}


int main()
{
	// Read a collection of books from an INI file.
	// Using the SimpleINI C++ Lib: https://github.com/brofield/simpleini

	// Read the data
	std::string input_data("../../data/ermahgerd_berks.ini");
	std::cout << "Reading the data from " << input_data << std::endl;
	std::vector<Book> books_from_file = readBooksFromIniFile(input_data);

	// Print the data
	std::cout << "Here are all the books found in the data file..." << std::endl;
	for (auto book : books_from_file)
	{
		book.print();
	}

	//print_INI_File(input_data);

	return 0;
}