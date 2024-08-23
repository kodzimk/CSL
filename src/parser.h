#pragma once

#include <variant>

#include"tokenize.h"
#include"arena.h"

struct NodeTermVar
{
	std::optional<std::string> value;
	std::string name;
	std::string eqName;
};

struct NodeTermIntVal
{
	std::optional<std::string> value;
};

struct NodeWordCharVal {
	std::optional<std::string> value;
	std::string name;
};

struct NodeExpr;

struct NodeTermParen {
	NodeExpr* expr;
	TokenType type;
};

struct NodeBinExprAdd {
	NodeExpr* lhs;
	NodeExpr* rhs;
};

struct NodeBinExprMulti {
	NodeExpr* lhs;
	NodeExpr* rhs;
};

struct NodeBinExprSub {
	NodeExpr* lhs;
	NodeExpr* rhs;
};

struct NodeBinExprDiv {
	NodeExpr* lhs;
	NodeExpr* rhs;
};

struct NodeBinExpr {
	std::variant<NodeBinExprAdd*, NodeBinExprMulti*, NodeBinExprSub*, NodeBinExprDiv*> var;
};

struct NodeLogAnd
{
	NodeExpr* lhs;
};
struct NodeLogOr
{
	NodeExpr* lhs;
};


struct NodeLogExprGreater {
	NodeExpr* lhs;
	NodeExpr* rhs;
	std::optional<NodeLogAnd*> varAnd;
	std::optional<NodeLogOr*> varOr;
};

struct NodeLogExprLesser {
	NodeExpr* lhs;
	NodeExpr* rhs;
	std::optional<NodeLogAnd*> varAnd;
	std::optional<NodeLogOr*> varOr;
};

struct NodeLogExprEqual {
	NodeExpr* lhs;
	NodeExpr* rhs;
	std::optional<NodeLogAnd*> varAnd;
	std::optional<NodeLogOr*> varOr;
};

struct NodeLogExprNotEqual {
	NodeExpr* lhs;
	NodeExpr* rhs;
	std::optional<NodeLogAnd*> varAnd;
	std::optional<NodeLogOr*> varOr;
};

struct NodeLogExprGreaterEqual {
	NodeExpr* lhs;
	NodeExpr* rhs;
	std::optional<NodeLogAnd*> varAnd;
	std::optional<NodeLogOr*> varOr;
};
struct NodeLogExprLesserEqual {
	NodeExpr* lhs;
	NodeExpr* rhs;
	std::optional<NodeLogAnd*> varAnd;
	std::optional<NodeLogOr*> varOr;
};

struct NodeLogExprParen
{
	NodeExpr* lhs;
	NodeExpr* rhs;
	std::optional<NodeLogAnd*> varAnd;
	std::optional<NodeLogOr*> varOr;
};


struct NodeLogExpr
{
	std::variant<NodeLogExprGreater*, NodeLogExprLesser*, NodeLogExprEqual*, NodeLogExprNotEqual*, NodeLogExprGreaterEqual*, NodeLogExprLesserEqual*, NodeLogAnd*, NodeLogOr*, NodeLogExprParen*> var;
};


struct NodeStatIncerement
{
	NodeExpr* expr;
	std::string variableName;
};

struct NodeStatDecrement
{
	NodeExpr* expr;
	std::string variableName;
};

struct NodeTermOpposet
{
	std::variant <NodeTermIntVal*, NodeTermVar*> var;
};

struct NodeTermArray
{
	std::string type;
	std::string equalType;
	NodeExpr* expr = nullptr;
	std::string name;
    std::string size;
};

struct NodeTerm {
	std::variant<NodeTermIntVal*, NodeTermVar*, NodeTermParen*, NodeWordCharVal*, NodeLogExpr*, NodeTermOpposet*,NodeTermArray*> var;
};

struct NodeExpr {
	std::variant<NodeTerm*, NodeBinExpr*, NodeLogExpr*> var;
};

struct NodeStmtExit {
	NodeExpr* expr;
};

struct NodeStatVar
{
	NodeExpr* expr;
	std::string name;
	std::string type;
};

struct NodeStateEq
{
	NodeExpr* expr;
	std::string variableName;
	std::string type;
};

struct NodeStatPrint
{
	std::optional<std::string> variableName;
	NodeExpr* expr;
	std::string type;
	bool newLine;
};

struct NodeStatInput
{
	std::string name;
	NodeExpr* expr;
};

struct NodeStat;

struct NodeScope {
	std::vector<NodeStat*> stmts;
};

