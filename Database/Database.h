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
		DATABASE_API Record(int size);

		// Return the size of the record	
		DATABASE_API int GetSize();

		// Get an attribute of a record
		DATABASE_API std::string Get(int index);

		// Set an attribute of a record
		DATABASE_API void Set(int index, std::string new_value);

	};

	class Table
	{
	public:
		// Constructs an empty table
		DATABASE_API Table();

		// Constructs a table with all attributes in thsi list
		DATABASE_API Table(std::vector<std::string> attributes);

		// Adds an attribute column to the end of the table
		DATABASE_API void AddAttribute(std::string attribute_name);

		// Deletes an attribute column from the table
		DATABASE_API void DeleteAttribute(std::string attribute_name);

		// Inserts a record into the table
		DATABASE_API void InsertRecord(Record new_record);

		// Returns a list of all attributes in the table
		DATABASE_API std::vector<std::string> ListAttributes();

		// Get the size (number of records from the table)
		DATABASE_API int GetSize();

		// Get an pointer to the first record, returns NULL if table is empty
		// This pointer can then be used to iterate through the table
		DATABASE_API Record* GetFirstRecord();

		// Designate the key_name to be the key
		DATABASE_API void DesignateKey(std::string key_name);

		// cross join joining table with this table
		DATABASE_API Table CrossJoin(Table joining_table);

		// natural join joining table with this table
		DATABASE_API Table NaturalJoin(Table joining_table);

		// Count the non-null entries of this attribute
		DATABASE_API int Count(std::string attribute_name);

		// Compute the maximum of the entries of this attribute
		DATABASE_API int Max(std::string attribute);

		// Compute the minimum of the entries of this attribure
		DATABASE_API int Min(std::string attribute);

	};

	// This class is exported from the Database.dll  
	class Database
	{
	public:

		// Construct an empty database
		DATABASE_API Database();

		// Add a table with name table_name to the database
		DATABASE_API void Add(std::string table_name, Table table);

		// Drop table table_name from the database
		DATABASE_API void Drop(std::string table_name);

		// Returns a list of table names
		DATABASE_API std::vector<std::string> ListTables();

		// Get pointers to all tables in the database
		DATABASE_API std::vector<Table*> GetTables();

		// Query to return a table
		DATABASE_API Table* Query(std::string SELECT, std::string FROM, std::string WHERE);

	};
}
