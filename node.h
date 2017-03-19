#pragma once

#include "string"
#include "vector"
#include "iostream"

class Node
{
public:
	int id;
	int parent_id{ 0 };
	//��� ����, ����� ��������� ������ �����, ����� ��� ���� ������������� � �� ����� ���������� � �����
	std::string name;
	//�������� ����, �� ����� ��������� \r, \n � ������� �������
	std::string text;
	std::vector<Node> subnodes;
	void set_parent_id();
	void export_to_file(std::ostream *csv_file);
};

