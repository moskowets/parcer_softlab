#include "node.h"

using namespace std;

void Node::set_parent_id()
{
	for (Node& n : subnodes)
	{
		n.parent_id = id;		
		n.set_parent_id();
	}
}

void Node::export_to_file(ostream *csv_file)
{
	if (!parent_id)
	{
		*csv_file << "\"id\";\"parent_id\";\"name\";\"value\"" << endl;
	}	
	*csv_file << id << ';' << parent_id << ";\"" << name << "\";\"" << text << "\"" << endl;
	for (Node& n : subnodes) n.export_to_file(csv_file);

}