#pragma once


#include"tokenize.h"

struct NodeTermVar
{
	std::optional<std::string> value;
	std::string name;
};

struct NodeTermIntVal
{
	std::optional<std::string> value;
};

struct NodeTerm
{
	std::variant<NodeTermIntVal, NodeTermVar> term;
};

struct NodeExpr
{
	NodeTerm term;
};

struct NodeStatVar
{
	NodeExpr expr;
	std::string name;
};

struct NodeStateEq
{
	NodeExpr expr;
	std::string variableName;
};


struct NodeStatExit
{
	NodeExpr expr;
};

struct NodeStat
{
	std::variant<NodeStatExit,NodeStatVar,NodeStateEq> stat;
};

struct NodeProg
{
	std::vector<NodeStat> stats;
};


class Parser
{
public:
	Parser(std::vector<Token> tokens)
		: m_tokens(tokens)
	{

	}

	std::optional<NodeTerm> parse_term()
	{
		if (peek().value().type == TokenType::int_val)
		{
			NodeTerm term;
			NodeTermIntVal int_val = { .value = consume().value};
			term.term = int_val;
			return term;
		}
		else if (peek().value().type == TokenType::variable)
		{
			NodeTerm term;
			NodeTermVar var_val = { .value = {}, .name = consume().value.value()};
			term.term = var_val;
			return term;
		}

		return {};
	}

	std::optional<NodeExpr> parse_expr()
	{
		if (peek().value().type == TokenType::int_val || peek().value().type == TokenType::variable)
		{
			NodeExpr expr = { .term = parse_term().value() };
			return expr;
		}

		return {};
	}

	std::optional<NodeStat> parse_stat()
	{
		if (peek().has_value() && peek().value().type == TokenType::ret && peek(1).has_value() && (peek(1).value().type == TokenType::int_val || peek(1).value().type == TokenType::variable) &&
			peek(2).has_value() && peek(2).value().type == TokenType::semi)
		{
			consume();
			NodeStat stat;
			NodeStatExit stat_exit = { .expr = parse_expr().value()};
			consume();
			stat.stat = stat_exit;
			return stat;
		}
		else if (peek().has_value() && peek().value().type == TokenType::integer && peek(1).has_value() && peek(1).value().type == TokenType::variable &&
			peek(2).has_value() && peek(2).value().type == TokenType::eq)
		{
			consume();
			NodeStat stat;
			NodeStatVar stat_var;
			stat_var.name = consume().value.value();
			consume();
			stat_var.expr = parse_expr().value();
			consume();
			
			stat.stat = stat_var;
			return stat;
		}
		else if (peek().has_value() && peek().value().type == TokenType::variable && peek(1).has_value() && peek(1).value().type == TokenType::eq 
			&& peek(2).has_value() && (peek(2).value().type == TokenType::int_val || peek(2).value().type == TokenType::variable))
		{
			NodeStat stat;
			NodeStateEq stat_eq;
			stat_eq.variableName = consume().value.value();
			consume();
			stat_eq.expr = parse_expr().value();
			consume();

			stat.stat = stat_eq;
			return stat;
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
			else
			{
				std::cerr << "Invalid statement!!" << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		return prog;
	}

private:
	[[nodiscard]] std::optional<Token> peek(size_t offset = 0) const
	{
		if (m_index + offset >= m_tokens.size()) {
			return {};
		}
		return m_tokens.at(m_index + offset);
	}

	Token consume()
	{
		return m_tokens.at(m_index++);
	}

	size_t m_index = 0;
	std::vector<Token> m_tokens;
};