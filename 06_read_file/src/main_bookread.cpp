#include <iostream>
#include <string>
#include <vector>
#include <fstream>

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
	Reads a vector of books from a text file.
	Note: the text file shall contain lines of text in pairs for the book name and authors
	E.g.
		BOOK 1
		AUTHOR 1
		BOOK 2
		AUTHOR 2
		BOOK 3
		AUTHOR 1, AUTHOR 2, AUTHOR 3
	@param file_name The name of the file to read from (must include path).
	@return Vector of books.
*/
std::vector<Book> readBooksFromTextFile(const std::string& file_name)
{
	std::vector<Book> results;
	std::ifstream input_stream; //declare input stream
	Book input_book;				//declare input object
	// TODO: BEGIN read the file -------------------------------------


	input_stream.open(file_name);	//open the input file


	if (input_stream.fail())	//check if the file can be opened
	{
		std::cout << "The file can't be opened";
		exit(1);
	}

	else
	{
		//read the file line by line
		while (std::getline(input_stream, input_book.name) && std::getline(input_stream, input_book.authors))
		{

			//check for empty lines
			if (input_book.authors.size() != 0 && input_book.name.size() != 0)
			{
				results.push_back(input_book);	//put the book in the data holder
			}

		}
	}


	// E.g. Book myBook;
	//		...
	//		results.emplace_back(myBook);

	// TODO: END read file and add to results vector ------------------
	input_stream.close(); //close the input stream

	return results;
}

int main()
{
	// Read a collection of books from a file.
	// Simple examples can be found everywhere.
	// E.g. https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c

	// Read the data
	std::string input_data("../../data/ermahgerd_berks.txt");
	std::cout << "Reading the data from " << input_data << std::endl;
	std::vector<Book> books_from_file = readBooksFromTextFile(input_data);

	// Print the data
	std::cout << "Here are all the books found in the data file..." << std::endl;
	for (auto book : books_from_file)
	{
		book.print();
	}

	return 0;
}