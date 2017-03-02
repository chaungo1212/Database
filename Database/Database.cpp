// MathLibrary.cpp : Defines the exported functions for the DLL application.  
// Compile by using: cl /EHsc /DDATABASE_EXPORTS /LD Database.cpp  

#include "stdafx.h"  
#include <iostream>
#include "Database.h"  

namespace Database {
	Record::Record(int size)
	{
		this->values = std::vector<std::string>();
		//initialize record to have the correct amount of entires
		//record values initialized to "NA" to represent that there is no data for it yet
		this->values.resize(size, "NA");
		this->next = NULL;

	}

	int Record::GetSize()
	{
		return this->values.size();
	}

	std::string Record::Get(int index)
	{
		return this->values.at(index);
	}

	void Record::Set(int index, std::string new_value)
	{
		this->values.at(index) = new_value;
	}

	Record Record::Join(Record record2) {
		Record join_record(this->GetSize() + record2.GetSize());
		for (int i = 0; i < this->GetSize(); i++)
			join_record.Set(i, this->Get(i));
		for (int i = 0; i < record2.GetSize(); i++)
			join_record.Set(i + this->GetSize(), record2.Get(i));
		return join_record;
	}
	Record Record::Join_wo_index2entry(Record record2, int index2) {
		Record join_record(this->GetSize() + record2.GetSize());
		for (int i = 0; i < this->GetSize(); i++)
			join_record.Set(i, this->Get(i));
		for (int i = 0; i < record2.GetSize(); i++)
			join_record.Set(i + this->GetSize(), record2.Get(i));
		join_record.values.erase(join_record.values.begin() + this->GetSize() + index2);
		return join_record;
	}

	Table::Table()
	{
		this->head = NULL;
	}

	Table::Table(std::vector<std::string> attributes)
	{
		this->head = NULL;
		this->attributes = attributes;
	}

	void Table::AddAttribute(std::string attribute_name)
	{
		bool find = false;
		for (int i = 0; i < this->attributes.size(); i++) {
			if (this->attributes[i].compare(attribute_name) == 0) {
				//attribute_name is found
				find = true;
				break;
			}
		}
		if (find == false)
			this->attributes.push_back(attribute_name);
	}

	void Table::DeleteAttribute(std::string attribute_name)
	{
		for (int i = 0; i < this->attributes.size(); i++) {
			if (this->attributes[i].compare(attribute_name) == 0) {
				//we find attribute_name and delete it
				this->attributes.erase(this->attributes.begin() + i);
				return;
			}
		}
		std::cout << "Attribute: " << attribute_name << " not found." << std::endl;
		return;
	}

	void Table::InsertRecord(Record new_record)
	{
		Record* _new_record = new Record(new_record.GetSize());
		for (int i = 0; i < new_record.GetSize(); i++)
			_new_record->Set(i, new_record.Get(i));
		if (this->head == NULL) {
			this->head = _new_record;
		}
		else {
			Record* current = this->head;
			while (current->next != NULL) {
				current = current->next;
			}
			current->next = _new_record;
		}
	}

	std::vector<std::string> Table::ListAttributes()
	{
		return this->attributes;
	}

	int Table::GetSize()
	{
		int size = 0;
		Record* current = this->head;
		while (current != NULL) {
			size++;
			current = current->next;

		}
		return size;
	}

	Record * Table::GetFirstRecord()
	{
		return this->head;
	}

	void Table::DesignateKey(std::string key_name)
	{
		bool find = false;
		for (int i = 0; i < this->attributes.size(); i++) {
			if (this->attributes[i].compare(key_name) == 0) {
				this->key_attribute = key_name;
				find = true;
				break;
			}
		}
		if (find == false) {
			std::cout << "Key_name " << key_name << " is not found" << std::endl;
		}
	}

	Table Table::CrossJoin(Table joining_table)
	{
		Table join_table;
		int t1_size = this->GetSize(); // # of records
		int t2_size = joining_table.GetSize();
		int t1_nattrs = this->attributes.size(); // # of attributes
		int t2_nattrs = joining_table.attributes.size();
		Record* current1_ptr = this->head;
		Record* current2_ptr;
		Record current1(t1_nattrs);
		Record current2(t2_nattrs);
		while (current1_ptr != NULL) {
			current2_ptr = joining_table.head;
			for (int i = 0; i < t1_nattrs; i++)
				current1.Set(i, current1_ptr->values[i]);
			while (current2_ptr != NULL) {
				for (int i = 0; i < t2_nattrs; i++)
					current2.Set(i, current2_ptr->values[i]);
				Record join_record = current1.Join(current2);
				join_table.InsertRecord(join_record);
				current2_ptr = current2_ptr->next;
			}
			current1_ptr = current1_ptr->next;
		}
		return join_table;
	}

