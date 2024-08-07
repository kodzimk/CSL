#pragma once

enum TokenType
{
	ret,
	int_val,
	semi,
	integer,
	MAIN,
	variable,
	open_paren,
	close_paren,
	open_brek,
	close_brek,
	eq,
     plus,
	 star,
	 minus,
	 fslash
};

inline std::string to_string(const TokenType type)
{
	switch (type) {
	case TokenType::ret:
		return "`exit`";
	case TokenType::int_val:
		return "int literal";
	case TokenType::semi:
		return "`;`";
	case TokenType::open_paren:
		return "`(`";
	case TokenType::close_paren:
		return "`)`";
	case TokenType::integer:
		return "identifier";
	case TokenType::variable:
		return "`let`";
	case TokenType::eq:
		return "`=`";
	case TokenType::plus:
		return "`+`";
	case TokenType::star:
		return "`*`";
	case TokenType::minus:
		return "`-`";
	case TokenType::fslash:
		return "`/`";
	}
	assert(false);
}

struct Token
{
	TokenType type;
	int line;
	std::optional<std::string> value;
};

inline std::optional<int> bin_prec(const TokenType type)
{
	switch (type) {
	case TokenType::minus:
	case TokenType::plus:
		return 0;
	case TokenType::fslash:
	case TokenType::star:
		return 1;
	default:
		return {};
	}
}

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
				else
				{
						tokens.push_back({ .type = TokenType::variable, .value = str });
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
			else if (peek().value() == '+')
			{
				tokens.push_back({ .type = TokenType::plus });
				consume();
				continue;
			}
			else if (peek().value() == '-')
			{
				tokens.push_back({ .type = TokenType::minus });
				consume();
				continue;
			}
			else if (peek().value() == '*')
			{
				tokens.push_back({ .type = TokenType::star });
				consume();
				continue;
			}
			else if (peek().value() == '/')
			{
				tokens.push_back({ .type = TokenType::fslash });
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
			else if (peek().value() == '=')
			{
				tokens.push_back({ .type = TokenType::eq });
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