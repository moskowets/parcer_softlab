#pragma once
#include "string"
#include "node.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class SQL_conf_data
{
public:
	std::string address;
	std::string user;
	std::string password;
	std::string database;
	std::string table;
};

class SQL_obj
{
public:
	SQL_obj(const SQL_conf_data& data);
	~SQL_obj();
	void add_data(Node n);
	int test();
	operator bool() const;
private:
	SQL_conf_data conf;
	sql::Driver *driver;
	sql::Connection *con{ nullptr };
	sql::Statement *stmt{ nullptr };
	sql::ResultSet *res;
};

