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
	 fslash,
	 character,
	 open_char,
	 close_char,
	 char_val,
	 print,
	 newLine,
	 ifscope,
	 elseifscope,
	 elsescope,
	 greater,
	 lesser,
	 notequal,
	 equal,
	 greaterequal,
	 lesserequal,
	 OR,
	 AND,
	 boolean,
	 INPUT,
	 opposet,
	 FALSE,
	 TRUE,
	 open_braket,
	 close_braket,
	 string,
	 open_string,
	 close_string,
	 string_val,
	 REMOVE_STRING,
};
inline std::string to_string(const TokenType type)
{
	switch (type) {
	case TokenType::ret:
		return "`exit`";
	case TokenType::int_val:
		return "integer";
	case TokenType::semi:
		return "`;`";
	case TokenType::open_paren:
		return "open_paren";
	case TokenType::close_paren:
		return "`)`";
	case TokenType::integer:
		return "integer";
	case TokenType::variable:
		return "variable";
	case TokenType::close_string:
		return "close_string";
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
	case TokenType::character:
		return "character";
	case TokenType::open_char:
		return "character";
	case TokenType::string:
		return "string";
	case TokenType::open_string:
		return "string";
	case TokenType::boolean:
	    return "boo";
	case TokenType::open_braket:
		return "open_bralet";
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
inline std::optional<int> log_prec(const TokenType type)
{
	switch (type) {
	case TokenType::greater:
		return 0;
	case TokenType::lesser:
		return 1;
	case TokenType::equal:
		return 2;
	case TokenType::notequal:
		return 3;
	case TokenType::greaterequal:
		return 4;
	case TokenType::lesserequal:
		return 5;
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
			if (isalpha(peek().value()[0]))
			{
				str.push_back(consume());
				while (peek().has_value() && isalpha(peek().value()[0]))
				{
					str.push_back(consume());
				}

				if (str == "return")
				{
					tokens.push_back({ .type = TokenType::ret });
					str.clear();
				}
				if (str == "remove")
				{
					tokens.push_back({ .type = TokenType::REMOVE_STRING });
					str.clear();
				}
				else if (str == "int")
				{
					tokens.push_back({ .type = TokenType::integer});
					str.clear();
				}
				else if (str == "string")
				{
					tokens.push_back({ .type = TokenType::string });
					str.clear();
				}
				else if (str == "else")
				{
					   str.clear();
						int temp = m_index;
						while (peek().has_value() && peek().value()[0] == ' ')
							consume();
						while (peek().has_value() && isalpha(peek().value()[0]))
						{
							str += consume();
						}
						if (str == "if")
						{
							tokens.push_back({ .type = TokenType::elseifscope });
							str.clear();
						}
						else
						{
							tokens.push_back({ .type = TokenType::elsescope });
							str.clear();
						}
				}
				else if (str == "if")
				{
					tokens.push_back({ .type = TokenType::ifscope });
					str.clear();
				}
				else if (str == "main")
				{
					tokens.push_back({ .type = TokenType::MAIN });
					str.clear();
				}
				else if (str == "false")
				{
					tokens.push_back({ .type = TokenType::int_val,.value = std::to_string(0) });
					str.clear();
				}
				else if (str == "true")
				{
					tokens.push_back({ .type = TokenType::int_val,.value = std::to_string(1) });
					str.clear();
				}
				else if (str == "cin")
				{
					tokens.push_back({ .type = TokenType::INPUT });
					str.clear();
				}
				else if (str == "char")
				{
					tokens.push_back({ .type = TokenType::character });
					str.clear();
				}
				else if (str == "printf")
				{
					tokens.push_back({ .type = TokenType::print });
					str.clear();
				}
				else if (str == "bool")
				{
					tokens.push_back({ .type = TokenType::boolean });
					str.clear();
				}
				else if(!str.empty())
				{
						tokens.push_back({ .type = TokenType::variable, .value = str });
					    str.clear();
				}
			}
			else if (peek().value()[0] == ' ')
			{
				consume();
				continue;
			}
			else if (peek().value()[0] == '>')
			{
				if (peek(1).has_value() && peek(1).value()[0] == '=')
				{
					tokens.push_back({ .type = TokenType::greaterequal });
					consume();
					consume();
					continue;
				}
				else
				{
					tokens.push_back({ .type = TokenType::greater });
					consume();
					continue; 
				}
			}
			else if (peek().value()[0] == '<')
			{
			    if (peek(1).has_value() && peek(1).value()[0] == '=')
			    {
			     	tokens.push_back({ .type = TokenType::lesserequal });
			     	consume();
			    	consume();
			   	    continue;
			    }
			    else
			    {
			    	tokens.push_back({ .type = TokenType::lesser });
			   	    consume();
			   	    continue;
			    }
			}
			else if (peek().value()[0] == '(')
			{
				tokens.push_back({ .type = TokenType::open_paren });
				consume();
				continue;
			}
			else if (peek().value()[0] == '[')
			{
				tokens.push_back({ .type = TokenType::open_braket });
				consume();
				continue;
			}
			else if (peek().value()[0] == ']')
			{
				tokens.push_back({ .type = TokenType::close_braket });
				consume();
				continue;
			}
			else if (peek().value()[0] == '$' && peek(1).has_value() && peek(1).value()[0] == 'n')
			{
				tokens.push_back({ .type = TokenType::newLine });
				consume();
				consume();
				continue;
			}
			else if (peek().value()[0] == ')')
			{
				tokens.push_back({ .type = TokenType::close_paren });
				consume();
				continue;
			}
			else if (peek().value()[0] == '+')
			{
				tokens.push_back({ .type = TokenType::plus });
				consume();
				continue;
			}
			else if (peek().value()[0] == '-')
			{
				tokens.push_back({ .type = TokenType::minus });
				consume();
				continue;
			}
			else if (peek().value()[0] == '*')
			{
				tokens.push_back({ .type = TokenType::star });
				consume();
				continue;
			}
			else if (peek().value()[0] == '&')
			{
				tokens.push_back({ .type = TokenType::AND });
				consume();
				continue;
			}
			else if (peek().value()[0] == '|')
			{
				tokens.push_back({ .type = TokenType::OR });
				consume();
				continue;
			}
			else if (peek().value()[0]  == '"')
			{
				tokens.push_back({ .type = TokenType::open_string });
				consume();

				str.push_back(consume());
				while (peek().has_value() && peek().value()[0] != '"')
				{
					str.push_back(consume());
				}

				tokens.push_back({ .type = TokenType::string_val,.value = str });
				str.clear();

				if (peek().value()[0] == '"')
				{
					tokens.push_back({ .type = TokenType::close_string });
					consume();
				}
				else
				{
					std::cerr << " Invalid expressiuon!!" << std::endl;
					exit(EXIT_FAILURE);
				}
				continue;
				}
			else if (peek().value() == "'")
			{
				tokens.push_back({ .type = TokenType::open_char });
				consume();
				if (peek().has_value() && peek(1).has_value() && peek(1).value() == "'")
				{
					std::string name;
					name += consume();
					tokens.push_back({.type = TokenType::char_val, .value = name});
					tokens.push_back({ .type = TokenType::close_char });
					consume();
				}

				continue;
			}
			else if (peek().value()[0] == '/')
			{
				tokens.push_back({ .type = TokenType::fslash });
				consume();
				continue;
			}
			else if (peek().value()[0] == '{')
			{
				tokens.push_back({ .type = TokenType::open_brek });
				consume();
				continue;
			}
			else if (peek().value()[0] == '}')
			{
				tokens.push_back({ .type = TokenType::close_brek });
				consume();
				continue;
			}
			else if (peek().value()[0] == '=')
			{
				if (peek(1).has_value() && peek(1).value()[0] == '=')
				{
					tokens.push_back({ .type = TokenType::equal });
					consume();
					consume();
					continue;
				}
				else
				{
					tokens.push_back({ .type = TokenType::eq });
					consume();
					continue;
				}
			}
			else if (peek().value()[0] == ';')
			{
				tokens.push_back({ .type = TokenType::semi });
				consume();
				continue;
			}
			else if (peek().value()[0] == '!')
			{
				if (peek(1).has_value() && peek(1).value()[0] == '=')
				{
					tokens.push_back({ .type = TokenType::notequal });
					consume();
					consume();
					continue;
				}
				else 
				{
					tokens.push_back({ .type = TokenType::opposet });
					consume();
					continue;
				}
			}
			else if (isdigit(peek().value()[0]))
			{
				str.push_back(consume());
				while (peek().has_value() && isdigit(peek().value()[0]))
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
	[[nodiscard]] std::optional<std::string> peek(size_t offset = 0) const
	{
		if (m_index + offset >= filename.length()) {
			return {};
		}
		std::string res;
		res += filename.at(m_index + offset);
		return res;
	}
	char consume()
	{
		return filename.at(m_index++);
	}
private:
	std::string filename;
	size_t m_index = 0;
};