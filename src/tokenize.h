#pragma once

enum TokenType
{
	ret,
	int_val,
	semi,
	integer,
	MAIN,
	open_paren,
	close_paren,
	open_brek,
	close_brek
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
				else if (str == "int")
				{
					tokens.push_back({ .type = TokenType::integer });
					str.clear();
				}
				else if (str == "main")
				{
					tokens.push_back({ .type = TokenType::MAIN });
					str.clear();
				}
			}
			else if (peek().value() == ' ')
			{
				consume();
				continue;
			}
			else if (peek().value() == '(')
			{
				tokens.push_back({ .type = TokenType::open_paren });
				consume();
				continue;
			}
			else if (peek().value() == ')')
			{
				tokens.push_back({ .type = TokenType::close_paren });
				consume();
				continue;
			}
			else if (peek().value() == '{')
			{
				tokens.push_back({ .type = TokenType::open_brek });
				consume();
				continue;
			}
			else if (peek().value() == '}')
			{
				tokens.push_back({ .type = TokenType::close_brek });
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
			else
				consume();
		}

		if (tokens.size() < 4 || tokens[0].type != TokenType::integer || tokens[1].type != TokenType::MAIN || tokens[2].type != TokenType::open_paren || tokens[3].type != TokenType::close_paren ||
			tokens[4].type != TokenType::open_brek || tokens[tokens.size() - 1].type != TokenType::close_brek)
		{
			std::cerr << "Dont have entry points!!" << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			tokens.erase(tokens.begin(),tokens.begin() + 5);
			tokens.erase(tokens.end() - 1);
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