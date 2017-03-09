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

	void Record::RemoveAt(int i)
	{
		this->values.erase(this->values.begin() + i);
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

	int Table::getIndexOfAttribute(std::string attribute)
	{
		int indexOfAttribute = -1;
		std::vector<std::string> attributes = this->attributes;

		for (int i = 0; i < attributes.size(); i++)
		{

			if (attributes.at(i) == attribute)
			{
				indexOfAttribute = i;
				break;
			}
		}
		return indexOfAttribute;
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
		if (this->attributes.size() == 0) {
			std::cerr << "Table can not inserted record without setting attributes\n";
			return;
		}

		Record* _new_record = new Record(new_record.GetSize());
		for (int i = 0; i < new_record.GetSize(); i++)
			_new_record->Set(i, new_record.Get(i));

		// Trim more value which more than table's # of attributes
		if (this->attributes.size() < _new_record->GetSize()) {
			_new_record->values.erase(_new_record->values.begin() + this->attributes.size(), _new_record->values.end());
		}
		// Or set NULL for missing value
		if (this->attributes.size() > _new_record->GetSize()) {
			for (int i = 0; i < (this->attributes.size() - _new_record->GetSize()); i++)
				_new_record->values.push_back("NULL");
		}

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
		// Always set table attributes first.
		for (int i = 0; i < this->attributes.size(); i++)
			join_table.AddAttribute(this->attributes[i]);
		for (int i = 0; i < joining_table.attributes.size(); i++)
			join_table.AddAttribute(joining_table.attributes[i]);

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
			// Always set table attributes first.
			for (int i = 0; i < this->attributes.size(); i++)
				join_table.AddAttribute(this->attributes[i]);
			for (int i = 0; i < joining_table.attributes.size(); i++) {
				if (i != index2)
					join_table.AddAttribute(joining_table.attributes[i]);
			}
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
		int index = 0;
		bool find = false;
		int count = 0;
		for (int i = 0; i < this->attributes.size(); i++) {
			if (this->attributes[i].compare(attribute_name) == 0) {
				index = i;
				find = true;
				break;
			}
		}
		if (find) {
			Record* current = this->head;
			while (current != NULL) {
				if (current->Get(index).compare("NULL") != 0)
					count++;
				current = current->next;
			}
			return count;
		}
		else {
			return -1; // No this attribute in this table.
		}
	}

	std::string Table::Max(std::string attribute)
	{
		int index = this->getIndexOfAttribute(attribute);
		if (index == -1) {
			std::cerr << "There is no this attribute:" << attribute << "in this table\n";
			return "";
		}
		Record* current = this->head;
		std::string value, max_value;
		int i_value, _value, max;
		char i_charvalue;
		max = 0;
		max_value = current->Get(index);
		while (current != NULL) {
			value = current->Get(index);
			_value = 0;
			for (int i = 0; i < value.size(); i++) {
				i_charvalue = value[i];
				i_value = i_charvalue; // cast to ASCII
				_value = _value + i_value*pow(10, value.size() - 1 - i);
			}
			if (max < _value) {
				max = _value;
				max_value = value;
			}
			current = current->next;
		}
		return max_value;
	}

	std::string Table::Min(std::string attribute)
	{
		int index = this->getIndexOfAttribute(attribute);
		if (index == -1) {
			std::cerr << "There is no this attribute:" << attribute << "in this table\n";
			return "";
		}
		Record* current = this->head;
		std::string value, min_value;
		int i_value, _value, min;
		char i_charvalue;
		min = 10000000000;
		min_value = current->Get(index);
		while (current != NULL) {
			value = current->Get(index);
			_value = 0;
			for (int i = 0; i < value.size(); i++) {
				i_charvalue = value[i];
				i_value = i_charvalue; // cast to ASCII
				_value = _value + i_value*pow(10, value.size() - 1 - i);
			}
			if (min > _value) {
				min = _value;
				min_value = value;
			}
			current = current->next;
		}
		return min_value;
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

	void Table::DeleteRecord(Record * r)
	{
		Record * current_record = head;
		if (r == head)
		{
			head = head->next;
			return;
		}
		else
		{
			while (current_record->next)
			{
				if (current_record->next == r)
				{
					current_record->next = current_record->next->next;
					return;
				}
				current_record = current_record->next;
			}
		}
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

		/*
		Remove records that don't fit our where clause
		*/
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
		std::string a = "";
		std::string b = "";
		int attr_index;
		while (r)
		{
			stack = std::stack<std::string>();
			for (std::string exp : expression)
			{
				// Handle operators
				if (exp == "=")
				{
					a = stack.top();
					stack.pop();
					b = stack.top();
					stack.pop();
					if (a == b)
					{
						stack.push("TRUE");
					}
					else
					{
						stack.push("FALSE");
					}
				}

				else if (exp == "<>")
				{
					a = stack.top();
					stack.pop();
					b = stack.top();
					stack.pop();
					if (a != b)
					{
						stack.push("TRUE");
					}
					else
					{
						stack.push("FALSE");
					}
				}

				else if (exp == ">")
				{
					a = stack.top();
					stack.pop();
					b = stack.top();
					stack.pop();
					if (a > b)
					{
						stack.push("TRUE");
					}
					else
					{
						stack.push("FALSE");
					}
				}

				else if (exp == "<")
				{
					a = stack.top();
					stack.pop();
					b = stack.top();
					stack.pop();
					if (a < b)
					{
						stack.push("TRUE");
					}
					else
					{
						stack.push("FALSE");
					}
				}

				else if (exp == ">=")
				{
					a = stack.top();
					stack.pop();
					b = stack.top();
					stack.pop();
					if (a >= b)
					{
						stack.push("TRUE");
					}
					else
					{
						stack.push("FALSE");
					}
				}

				else if (exp == "<=")
				{
					a = stack.top();
					stack.pop();
					b = stack.top();
					stack.pop();
					if (a <= b)
					{
						stack.push("TRUE");
					}
					else
					{
						stack.push("FALSE");
					}
				}

				// Handle logic operators
				else if (exp == "AND")
				{
					a = stack.top();
					stack.pop();
					b = stack.top();
					stack.pop();
					if ((a == "TRUE") && (b == "TRUE"))
					{
						stack.push("TRUE");
					}
					else
					{
						stack.push("FALSE");
					}
				}

				else if (exp == "OR")
				{
					a = stack.top();
					stack.pop();
					b = stack.top();
					stack.pop();
					if ((a == "TRUE") || (b == "TRUE"))
					{
						stack.push("TRUE");
					}
					else
					{
						stack.push("FALSE");
					}
				}

				else if (exp == "NOT")
				{
					a = stack.top();
					stack.pop();
					if ((a == "FALSE"))
					{
						stack.push("TRUE");
					}
					else
					{
						stack.push("FALSE");
					}
				}

				// Handle values and attributes
				else
				{
					attr_index = result->getIndexOfAttribute(exp);
					if (attr_index == -1)
					{
						stack.push(exp);
					}
					else
					{
						stack.push(r->Get(attr_index));
					}
				}
			}

			if (stack.top() != "TRUE")
			{
				result->DeleteRecord(r);
			}
			r = r->next;
		}

		/*
		Trim unwanted attributes
		*/
		attr_index = -1;
		for (std::string attr : all_attributes)
		{
			if (std::find(wanted_attributes.begin(), wanted_attributes.end(), attr) == wanted_attributes.end())
			{
				attr_index = result->getIndexOfAttribute(attr);
				r = result->GetFirstRecord();
				while (r)
				{
					r->RemoveAt(attr_index);
					r = r->next;
				}
				result->DeleteAttribute(attr);
			}
		}

		return result;
	}
}
