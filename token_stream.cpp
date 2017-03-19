#include "token_stream.h"

using namespace std;

void error()
{
	throw std::exception("Wrong data format");
}

TokenStream::TokenStream(istream * input_stream)
{
	if (*input_stream) stream = input_stream;
	else throw exception("Bad file");
}

TokenStream::operator bool() const
{
	return stream->good();
}

void TokenStream::unget_token()
{
	available_in_buffer = true;
}

Token TokenStream::get_token()
{
	if (available_in_buffer)
	{
		available_in_buffer = false;
		return buffer;
	}
	if (*stream)
	{
		char ch{ 0 };
		*stream >> ch;
		if (isalpha(ch) || ch == '_')
		{
			string tmp;
			tmp += ch;
			while (stream->get(ch))
			{
				if (isalpha(ch) || ch == '_' || isdigit(ch))
				{
					tmp += ch;
				}
				else
				{
					stream->unget();
					break;
				}
			}
			buffer.kind = data::name;
			buffer.text = tmp;
			return buffer;
		}
		switch (ch)
		{
		case '{':
			buffer.kind = '{';
			return buffer;
		case '}':
			buffer.kind = '}';
			return buffer;
		case '=':
			buffer.kind = '=';
			return buffer;
		case '\"':
		{
			string tmp;
			bool second_double_quote{ false };
			while (stream->get(ch))
			{
				if (ch == '\"')
				{
					second_double_quote = true;
					break;
				}
				if (ch == '\r' || ch == '\n') error();
				tmp += ch;
			}
			if (!second_double_quote) error();
			buffer.kind = data::text;
			buffer.text = tmp;
			return buffer;
		}
		default:
			break;
		}
	}	
	buffer.kind = data::unknown;
	return buffer;	

}