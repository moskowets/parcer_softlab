#pragma once
#include "iostream"
#include "string"

namespace data
{
	const char name = 'n';
	const char text = 't';
	const char unknown = 'u';
}

class Token
{
public:
	char kind;
	std::string text;
};

void error();

class TokenStream
{
public:
	TokenStream(std::istream * input_stream);
	Token get_token();
	void unget_token();
	operator bool() const;

private:
	Token buffer;
	bool available_in_buffer{ false };
	std::istream * stream;
};

