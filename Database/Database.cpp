// MathLibrary.cpp : Defines the exported functions for the DLL application.  
// Compile by using: cl /EHsc /DDATABASE_EXPORTS /LD Database.cpp  

#include "stdafx.h"  
#include <iostream>
#include "Database.h"  

namespace Database
{
	Record::Record(int size)
	{
	}

	int Record::GetSize()
	{
		return 0;
	}

	std::string Record::Get(int index)
	{
		return std::string();
	}

	void Record::Set(int index, std::string new_value)
	{
	}

	Table::Table()
	{
	}

	Table::Table(std::vector<std::string> attributes)
	{
	}

	void Table::AddAttribute(std::string attribute_name)
	{
	}

	void Table::DeleteAttribute(std::string attribute_name)
	{
	}

	void Table::InsertRecord(Record new_record)
	{
	}

	std::vector<std::string> Table::ListAttributes()
	{
		return std::vector<std::string>();
	}

	int Table::GetSize()
	{
		return 0;
	}

	Record * Table::GetFirstRecord()
	{
		return nullptr;
	}

	void Table::DesignateKey(std::string key_name)
	{
	}

	Table Table::CrossJoin(Table joining_table)
	{
		return Table();
	}

	Table Table::NaturalJoin(Table joining_table)
	{
		return Table();
	}

	int Table::Count(std::string attribute_name)
	{
		return 0;
	}

	int Table::Max(std::string attribute)
	{
		return 0;
	}

	int Table::Min(std::string attribute)
	{
		return 0;
	}

	/*
		Begin database functions
	*/
	Database::Database()
	{
		
	}

	void Database::Add(std::string table_name, Table* table)
	{
		all_tables.insert(std::make_pair(table_name, table));
	}

	void Database::Drop(std::string table_name)
	{
		all_tables.erase(table_name);
	}

	std::vector<std::string> Database::ListTables()
	{
		std::vector<std::string> table_names;
		std::map<std::string, Table*>::iterator it = all_tables.begin();
			
		while (it != all_tables.end())
		{
			table_names.push_back(it->first);
			it++;
		}
		return table_names;
	}

	std::vector<Table*> Database::GetTables()
	{
		std::vector<Table*> table_pointers;
		std::map<std::string, Table*>::iterator it = all_tables.begin();

		while (it != all_tables.end())
		{
			table_pointers.push_back(it->second);
			it++;
		}
		return table_pointers;
	}

	Table * Database::Query(std::string SELECT, std::string FROM, std::string WHERE)
	{
		return nullptr;
	}
}