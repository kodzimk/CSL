#pragma once

enum TokenType
{
	ret,
	int_val,
	semi
};

struct Token
{
	TokenType type;
	std::optional<std::string> value;
};

class Tokenize
{
public:
	Tokenize(std::string& file)
		: filename(file)
	{

	}

	std::vector<Token> tokenazetion()
	{
		std::vector<Token> tokens;
		std::string str;
		while (peek().has_value())
		{
			if (isalpha(peek().value()))
			{
				str.push_back(consume());
				while (peek().has_value() && isalpha(peek().value()))
				{
					str.push_back(consume());
				}

				if (str == "return")
				{
					tokens.push_back({ .type = TokenType::ret });
					str.clear();
				}
			}
			else if (peek().value() == ' ')
			{
				consume();
				continue;
			}
			else if (peek().value() == ';')
			{
				tokens.push_back({ .type = TokenType::semi });
				consume();
				continue;
			}
			else if (isdigit(peek().value()))
			{
				str.push_back(consume());
				while (peek().has_value() && isdigit(peek().value()))
				{
					str.push_back(consume());
				}

				tokens.push_back({ .type = TokenType::int_val,.value = str });
				str.clear();
			}
		}

		return tokens;
	}

private:
	[[nodiscard]] std::optional<char> peek(size_t offset = 0) const
	{
		if (m_index + offset >= filename.length()) {
			return {};
		}
		return filename.at(m_index + offset);
	}

	char consume()
	{
		return filename.at(m_index++);
	}

	std::string filename;
	size_t m_index = 0;
};