	Table Table::NaturalJoin(Table joining_table)
	{
		Table join_table;
		std::vector<std::string> attributes1 = this->attributes;
		std::vector<std::string> attributes2 = joining_table.attributes;
		int index1, index2;
		bool find = false;
		for (int i = 0; i < attributes1.size(); i++) {
			for (int j = 0; j < attributes2.size(); j++) {
				if (attributes1[i].compare(attributes2[j]) == 0) { // Find same attribute
					find = true;
					index1 = i;
					index2 = j;
					break;
				}
			}
		}
		if (find) {
			Record* current1_ptr = this->head;
			Record* current2_ptr;
			int t1_nattrs = this->attributes.size(); // # of attributes
			int t2_nattrs = joining_table.attributes.size();
			Record current1(t1_nattrs);
			Record current2(t2_nattrs);
			while (current1_ptr != NULL) {
				current2_ptr = joining_table.head;
				for (int i = 0; i < t1_nattrs; i++)
					current1.Set(i, current1_ptr->values[i]);
				while (current2_ptr != NULL) {
					if (current1_ptr->Get(index1).compare(current2_ptr->Get(index2)) == 0) {
						for (int i = 0; i < t2_nattrs; i++)
							current2.Set(i, current2_ptr->values[i]);
						Record join_record = current1.Join_wo_index2entry(current2, index2);

						join_table.InsertRecord(join_record);
					}
					current2_ptr = current2_ptr->next;
				}
				current1_ptr = current1_ptr->next;
			}

		}
		//else //error message
		return join_table;
	}


	int Table::Count(std::string attribute_name)
	{
		int count = 0;
		Record* currentRecord = this->head;

		int indexOfAttribute;
		std::vector<std::string> attributes = this->attributes;
		for (int i = 0; i < attributes.size(); i++)
		{
			if (attributes.at(i) == attribute_name)
			{
				indexOfAttribute = i;
			}
		}

		while (currentRecord != NULL)
		{
			if (currentRecord->Get(indexOfAttribute) == attribute_name)
			{
				count++;
			}
			currentRecord = currentRecord->next;
		}
		return count;
	}

	std::string Table::Max(std::string attribute)
	{
		std::string max = "";
		Record* currentRecord = this->head;

		int indexOfAttribute;
		std::vector<std::string> attributes = this->attributes;

		if (attributes.size() > 0)
		{
			max = attributes.at(0);
		}

		for (int i = 0; i < attributes.size(); i++)
		{

			if (attributes.at(i) == attribute)
			{
				indexOfAttribute = i;
			}
		}

		while (currentRecord != NULL)
		{

			if ((currentRecord->Get(indexOfAttribute)).compare(max) > 0)
			{
				max = currentRecord->Get(indexOfAttribute);
			}
			currentRecord = currentRecord->next;
		}
		return max;
	}

	std::string Table::Min(std::string attribute)
	{
		std::string min = "";
		Record* currentRecord = this->head;

		int indexOfAttribute;
		std::vector<std::string> attributes = this->attributes;

		if (attributes.size() > 0)
		{
			min = attributes.at(0);
		}

		for (int i = 0; i < attributes.size(); i++)
		{

			if (attributes.at(i) == attribute)
			{
				indexOfAttribute = i;
			}
		}

		while (currentRecord != NULL)
		{

			if ((currentRecord->Get(indexOfAttribute)).compare(min) < 0)
			{
				min = currentRecord->Get(indexOfAttribute);
			}
			currentRecord = currentRecord->next;
		}
		return min;
	}

	void Table::Print() {
		std::cout << "----------------------------------------------------\n";
		Record* current = this->head;
		std::vector<std::string> values;
		while (current != NULL) {
			values = current->values;
			for (int i = 0; i < values.size(); i++)
				std::cout << values[i] << " ";
			std::cout << std::endl;
			current = current->next;
		}
		std::cout << "----------------------------------------------------\n";
	}

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
		/*
		Copy our source table
		*/
		Table* result = new Table();
		*result = *all_tables[FROM];

