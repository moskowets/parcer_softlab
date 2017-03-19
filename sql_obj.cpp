#include "sql_obj.h"
#include "iostream"

using namespace std;

SQL_obj::SQL_obj(const SQL_conf_data& data)
{
	conf = data;
	driver = get_driver_instance();
	con = driver->connect(conf.address, conf.user, conf.password);
	stmt = con->createStatement();
	string statement;
	statement = "CREATE DATABASE IF NOT EXISTS " + conf.database;
	stmt->execute(statement);	
	statement = "USE " + conf.database;
	stmt->execute(statement);	
	statement  = "CREATE TABLE IF NOT EXISTS `" + conf.database + "`.`" + conf.table;
	statement += "` (`node_id` INT NOT NULL PRIMARY KEY,";
	statement += " `parent_id` INT REFERENCES tree_table(node_id),";
	statement += " `node_name` VARCHAR(100),";
	statement += " `node_value` VARCHAR(1000))";	
	stmt->execute(statement);
}

SQL_obj::operator bool() const
{
	return con->isValid();
}

SQL_obj::~SQL_obj()
{
	if (con != nullptr) delete con;
	if (stmt != nullptr) delete stmt;
	
}

void SQL_obj::add_data(Node n)
{
	if (con->isValid())
	{
		string parent_string;
		if (n.parent_id) parent_string = to_string(n.parent_id);
		else parent_string = "NULL";
		string statement = "INSERT INTO tree_table(node_id, parent_id, node_name, node_value) VALUES (";
		statement += to_string(n.id) + ',' + parent_string + ',';
		statement += '\'' + n.name + "','" + n.text + "')";
		stmt->execute(statement);
		for (Node& nd : n.subnodes) add_data(nd);
	}	
}

int SQL_obj::test()
{
	if (con->isValid())
	{
		string statement;
		statement = "USE " + conf.database;
		stmt->execute(statement);
		statement = "SELECT COUNT(*) FROM `" + conf.table + "`";
		res = stmt->executeQuery(statement);
		res->next();
		int result = res->getInt(1);
		delete res;
		return result;
	}
	else return -1;
}
