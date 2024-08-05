#pragma once

struct NodeExpr
{
	std::optional<std::string> val;
};

struct NodeStatExit
{
	NodeExpr expr;
};

struct NodeStat
{
	NodeStatExit node;
};

struct NodeProg
{
	std::vector<NodeStat> stats;
};

class Parser
{
 public:
	Parser(std::vector<Token> tokens)
		: tokens(std::move(tokens)),
		 m_index(0)
	{

	}

	std::optional<NodeStat> parse_stat()
	{
		if(peek().has_value() && peek().value().type == TokenType::ret && peek(1).has_value() && peek(1).value().type == TokenType::int_val
			&& peek(2).has_value() && peek(2).value().type == TokenType::semi)
		{
			consume();
			NodeStat stats;
			NodeStatExit exit;
			exit.expr.val = consume().value;
			consume();
			stats.node = exit;
			return stats;
		}

		return {};
	}

	std::optional<NodeProg> parse_prog()
	{
		NodeProg prog;
		while (peek().has_value())
		{
			if (auto stat = parse_stat())
			{
				prog.stats.push_back(stat.value());
			}
			else {
				std::cerr << "Invalid statement" << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		return prog;
	}

private:
	[[nodiscard]] std::optional<Token> peek(size_t offset = 0) const
	{
		if (m_index + offset >= tokens.size()) {
			return {};
		}
		return tokens.at(m_index + offset);
	}

	Token consume()
	{
		return tokens.at(m_index++);
	}

	size_t m_index = 0;
	std::vector<Token> tokens;
};