		/*
		Get all attributes and wanted attributes
		*/
		std::vector<std::string> all_attributes = result->ListAttributes();
		std::vector<std::string> wanted_attributes = std::vector<std::string>();

		std::istringstream stream(SELECT);
		std::string selected_attribute = "";
		while (std::getline(stream, selected_attribute, ','))
		{
			wanted_attributes.push_back(selected_attribute);
		}

		/*
		Remove records that don't fit our where clause
		*/
<<<<<<< HEAD
		std::stack<std::string> stack = std::stack<std::string>();
		std::vector<std::string> expression = std::vector<std::string>();

		// Turn infix into postfix expression
		std::istringstream str(WHERE);
		std::string token = "";
		while (std::getline(str, token, ' '))
		{
			if (strncmp(token.c_str(), "=", token.length()) == 0)
			{
				while (!(stack.empty() || (strncmp(stack.top().c_str(), "(", 1) == 0)))
				{
					expression.push_back(stack.top());
					stack.pop();
				}
				stack.push(token);
			}

			else if (strncmp(token.c_str(), "<>", token.length()) == 0)
			{
				while (!(stack.empty() || (strncmp(stack.top().c_str(), "(", 1) == 0)))
				{
					expression.push_back(stack.top());
					stack.pop();
				}
				stack.push(token);
			}

			else if (strncmp(token.c_str(), "<", token.length()) == 0)
			{
				while (!(stack.empty() || (strncmp(stack.top().c_str(), "(", 1) == 0)))
				{
					expression.push_back(stack.top());
					stack.pop();
				}
				stack.push(token);
			}

			else if (strncmp(token.c_str(), ">", token.length()) == 0)
			{
				while (!(stack.empty() || (strncmp(stack.top().c_str(), "(", 1) == 0)))
				{
					expression.push_back(stack.top());
					stack.pop();
				}
				stack.push(token);
			}

			else if (strncmp(token.c_str(), "<=", token.length()) == 0)
			{
				while (!(stack.empty() || (strncmp(stack.top().c_str(), "(", 1) == 0)))
				{
					expression.push_back(stack.top());
					stack.pop();
				}
				stack.push(token);
			}

			else if (strncmp(token.c_str(), ">=", token.length()) == 0)
			{
				while (!(stack.empty() || (strncmp(stack.top().c_str(), "(", 1) == 0)))
				{
					expression.push_back(stack.top());
					stack.pop();
				}
				stack.push(token);
			}

			else if (strncmp(token.c_str(), "AND", token.length()) == 0)
			{
				while (!(stack.empty() || (strncmp(stack.top().c_str(), "(", 1) == 0)))
				{
					expression.push_back(stack.top());
					stack.pop();
				}
				stack.push(token);
			}

			else if (strncmp(token.c_str(), "OR", token.length()) == 0)
			{
				while (!(stack.empty() || (strncmp(stack.top().c_str(), "(", 1) == 0)))
				{
					expression.push_back(stack.top());
					stack.pop();
				}
				stack.push(token);
			}

			else if (strncmp(token.c_str(), "NOT", token.length()) == 0)
			{
				while (!(stack.empty() || (strncmp(stack.top().c_str(), "(", 1) == 0)))
				{
					expression.push_back(stack.top());
					stack.pop();
				}
				stack.push(token);
			}

			else if (strncmp(token.c_str(), "(", token.length()) == 0)
			{
				stack.push(token);
			}

			else if (strncmp(token.c_str(), ")", token.length()) == 0)
			{
				while (!(stack.empty() || (strncmp(stack.top().c_str(), "(", 1) == 0)))
				{
					expression.push_back(stack.top());
					stack.pop();
				}
				stack.pop();
			}

			else
			{
				expression.push_back(token);
			}
		}
		
		while (!stack.empty())
		{
			expression.push_back(stack.top());
			stack.pop();
		}

		Record * r = result->GetFirstRecord();
		while (r)
		{
			stack = std::stack<std::string>();
			for (std::string exp : expression)
			{

			}
			r = r->next;
		}
=======
>>>>>>> origin/master

		/*
		Trim unwanted attributes
		*/
		for (std::string attr : all_attributes)
		{
			if (std::find(wanted_attributes.begin(), wanted_attributes.end(), attr) == wanted_attributes.end())
			{
				result->DeleteAttribute(attr);
			}
		}

		return result;
	}
}
