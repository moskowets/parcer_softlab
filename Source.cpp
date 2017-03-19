/*
	Данная программа представляет из себя синтаксический анализатор, 
	который принимает на вход файл, содержащий один корневой узел, в который 
	входят другие узлы.
	Функция main_menu() обеспечивает пользовательское меню.
	Функция add_node() считывает данные из потока лексем и записывает 
	считанные узлы в корневой узел root_node (класс Node). По мере считывания узлам
	присваивается уникальный id. В случае успешного считываения, узлам 
	присваивается также родительский parent_id с помощью фукнции 
	set_parent_id() класса Node. В дальнейшем данные экспортируются в файл с 
	помощью функции export_to_file() класса Node, а также в базу данных MySQL,
	для взаимодействия с которой служит объект sql_c.
*/

#include "iostream"
#include "fstream"
#include "string"
#include "iomanip"

#include "sql_obj.h"

#include "node.h"
#include "token_stream.h"

using namespace std;

namespace data
{
	int id{ 0 };
	string input_file_name;	
	SQL_conf_data sql_parameters;
	bool upload_to_database{ false };
}

void main_menu(SQL_conf_data& data, string& file_name)
{
	string answer;
	cout << "Upload data to database? Accept: enter 'yes': ";
	cin >> answer;
	int start{ 5 };
	if (answer == "yes")
	{
		start = 0;
		data::upload_to_database = true;
	}
	for (int i = start; i < 7; ++i)
	{
		system("CLS");
		int cell_len{ 40 };
		cout << setw(cell_len) << "MySQL 'ip_address:port': "	<< data.address << '\n';
		cout << setw(cell_len) << "MySQL username: "	<< data.user << '\n';
		cout << setw(cell_len) << "MySQL password': "	<< data.password << '\n';
		cout << setw(cell_len) << "MySQL database name: "	<< data.database << '\n';
		cout << setw(cell_len) << "MySQL table name: "	<< data.table << '\n';
		cout << setw(cell_len) << "Input file name (file with nodes): "	<< file_name << '\n';
		cout << setw(cell_len) << "Output file name: " << "table.csv\n\n";
		switch (i)
		{
		case 0:
			cout << "Enter MySQL server ip address and password, example 192.168.0.3:3306: ";			
			cin >> data.address;
			data.address = "tcp://" + data.address;
			break;
		case 1:
			cout << "Enter MySQL username: ";
			cin >> data.user;
			break;
		case 2:
			cout << "Enter MySQL user password: ";
			cin >> data.password;
			break;
		case 3:
			cout << "Enter the name of database|shema on MySQL server (if there is no shema with such name, it will be created): ";
			cin >> data.database;
			break;
		case 4:
			cout << "Enter the name of table in selected database (if there is no table with such name, it will be created): ";
			cin >> data.table;
			break;
		case 5:
			cout << "Enter the name of input file, wich contains nodes (example input.txt): ";
			cin >> file_name;
			break;
		default:
			break;
		}
	}
}

Node add_node(TokenStream& ts)
{
	Token t = ts.get_token();
	if (t.kind == data::name)
	{
		Node n;
		n.id = ++data::id;
		n.name = t.text;
		t = ts.get_token();
		if (t.kind == '=')
		{
			t = ts.get_token();
			if (t.kind == data::text)
			{
				n.text = t.text;
				return n;
			}
			else if (t.kind == '{')
			{
				while (true)
				{
					t = ts.get_token();
					if (t.kind == data::name)
					{
						ts.unget_token();
						n.subnodes.push_back(add_node(ts));
					}
					else if (t.kind == '}')
					{
						return n;
					}
					else error();
				}
			}
			else error();
		}		
		else error();
	}
	else error();
}

int main()
try
{
	main_menu(data::sql_parameters, data::input_file_name);
	ifstream file{ data::input_file_name };
	ofstream export_file{ "table.csv" };
	if (!file) error();
	TokenStream ts{ &file };
	Node root_node = add_node(ts);
	root_node.set_parent_id();	
	root_node.export_to_file(&export_file);
	if (export_file)
	{
		cout << "Success! Data exported to file!\n";
		export_file.close();
	}
	else cout << "Unable to export data...\n";
	if (data::upload_to_database)
	{
		try
		{
			SQL_obj sql_c{ data::sql_parameters };
			if (sql_c)
			{
				cout << "Connected to database.\n";
				sql_c.add_data(root_node);
				cout << sql_c.test() << " lines on record in database.\n";
			}
			else cout << "Unable connect to database.\n";
		}
		catch (sql::SQLException &e)
		{
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}
	cout << "Type 'exit' to quit\n";
	string temp;
	while (temp != "exit") cin >> temp;
	return 0;
}
catch (const exception& e)
{
	cout << e.what() << '\n';	
	cout << "Type 'exit' to quit\n";
	string temp;
	while (temp != "exit") cin >> temp;
	return 1;
}