struct NodeIfPred;

struct NodeIfPredElif {
	NodeExpr* expr{};
	NodeScope* scope{};
	std::optional<NodeIfPred*> pred;
};

struct NodeIfPredElse {
	NodeScope* scope;
};

struct NodeIfPred {
	std::variant<NodeIfPredElif*, NodeIfPredElse*> var;
};

struct NodeStatIf {
	NodeExpr* expr{};
	NodeScope* scope{};
	std::optional<NodeIfPred*> pred;
};

struct NodeStatExit
{
	NodeExpr* expr;
};

struct NodeStat
{
	std::variant<NodeStatExit*, NodeStatVar*, NodeStateEq*, NodeStatPrint*, NodeScope*, NodeStatIf*, NodeStatIncerement*, NodeStatDecrement*, NodeStatInput*> stat;
};

struct NodeProg
{
	std::vector<NodeStat*> stats;
};

class Parser
{
public:
	Parser(std::vector<Token> tokens)
		: m_tokens(tokens)
		, m_allocator(1024 * 1024 * 4)
	{

	}

public:
	std::optional<NodeTerm*> parse_term()
	{
		if (auto int_lit = try_consume(TokenType::int_val))
		{
			NodeTerm* term = m_allocator.emplace<NodeTerm>();
			NodeTermIntVal* int_val = m_allocator.emplace<NodeTermIntVal>();
			int_val->value = int_lit.value().value.value();

			term->var = int_val;
			return term;
		}
		else if (auto int_lit = try_consume(TokenType::minus))
		{
			NodeTerm* term = m_allocator.emplace<NodeTerm>();
			if (auto int_lit = try_consume(TokenType::int_val))
			{
				NodeTermIntVal* int_val = m_allocator.emplace<NodeTermIntVal>();
				int_val->value = int_lit.value().value.value();
				int_val->value.value().insert(int_val->value.value().begin(),'-');
				term->var = int_val;
			}
			
			return term;
		}
		else if (auto int_lit = try_consume(TokenType::variable))
		{
			NodeTerm* term = m_allocator.emplace<NodeTerm>();
			if (auto arr = try_consume(TokenType::open_braket))
			{
				NodeTermArray* array1 = m_allocator.emplace<NodeTermArray>();
				array1->name = m_tokens[m_index - 2].value.value();
				array1->size = consume().value.value();
				term->var = array1;
				try_consume(TokenType::close_braket);
			}
			else
			{
				NodeTermVar* var_val = m_allocator.emplace<NodeTermVar>();
				var_val->name = int_lit.value().value.value();
				term->var = var_val;
			}
			return term;
		}
		else if (auto int_lit = try_consume(TokenType::open_paren))
		{
			std::optional<NodeExpr*> expr = parse_expr();
			if (!expr.has_value()) {
				error_expected("expression");
			}
			bin_expr = false;
			try_consume_err(TokenType::close_paren);
			NodeTermParen* term_paren = m_allocator.emplace<NodeTermParen>();
			term_paren->expr = expr.value();
			NodeTerm* term = m_allocator.emplace<NodeTerm>();
			term->var = term_paren;
			return term;
		}
		else if (auto int_lit = try_consume(TokenType::opposet))
		{
			NodeTermOpposet* val = m_allocator.emplace<NodeTermOpposet>();
			opposet_on = true;
			if (auto val1 = std::get_if<NodeTerm*>(&parse_expr().value()->var))
			{
				if (auto val2 = std::get_if<NodeTermIntVal*>(&(*val1)->var))
				{
					val->var = *val2;
				}
				else if (auto val2 = std::get_if<NodeTermVar*>(&(*val1)->var))
				{
					val->var = *val2;
				}
			}

			opposet_on = false;
			NodeTerm* word = m_allocator.emplace<NodeTerm>();
			word->var = val;
			return word;
		}
		else if (auto int_lit = try_consume(TokenType::open_char))
		{
			NodeWordCharVal* char_val = m_allocator.emplace<NodeWordCharVal>();
			char_val->value = consume().value;
			NodeTerm* word = m_allocator.emplace<NodeTerm>();
			word->var = char_val;
			consume();
			return word;
		}
		else if (auto int_lit = try_consume(TokenType::open_braket))
		{
			NodeTermArray* array = m_allocator.emplace<NodeTermArray>();
			if (auto val = try_consume(TokenType::int_val))
			{
				array->size = val.value().value.value();
			}
			else if(auto val = try_consume(TokenType::variable))
			{
				array->size = val.value().value.value();
			}
			NodeTerm* word = m_allocator.emplace<NodeTerm>();
			try_consume_err(TokenType::close_braket);
			if (try_consume(TokenType::eq))
			{
				if (peek().value().type == TokenType::int_val)
					array->equalType = to_string(peek().value().type);
				else if (peek().value().type == TokenType::character)
					array->equalType = to_string(peek().value().type);
				else if (peek().value().type == TokenType::boolean)
					array->equalType = to_string(peek().value().type);
				else if (peek().value().type == TokenType::variable)
					array->equalType = m_vars.at(peek().value().value.value());
				else if (peek().value().type == TokenType::open_paren)
					array->equalType = to_string(TokenType::integer);

				array->expr = parse_expr().value();
				if (m_visit_count != 0)
				{
					m_visit_counts.push_back(m_visit_count);
				}
				m_visit_count = 0;
			}
			word->var = array;

			return word;
		}

		return {};
	}
	std::optional<NodeExpr*> parse_expr(const int min_prec = 0)
	{
		std::optional<NodeTerm*> term_lhs = parse_term();
		if (!term_lhs.has_value()) {
			return {};
		}

		auto expr_lhs = m_allocator.emplace<NodeExpr>(term_lhs.value());

		if (peek(1).has_value() && peek(1).value().type != TokenType::plus && peek(1).value().type != TokenType::minus)
		{
			while (true && !opposet_on) {
				std::optional<Token> curr_tok = peek();
				std::optional<int> prec;
				if (curr_tok.has_value()) {
					prec = bin_prec(curr_tok->type);
					if (!prec.has_value() || prec < min_prec) {
						break;
					}
				}
				else {
					break;
				}
				bin_expr = true;
				const auto [type, line, value] = consume();
				const int next_min_prec = prec.value() + 1;
				auto expr_rhs = parse_expr(next_min_prec);
				if (!expr_rhs.has_value()) {
					error_expected("expression");
				}
				auto expr = m_allocator.emplace<NodeBinExpr>();
				auto expr_lhs2 = m_allocator.emplace<NodeExpr>();
				if (type == TokenType::plus) {
					expr_lhs2->var = expr_lhs->var;
					auto add = m_allocator.emplace<NodeBinExprAdd>(expr_lhs2, expr_rhs.value());
					expr->var = add;
					m_visit_count++;
				}
				else if (type == TokenType::star) {
					expr_lhs2->var = expr_lhs->var;
					auto multi = m_allocator.emplace<NodeBinExprMulti>(expr_lhs2, expr_rhs.value());
					expr->var = multi;
					m_visit_count++;
				}
				else if (type == TokenType::minus) {
					expr_lhs2->var = expr_lhs->var;
					auto sub = m_allocator.emplace<NodeBinExprSub>(expr_lhs2, expr_rhs.value());
					expr->var = sub;
					m_visit_count++;
				}
				else if (type == TokenType::fslash) {
					expr_lhs2->var = expr_lhs->var;
					auto div = m_allocator.emplace<NodeBinExprDiv>(expr_lhs2, expr_rhs.value());
					expr->var = div;
					m_visit_count++;
				}
				else {
					assert(false);
				}
				expr_lhs->var = expr;
				if (peek().has_value() && (log_prec(peek().value().type).has_value() || peek().value().type == TokenType::AND || peek().value().type == TokenType::OR))
					bin_expr = false;
			}
		}

		std::optional<Token> curr_tok = peek();
		std::optional<int> prec;

		while (true && !bin_expr && !opposet_on)
		{
			curr_tok = peek();
			if (curr_tok.has_value()) {
				prec = log_prec(curr_tok->type);
				if (prec.has_value()) {
					if (prec.value() == 0)
					{
						NodeLogExpr* log_expr = m_allocator.emplace<NodeLogExpr>();
						NodeLogExprGreater* log_expr_greater = m_allocator.emplace<NodeLogExprGreater>();
						log_expr_greater->lhs = m_allocator.emplace<NodeExpr>();
						log_expr_greater->lhs->var = expr_lhs->var;
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;

						consume();
						log_expr_greater->rhs = parse_expr().value();
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;
						if (peek().has_value() && peek().value().type == TokenType::AND)
						{
							consume();
							NodeLogAnd* log_and = m_allocator.emplace<NodeLogAnd>();
							log_and->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_greater->varAnd = log_and;
						}
						else if (peek().has_value() && peek().value().type == TokenType::OR)
						{
							consume();
							NodeLogOr* log_or = m_allocator.emplace<NodeLogOr>();
							log_or->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_greater->varOr = log_or;
						}
						log_expr->var = log_expr_greater;
						expr_lhs->var = log_expr;
					}
					else if (prec.value() == 1)
					{
						NodeLogExpr* log_expr = m_allocator.emplace<NodeLogExpr>();
						NodeLogExprLesser* log_expr_lesser = m_allocator.emplace<NodeLogExprLesser>();
						log_expr_lesser->lhs = m_allocator.emplace<NodeExpr>();
						log_expr_lesser->lhs->var = expr_lhs->var;
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;
						consume();
						log_expr_lesser->rhs = parse_expr().value();
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;
						if (peek().has_value() && peek().value().type == TokenType::AND)
						{
							consume();
							NodeLogAnd* log_and = m_allocator.emplace<NodeLogAnd>();
							log_and->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_lesser->varAnd = log_and;
						}
						else if (peek().has_value() && peek().value().type == TokenType::OR)
						{
							consume();
							NodeLogOr* log_or = m_allocator.emplace<NodeLogOr>();
							log_or->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_lesser->varOr = log_or;
						}
						log_expr->var = log_expr_lesser;
						expr_lhs->var = log_expr;
					}
					else if (prec.value() == 2)
					{
						NodeLogExpr* log_expr = m_allocator.emplace<NodeLogExpr>();
						NodeLogExprEqual* log_expr_equal = m_allocator.emplace<NodeLogExprEqual>();
						log_expr_equal->lhs = m_allocator.emplace<NodeExpr>();
						log_expr_equal->lhs->var = expr_lhs->var;
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;
						consume();
						log_expr_equal->rhs = parse_expr().value();
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;
						if (peek().has_value() && peek().value().type == TokenType::AND)
						{
							consume();
							NodeLogAnd* log_and = m_allocator.emplace<NodeLogAnd>();
							log_and->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_equal->varAnd = log_and;
						}
						else if (peek().has_value() && peek().value().type == TokenType::OR)
						{
							consume();
							NodeLogOr* log_or = m_allocator.emplace<NodeLogOr>();
							log_or->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_equal->varOr = log_or;
						}
						log_expr->var = log_expr_equal;
						expr_lhs->var = log_expr;
					}
					else if (prec.value() == 3)
					{
						NodeLogExpr* log_expr = m_allocator.emplace<NodeLogExpr>();
						NodeLogExprNotEqual* log_expr_notequal = m_allocator.emplace<NodeLogExprNotEqual>();
						log_expr_notequal->lhs = m_allocator.emplace<NodeExpr>();
						log_expr_notequal->lhs->var = expr_lhs->var;
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;
						consume();
						log_expr_notequal->rhs = parse_expr().value();
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;
						if (peek().has_value() && peek().value().type == TokenType::AND)
						{
							consume();
							NodeLogAnd* log_and = m_allocator.emplace<NodeLogAnd>();
							log_and->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_notequal->varAnd = log_and;
						}
						else if (peek().has_value() && peek().value().type == TokenType::OR)
						{
							consume();
							NodeLogOr* log_or = m_allocator.emplace<NodeLogOr>();
							log_or->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_notequal->varOr = log_or;
						}
						log_expr->var = log_expr_notequal;
						expr_lhs->var = log_expr;
					}
					else if (prec.value() == 4)
					{
						NodeLogExpr* log_expr = m_allocator.emplace<NodeLogExpr>();
						NodeLogExprGreaterEqual* log_expr_greater_equal = m_allocator.emplace<NodeLogExprGreaterEqual>();
						log_expr_greater_equal->lhs = m_allocator.emplace<NodeExpr>();
						log_expr_greater_equal->lhs->var = expr_lhs->var;
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;
						consume();
						log_expr_greater_equal->rhs = parse_expr().value();
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;
						if (peek().has_value() && peek().value().type == TokenType::AND)
						{
							consume();
							NodeLogAnd* log_and = m_allocator.emplace<NodeLogAnd>();
							log_and->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_greater_equal->varAnd = log_and;
						}
						else if (peek().has_value() && peek().value().type == TokenType::OR)
						{
							consume();
							NodeLogOr* log_or = m_allocator.emplace<NodeLogOr>();
							log_or->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_greater_equal->varOr = log_or;
						}
						log_expr->var = log_expr_greater_equal;
						expr_lhs->var = log_expr;
					}
					else if (prec.value() == 5)
					{
						NodeLogExpr* log_expr = m_allocator.emplace<NodeLogExpr>();
						NodeLogExprLesserEqual* log_expr_lesser_equal = m_allocator.emplace<NodeLogExprLesserEqual>();
						log_expr_lesser_equal->lhs = m_allocator.emplace<NodeExpr>();
						log_expr_lesser_equal->lhs->var = expr_lhs->var;
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;
						consume();
						log_expr_lesser_equal->rhs = parse_expr().value();
						if (m_visit_count != 0)
						{
							m_visit_counts.push_back(m_visit_count);
						}
						m_visit_count = 0;
						if (peek().has_value() && peek().value().type == TokenType::AND)
						{
							consume();
							NodeLogAnd* log_and = m_allocator.emplace<NodeLogAnd>();
							log_and->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_lesser_equal->varAnd = log_and;
						}
						else if (peek().has_value() && peek().value().type == TokenType::OR)
						{
							consume();
							NodeLogOr* log_or = m_allocator.emplace<NodeLogOr>();
							log_or->lhs = parse_expr().value();
							if (m_visit_count != 0)
							{
								m_visit_counts.push_back(m_visit_count);
							}
							m_visit_count = 0;
							log_expr_lesser_equal->varOr = log_or;
						}
						log_expr->var = log_expr_lesser_equal;
						expr_lhs->var = log_expr;
					}

				}
				else if (curr_tok->type == TokenType::AND && m_tokens[m_index - 1].type == TokenType::close_paren)
				{
					NodeLogExpr* log_expr = m_allocator.emplace<NodeLogExpr>();
					NodeLogExprParen* log_paren = m_allocator.emplace<NodeLogExprParen>();
					log_paren->lhs = m_allocator.emplace<NodeExpr>();
					log_paren->lhs->var = expr_lhs->var;
					if (m_visit_count != 0)
					{
						m_visit_counts.push_back(m_visit_count);
					}
					m_visit_count = 0;
					consume();
					NodeLogAnd* log_and = m_allocator.emplace<NodeLogAnd>();
					log_and->lhs = parse_expr().value();
					if (m_visit_count != 0)
					{
						m_visit_counts.push_back(m_visit_count);
					}
					m_visit_count = 0;
					log_paren->varAnd = log_and;

					log_expr->var = log_paren;
					expr_lhs->var = log_expr;
				}
				else if (curr_tok->type == TokenType::OR && m_tokens[m_index - 1].type == TokenType::close_paren)
				{
					NodeLogExpr* log_expr = m_allocator.emplace<NodeLogExpr>();
					NodeLogExprParen* log_paren = m_allocator.emplace<NodeLogExprParen>();
					log_paren->lhs = m_allocator.emplace<NodeExpr>();
					log_paren->lhs->var = expr_lhs->var;
					if (m_visit_count != 0)
					{
						m_visit_counts.push_back(m_visit_count);
					}
					m_visit_count = 0;
					consume();
					NodeLogOr* log_or = m_allocator.emplace<NodeLogOr>();
					log_or->lhs = parse_expr().value();
					if (m_visit_count != 0)
					{
						m_visit_counts.push_back(m_visit_count);
					}
					m_visit_count = 0;
					log_paren->varOr = log_or;

					log_expr->var = log_paren;
					expr_lhs->var = log_expr;
				}
				else
					break;
			}

		}

		return expr_lhs;
	}
	std::optional<NodeScope*> parse_scope()
	{
		if (!try_consume(TokenType::open_brek).has_value()) {
			return {};
		}
		auto scope = m_allocator.emplace<NodeScope>();
		while (auto stmt = parse_stat()) {
			scope->stmts.push_back(stmt.value());
		}
		try_consume(TokenType::close_brek, "Expected `}`");
		return scope;
	}
	std::optional<NodeIfPred*> parse_if_pred()
	{
		if (try_consume(TokenType::elseifscope)) {
			try_consume(TokenType::open_paren, "Expected `(`");
			const auto elif = m_allocator.alloc<NodeIfPredElif>();
			if (const auto expr = parse_expr()) {
				elif->expr = expr.value();
			}
			else {
				std::cerr << "Expected expression" << std::endl;
				exit(EXIT_FAILURE);
			}
			try_consume(TokenType::close_paren, "Expected `)`");
			if (const auto scope = parse_scope()) {
				elif->scope = scope.value();
			}
			else {
				std::cerr << "Expected scope" << std::endl;
				exit(EXIT_FAILURE);
			}
			elif->pred = parse_if_pred();
			auto pred = m_allocator.emplace<NodeIfPred>(elif);
			return pred;
		}
		if (try_consume(TokenType::elsescope)) {
			auto else_ = m_allocator.alloc<NodeIfPredElse>();
			if (const auto scope = parse_scope()) {
				else_->scope = scope.value();
			}
			else {
				std::cerr << "Expected scope" << std::endl;
				exit(EXIT_FAILURE);
			}
			auto pred = m_allocator.emplace<NodeIfPred>(else_);
			return pred;
		}
		return {};
	}
	std::optional<NodeStat*> parse_stat()
	{
		if (peek().has_value() && peek().value().type == TokenType::ret && peek(1).has_value() &&
			(peek(1).value().type == TokenType::int_val || peek(1).value().type == TokenType::variable))
		{
			consume();
			NodeStat* stat = m_allocator.emplace<NodeStat>();
			NodeStatExit* stat_exit = m_allocator.emplace<NodeStatExit>();
			stat_exit->expr = parse_expr().value();
			if (m_visit_count != 0)
			{
				m_visit_counts.push_back(m_visit_count);
			}
			m_visit_count = 0;
			consume();
			bin_expr = false;
			stat->stat = stat_exit;
			return stat;
		}
		else if (peek().has_value() && (peek().value().type == TokenType::integer || peek().value().type == TokenType::boolean) && peek(1).has_value() && peek(1).value().type == TokenType::variable &&
			peek(2).has_value() && peek(2).value().type == TokenType::eq)
		{
			consume();
			NodeStat* stat = m_allocator.emplace<NodeStat>();
			NodeStatVar* stat_var = m_allocator.emplace<NodeStatVar>();
			stat_var->type = "int";
			std::string type = to_string(peek(2).value().type);

			if (type == "character" || (type == "variable" && m_vars.contains(peek(2).value().value.value()) && m_vars.at(peek(2).value().value.value()) != "integer"))
			{
				std::cerr << "Invalid value!!" << std::endl;
				exit(EXIT_FAILURE);
			}

			stat_var->name = consume().value.value();
			consume();
			stat_var->expr = parse_expr().value();
			if (m_visit_count != 0)
			{
				m_visit_counts.push_back(m_visit_count);
			}
			m_visit_count = 0;
			consume();
			bin_expr = false;

			m_vars[stat_var->name] = "integer";
			stat->stat = stat_var;
			return stat;
		}
		else if (peek().has_value() && (peek().value().type == TokenType::integer || peek().value().type == TokenType::boolean || peek().value().type == TokenType::character) 
			&& peek(1).has_value() && peek(1).value().type == TokenType::variable && peek(2).has_value() && peek(2).value().type == TokenType::open_braket &&
			peek(3).has_value() && peek(3).value().type == TokenType::int_val)
		{
			NodeStat* stat = m_allocator.emplace<NodeStat>();
			NodeStatVar* stat_var = m_allocator.emplace<NodeStatVar>();
			stat_var->type = to_string(consume().type);
			stat_var->name = consume().value.value();
			stat_var->expr = parse_expr().value();
			if (m_visit_count != 0)
			{
				m_visit_counts.push_back(m_visit_count);
			}
			if (auto expr = std::get_if<NodeTerm*>(&stat_var->expr->var))
			{
				if (auto array = std::get_if<NodeTermArray*>(&((*expr)->var)))
				{
					(*array)->type = stat_var->type;
					(*array)->name = stat_var->name;
				}
				
			}
			m_visit_count = 0;
			consume();
			bin_expr = false;

			m_vars[stat_var->name] = stat_var->type;
			stat->stat = stat_var;
			return stat;
		}
		else if (peek().has_value() && peek().value().type == TokenType::variable && peek(1).has_value() && peek(1).value().type == TokenType::eq
			&& peek(2).has_value() && (peek(2).value().type == TokenType::int_val || peek(2).value().type == TokenType::variable || peek(2).value().type == TokenType::open_char || peek(2).value().type == open_paren))
		{
			if ((peek(2).value().value.has_value() && m_vars.contains(peek(2).value().value.value()) &&  m_vars.at(peek().value().value.value()) != m_vars.at(peek(2).value().value.value())) &&
				(to_string(peek(2).value().type) == "variable" && m_vars.at(peek().value().value.value()) != to_string(peek(2).value().type)) && to_string(peek(2).value().type) != "open_paren")
			{
				std::cerr << "Invalid value!!" << std::endl;
				exit(EXIT_FAILURE);
			}

			NodeStat* stat = m_allocator.emplace<NodeStat>();
			NodeStateEq* stat_eq = m_allocator.emplace<NodeStateEq>();
			stat_eq->type = to_string(peek(2).value().type);
			stat_eq->variableName = consume().value.value();
			consume();
			stat_eq->expr = parse_expr().value();
			if (m_visit_count != 0)
			{
				m_visit_counts.push_back(m_visit_count);
			}
			m_visit_count = 0;
			consume();
			bin_expr = false;
			if (stat_eq->type == "character")
			{
				NodeTerm* word = std::get<NodeTerm*>(stat_eq->expr->var);
				NodeWordCharVal* val = std::get<NodeWordCharVal*>(word->var);
				val->name = stat_eq->variableName;
			}

			stat->stat = stat_eq;
			return stat;
		}
		else if (peek().has_value() && peek().value().type == TokenType::variable && peek(1).has_value() && peek(1).value().type == TokenType::open_braket
			 && peek(2).has_value() && (peek(2).value().type == TokenType::int_val || peek(2).value().type == TokenType::variable) && peek(3).has_value() && peek(3).value().type == TokenType::close_braket &&
			    peek(4).has_value() && peek(4).value().type == TokenType::eq)
			{
				NodeStat* stat = m_allocator.emplace<NodeStat>();
				NodeStateEq* stat_eq = m_allocator.emplace<NodeStateEq>();
				stat_eq->variableName = consume().value.value();
				stat_eq->expr = parse_expr().value();
				if (m_visit_count != 0)
				{
					m_visit_counts.push_back(m_visit_count);
				}
				if (auto expr = std::get_if<NodeTerm*>(&stat_eq->expr->var))
				{
					if (auto array = std::get_if<NodeTermArray*>(&((*expr)->var)))
					{
						(*array)->name = stat_eq->variableName;
					}
				}
				m_visit_count = 0;
				consume();
				bin_expr = false;

				stat->stat = stat_eq;
				return stat;
		    }
		else if (peek().has_value() && peek().value().type == TokenType::character && peek(1).has_value() && peek(1).value().type == TokenType::variable &&
			peek(2).has_value() && peek(2).value().type == TokenType::eq)
		{
			consume();
			NodeStat* stat = m_allocator.emplace<NodeStat>();
			NodeStatVar* stat_var = m_allocator.emplace<NodeStatVar>();
			stat_var->type = "character";
			stat_var->name = consume().value.value();

			std::string type = to_string(peek(1).value().type);
			consume();
			stat_var->expr = parse_expr().value();
			if (m_visit_count != 0)
			{
				m_visit_counts.push_back(m_visit_count);
			}
			m_visit_count = 0;

			bin_expr = false;

			if (type == "integer")
			{
				std::cerr << "Invalid value!!" << std::endl;
				exit(EXIT_FAILURE);
			}
			else if (type == "variable")
			{
				NodeTerm* word = std::get<NodeTerm*>(stat_var->expr->var);
				NodeTermVar* val = std::get<NodeTermVar*>(word->var);
				val->eqName = stat_var->name;
				if (m_vars.at(val->name) != "character")
				{
					std::cerr << "Invalid value!!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			stat->stat = stat_var;

			m_vars[stat_var->name] = "character";
			consume();
			return stat;
		}
		else if (peek().has_value() && peek().value().type == TokenType::print && peek(1).has_value() && peek(1).value().type == TokenType::open_paren &&
			peek(2).has_value() && (peek(2).value().type == TokenType::variable || peek(2).value().type == TokenType::int_val || peek(2).value().type == TokenType::open_char))
		{
			consume();
			consume();
			NodeStat* stat = m_allocator.emplace<NodeStat>();
			NodeStatPrint* stat_print = m_allocator.emplace<NodeStatPrint>();
			stat_print->type = to_string(peek().value().type);
			if (peek().value().value.has_value())
				stat_print->variableName = peek().value().value.value();

			stat_print->expr = parse_expr().value();
			if (auto term = std::get_if<NodeTerm*>(&stat_print->expr->var))
			{
				if (auto arr = std::get_if<NodeTermArray*>(&((*term)->var)))
				{
					(*arr)->name = stat_print->variableName.value();
				}
			}

			if (m_visit_count != 0)
			{
				m_visit_counts.push_back(m_visit_count);
			}
			m_visit_count = 0;
			stat->stat = stat_print;
			bin_expr = false;

			if (peek().has_value() && peek().value().type == TokenType::newLine)
			{
				stat_print->newLine = true;
				consume();
			}

			consume();
			consume();
			return stat;
		}
		else if (auto if_ = try_consume(TokenType::ifscope)) {
			try_consume(TokenType::open_paren, "Expected `(`");
			auto stmt_if = m_allocator.emplace<NodeStatIf>();
			if (const auto expr = parse_expr()) {
				stmt_if->expr = expr.value();
				bin_expr = false;
				if (m_visit_count != 0)
				{
					m_visit_counts.push_back(m_visit_count);
				}
				m_visit_count = 0;
			}
			else {
				std::cerr << "Invalid expression" << std::endl;
				exit(EXIT_FAILURE);
			}
			try_consume(TokenType::close_paren, "Expected `)`");
			if (const auto scope = parse_scope()) {
				stmt_if->scope = scope.value();
			}
			else {
				std::cerr << "Invalid scope" << std::endl;
				exit(EXIT_FAILURE);
			}
			stmt_if->pred = parse_if_pred();
			auto stmt = m_allocator.emplace<NodeStat>(stmt_if);
			return stmt;
		}
		else if (peek().has_value() && peek().value().type == TokenType::variable && peek(1).has_value() && peek(1).value().type == TokenType::plus &&
			peek(2).has_value() && peek(2).value().type == TokenType::plus)
		{
			NodeStat* stat = m_allocator.emplace<NodeStat>();
			NodeStatIncerement* inc = m_allocator.emplace<NodeStatIncerement>();
			inc->expr = parse_expr().value();
			consume();
			consume();
			consume();

			NodeTerm* word = std::get<NodeTerm*>(inc->expr->var);
			NodeTermVar* val = std::get<NodeTermVar*>(word->var);
			inc->variableName = val->name;


			stat->stat = inc;
			return stat;
		}
		else if (peek().has_value() && peek().value().type == TokenType::variable && peek(1).has_value() && peek(1).value().type == TokenType::minus &&
			peek(2).has_value() && peek(2).value().type == TokenType::minus)
		{
			NodeStat* stat = m_allocator.emplace<NodeStat>();
			NodeStatDecrement* inc = m_allocator.emplace<NodeStatDecrement>();
			inc->expr = parse_expr().value();
			consume();
			consume();
			consume();

			NodeTerm* word = std::get<NodeTerm*>(inc->expr->var);
			NodeTermVar* val = std::get<NodeTermVar*>(word->var);
			inc->variableName = val->name;


			stat->stat = inc;
			return stat;
		}
		else if (peek().has_value() && peek().value().type == TokenType::INPUT && peek(1).has_value() && peek(1).value().type == TokenType::open_paren &&
			peek(2).has_value() && peek(2).value().type == TokenType::variable)
		{
			consume();
			consume();
			cin_count++;
			NodeStatInput* stat_input = m_allocator.emplace<NodeStatInput>();
			stat_input->expr = parse_expr().value();
			NodeTerm* term = std::get<NodeTerm*>(stat_input->expr->var);
			NodeTermVar* var = std::get<NodeTermVar*>(term->var);
			stat_input->name = var->name;

			NodeStat* stat = m_allocator.emplace<NodeStat>();
			stat->stat = stat_input;
			consume();
			consume();
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
	void error_expected(const std::string& msg) const
	{
		std::cerr << "[Parse Error] Expected " << msg << " on line " << peek(-1).value().value.value() << std::endl;
		exit(EXIT_FAILURE);
	}
	Token try_consume_err(const TokenType type)
	{
		if (peek().has_value() && peek().value().type == type) {
			return consume();
		}
		error_expected(to_string(type));
		return {};
	}
	[[nodiscard]] std::optional<Token> peek(size_t offset = 0) const
	{
		if (m_index + offset >= m_tokens.size()) {
			return {};
		}
		return m_tokens.at(m_index + offset);
	}
	Token try_consume(const TokenType type, const std::string& err_msg)
	{
		if (peek().has_value() && peek().value().type == type) {
			return consume();
		}
		std::cerr << err_msg << std::endl;
		exit(EXIT_FAILURE);
	}
	std::optional<Token> try_consume(const TokenType type)
	{
		if (peek().has_value() && peek().value().type == type) {
			return consume();
		}
		return {};
	}
	Token consume()
	{
		return m_tokens.at(m_index++);
	}
private:
	size_t m_index = 0;
	bool bin_expr = false;
	bool opposet_on = false;
	ArenaAllocator m_allocator;
	std::vector<Token> m_tokens;
public:
	int cin_count = 0;
	int m_visit_count = 0;
	std::vector<int> m_visit_counts;
	std::unordered_map<std::string, std::string> m_vars;
};