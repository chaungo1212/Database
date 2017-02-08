// Database.h - Contains declaration of Function class  
#pragma once  

#include <string>
#include <vector>

#ifdef DATABASE_EXPORTS  
#define DATABASE_API __declspec(dllexport)   
#else  
#define DATABASE_API __declspec(dllimport)   
#endif  

namespace Database
{

	class Record
	{
	public:

		// Create an empty record with size number of entries
		Record(int size);

		// Return the size of the record
		int GetSize();

		// Get an attribute of a record
		std::string Get(int index);

		// Set an attribute of a record
		void Set(int index, std::string new_value);

	};

	class Table
	{
	public:
		// Constructs an empty table
		Table();

		// Constructs a table with all attributes in thsi list
		Table(std::vector<std::string> attributes);

		// Adds an attribute column to the end of the table
		void AddAttribute(std::string attribute_name);

		// Deletes an attribute column from the table
		void DeleteAttribute(std::string attribute_name);

		// Inserts a record into the table
		void InsertRecord(Record new_record);

		// Returns a list of all attributes in the table
		std::vector<std::string> ListAttributes();

		// Get the size (number of records from the table)
		int GetSize();

		// Get an pointer to the first record, returns NULL if table is empty
		// This pointer can then be used to iterate through the table
		Record* GetFirstRecord();

		// Designate the key_name to be the key
		void DesignateKey(std::string key_name);

		// cross join joining table with this table
		Table CrossJoin(Table joining_table);

		// natural join joining table with this table
		Table NaturalJoin(Table joining_table);

		// Count the non-null entries of this attribute
		int Count(std::string attribute_name);

		// Compute the maximum of the entries of this attribute
		int Max(std::string attribute);

		// Compute the minimum of the entries of this attribure
		int Min(std::string attribute);

	};

	// This class is exported from the Database.dll  
	class Database
	{
	public:

		// Construct an empty database
		Database();

		// Add a table with name table_name to the database
		void Add(std::string table_name, Table table);

		// Drop table table_name from the database
		void Drop(std::string table_name);

		// Returns a list of table names
		std::vector<std::string> ListTables();

		// Get pointers to all tables in the database
		std::vector<Table*> GetTables();

		// Query to return a table
		Table* Query(std::string SELECT, std::string FROM, std::string WHERE);

	};
}
