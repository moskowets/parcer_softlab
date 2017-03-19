#pragma once

#include "string"
#include "vector"
#include "iostream"

class Node
{
public:
	int id;
	int parent_id{ 0 };
	//имя узла, может содержать только буквы, цифры или знак подчёркивания и не может начинаться с цифры
	std::string name;
	//значение узла, не может содержать \r, \n и двойные кавычки
	std::string text;
	std::vector<Node> subnodes;
	void set_parent_id();
	void export_to_file(std::ostream *csv_file);
};

