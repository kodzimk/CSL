#pragma once
#include<vector>
#include<variant>
#include<string>
#include<fstream>
#include<optional>
#include<iostream>
#include<sstream>
#include<unordered_map>
#include<cassert>
#include <conio.h>
#include<stack>
#include"parser.h"

using namespace std;

class Generator
{
public:
	Generator(NodeProg prog, std::unordered_map<std::string, std::string> m_types, std::vector<int> m_visit_counts, std::vector<int> m_paren_count)
		: prog(prog),
		m_types(m_types),
		m_visit_counts(m_visit_counts),
		m_paren_count(m_paren_count)
	{

	}

public:
	void gen_log_expr(NodeLogExpr* expr)
	{
		struct LogVisitor {
			Generator& gen;
			void operator()(const NodeLogOr* log_or)
			{

			}
			void operator()(const NodeLogAnd* log_and)
			{

			}
			void operator()(const NodeLogExprParen* log_paren)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_paren->lhs);
				gen.m_bin_expr.clear();
				while (gen.temp_log_expr != nullptr)
				{
					gen.gen_expr(gen.temp_log_expr.value());
					gen.is_bin_expr = false;
				}
				if (log_paren->varAnd.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::AND);
					}

					gen.gen_expr(log_paren->varAnd.value()->lhs);
					gen.pop("rax");
					gen.pop("rbx");
					gen.m_output << "    mov rsi, 1\n";
					gen.m_output << "    and rbx, rax\n";
					gen.m_output << "    jne carry_set" << std::to_string(gen.carry_count) << "\n";

					gen.m_output << "    \n";
					gen.m_output << "    mov rsi, 0\n";
					gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
					gen.m_output << "    \n";

					gen.m_output << "mov rax,rsi\n";
					gen.push("rax");
					gen.carry_count++;
				}
				else if (log_paren->varOr.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_paren->varOr.value()->lhs;
					gen.orCount++;
				}
			}
			void operator()(const NodeLogExprGreater* log_greater)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_greater->lhs);
				if (gen.if_stat)
				{
					if (gen.m_bin_expr.size() > 2)
					{
						if (gen.m_bin_expr[0] == '(')
							gen.m_bin_expr.push_back(')');

						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::greater);
				}
				gen.gen_expr(log_greater->rhs);
				if (gen.m_bin_expr.size() > 2)
				{
					if (gen.m_bin_expr[0] == '(')
						gen.m_bin_expr.push_back(')');
					gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
					gen.m_bin_expr.clear();
					gen.is_bin_expr = false;
				}
				gen.pop("rbx");
				gen.pop("rax");
				gen.m_output << "    mov rsi, 1\n";
				gen.m_output << "    cmp rbx, rax\n";
				gen.m_output << "    jc carry_set" << std::to_string(gen.carry_count) << "\n";

				gen.m_output << "    \n";
				gen.m_output << "    mov rsi, 0\n";
				gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
				gen.m_output << "    \n";

				gen.m_output << "mov rax,rsi\n";
				gen.push("rax");
				gen.carry_count++;
				if (log_greater->varAnd.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::AND);
					}

					gen.gen_expr(log_greater->varAnd.value()->lhs);
					if (gen.m_bin_expr.size() > 0)
					{
						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
					}
					gen.pop("rax");
					gen.pop("rbx");
					gen.m_output << "    mov rsi, 1\n";
					gen.m_output << "    and rbx, rax\n";
					gen.m_output << "    jne carry_set" << std::to_string(gen.carry_count) << "\n";

					gen.m_output << "    \n";
					gen.m_output << "    mov rsi, 0\n";
					gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
					gen.m_output << "    \n";

					gen.m_output << "mov rax,rsi\n";
					gen.push("rax");
					gen.carry_count++;
				}
				else if (log_greater->varOr.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_greater->varOr.value()->lhs;
					gen.orCount++;
				}
			}
			void operator()(const NodeLogExprLesser* log_lesser)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_lesser->lhs);
				if (gen.if_stat)
				{
					if (gen.m_bin_expr.size() > 2)
					{
						if (gen.m_bin_expr[0] == '(')
							gen.m_bin_expr.push_back(')');
						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::lesser);
				}
				gen.gen_expr(log_lesser->rhs);
				if (gen.m_bin_expr.size() > 2)
				{
					if (gen.m_bin_expr[0] == '(')
						gen.m_bin_expr.push_back(')');
					gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
					gen.m_bin_expr.clear();
					gen.is_bin_expr = false;
				}
				gen.pop("rbx");
				gen.pop("rax");
				gen.m_output << "    mov rsi, 1\n";
				gen.m_output << "    cmp rax, rbx\n";
				gen.m_output << "    jc carry_set" << std::to_string(gen.carry_count) << "\n";

				gen.m_output << "    \n";
				gen.m_output << "    mov rsi, 0\n";
				gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
				gen.m_output << "    \n";

				gen.m_output << "mov rax,rsi\n";
				gen.push("rax");
				gen.carry_count++;
				if (log_lesser->varAnd.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::AND);
					}
					gen.gen_expr(log_lesser->varAnd.value()->lhs);

					gen.pop("rax");
					gen.pop("rbx");
					gen.m_output << "    mov rsi, 1\n";
					gen.m_output << "    and rbx, rax\n";
					gen.m_output << "    jne carry_set" << std::to_string(gen.carry_count) << "\n";

					gen.m_output << "    \n";
					gen.m_output << "    mov rsi, 0\n";
					gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
					gen.m_output << "    \n";

					gen.m_output << "mov rax,rsi\n";
					gen.push("rax");
					gen.carry_count++;
				}
				else if (log_lesser->varOr.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_lesser->varOr.value()->lhs;
					gen.orCount++;
				}
			}
			void operator()(const NodeLogExprEqual* log_equal)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_equal->lhs);
				if (gen.if_stat)
				{
					if (gen.m_bin_expr.size() > 2)
					{
						if (gen.m_bin_expr[0] == '(')
							gen.m_bin_expr.push_back(')');
						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::equal);
				}
				gen.gen_expr(log_equal->rhs);
				if (gen.m_bin_expr.size() > 2)
				{
					if (gen.m_bin_expr[0] == '(')
						gen.m_bin_expr.push_back(')');
					gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
					gen.m_bin_expr.clear();
					gen.is_bin_expr = false;
				}
				gen.pop("rbx");
				gen.pop("rax");
				gen.m_output << "    mov rsi, 1\n";
				gen.m_output << "    cmp rax, rbx\n";
				gen.m_output << "    je carry_set" << std::to_string(gen.carry_count) << "\n";

				gen.m_output << "    \n";
				gen.m_output << "    mov rsi, 0\n";
				gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
				gen.m_output << "    \n";

				gen.m_output << "mov rax,rsi\n";
				gen.push("rax");
				gen.carry_count++;
				if (log_equal->varAnd.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::AND);
					}
					gen.gen_expr(log_equal->varAnd.value()->lhs);

					gen.pop("rax");
					gen.pop("rbx");
					gen.m_output << "    mov rsi, 1\n";
					gen.m_output << "    and rbx, rax\n";
					gen.m_output << "    jne carry_set" << std::to_string(gen.carry_count) << "\n";

					gen.m_output << "    \n";
					gen.m_output << "    mov rsi, 0\n";
					gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
					gen.m_output << "    \n";

					gen.m_output << "mov rax,rsi\n";
					gen.push("rax");
					gen.carry_count++;
				}
				else if (log_equal->varOr.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_equal->varOr.value()->lhs;
					gen.orCount++;
				}
			}
			void operator()(const NodeLogExprNotEqual* log_not_equal)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_not_equal->lhs);
				if (gen.if_stat)
				{
					if (gen.m_bin_expr.size() > 2)
					{
						if (gen.m_bin_expr[0] == '(')
							gen.m_bin_expr.push_back(')');
						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::notequal);
				}
				gen.gen_expr(log_not_equal->rhs);
				if (gen.m_bin_expr.size() > 2)
				{
					if (gen.m_bin_expr[0] == '(')
						gen.m_bin_expr.push_back(')');
					gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
					gen.m_bin_expr.clear();
					gen.is_bin_expr = false;
				}
				gen.pop("rbx");
				gen.pop("rax");
				gen.m_output << "    mov rsi, 1\n";
				gen.m_output << "    cmp rax, rbx\n";
				gen.m_output << "    jne carry_set" << std::to_string(gen.carry_count) << "\n";

				gen.m_output << "    \n";
				gen.m_output << "    mov rsi, 0\n";
				gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
				gen.m_output << "    \n";

				gen.m_output << "mov rax,rsi\n";
				gen.push("rax");
				gen.carry_count++;
				if (log_not_equal->varAnd.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::AND);
					}
					gen.gen_expr(log_not_equal->varAnd.value()->lhs);

					gen.pop("rax");
					gen.pop("rbx");
					gen.m_output << "    mov rsi, 1\n";
					gen.m_output << "    and rbx, rax\n";
					gen.m_output << "    jne carry_set" << std::to_string(gen.carry_count) << "\n";

					gen.m_output << "    \n";
					gen.m_output << "    mov rsi, 0\n";
					gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
					gen.m_output << "    \n";

					gen.m_output << "mov rax,rsi\n";
					gen.push("rax");
					gen.carry_count++;
				}
				else if (log_not_equal->varOr.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_not_equal->varOr.value()->lhs;
					gen.orCount++;
				}

			}
			void operator()(const NodeLogExprGreaterEqual* log_greater_equal)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_greater_equal->lhs);
				if (gen.if_stat)
				{
					if (gen.m_bin_expr[0] == '(')
						gen.m_bin_expr.push_back(')');
					if (gen.m_bin_expr.size() > 2)
					{
						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::greaterequal);
				}
				gen.gen_expr(log_greater_equal->rhs);
				if (gen.m_bin_expr.size() > 2)
				{
					if (gen.m_bin_expr[0] == '(')
						gen.m_bin_expr.push_back(')');
					gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
					gen.m_bin_expr.clear();
					gen.is_bin_expr = false;
				}
				gen.pop("rbx");
				gen.pop("rax");
				gen.m_output << "    mov rsi, 1\n";
				gen.m_output << "    cmp rax, rbx\n";
				gen.m_output << "    jge carry_set" << std::to_string(gen.carry_count) << "\n";

				gen.m_output << "    \n";
				gen.m_output << "    mov rsi, 0\n";
				gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
				gen.m_output << "    \n";

				gen.m_output << "mov rax,rsi\n";
				gen.push("rax");
				gen.carry_count++;
				if (log_greater_equal->varAnd.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::AND);
					}
					gen.gen_expr(log_greater_equal->varAnd.value()->lhs);

					gen.pop("rax");
					gen.pop("rbx");
					gen.m_output << "    mov rsi, 1\n";
					gen.m_output << "    and rbx, rax\n";
					gen.m_output << "    jne carry_set" << std::to_string(gen.carry_count) << "\n";

					gen.m_output << "    \n";
					gen.m_output << "    mov rsi, 0\n";
					gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
					gen.m_output << "    \n";

					gen.m_output << "mov rax,rsi\n";
					gen.push("rax");
					gen.carry_count++;
				}
				else if (log_greater_equal->varOr.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_greater_equal->varOr.value()->lhs;
					gen.orCount++;
				}
			}
			void operator()(const NodeLogExprLesserEqual* log_lesser_equal)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_lesser_equal->lhs);
				if (gen.if_stat)
				{
					if (gen.m_bin_expr[0] == '(')
						gen.m_bin_expr.push_back(')');
					if (gen.m_bin_expr.size() > 2)
					{
						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::lesserequal);
				}
				gen.gen_expr(log_lesser_equal->rhs);
				if (gen.m_bin_expr.size() > 2)
				{
					if (gen.m_bin_expr[0] == '(')
						gen.m_bin_expr.push_back(')');
					gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
					gen.m_bin_expr.clear();
					gen.is_bin_expr = false;
				}
				gen.pop("rbx");
				gen.pop("rax");
				gen.m_output << "    mov rsi, 1\n";
				gen.m_output << "    cmp rbx, rax\n";
				gen.m_output << "    jge carry_set" << std::to_string(gen.carry_count) << "\n";

				gen.m_output << "    \n";
				gen.m_output << "    mov rsi, 0\n";
				gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
				gen.m_output << "    \n";

				gen.m_output << "mov rax,rsi\n";
				gen.push("rax");
				gen.carry_count++;
				if (log_lesser_equal->varAnd.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::AND);
					}
					gen.gen_expr(log_lesser_equal->varAnd.value()->lhs);

					gen.pop("rax");
					gen.pop("rbx");
					gen.m_output << "    mov rsi, 1\n";
					gen.m_output << "    and rbx, rax\n";
					gen.m_output << "    jne carry_set" << std::to_string(gen.carry_count) << "\n";

					gen.m_output << "    \n";
					gen.m_output << "    mov rsi, 0\n";
					gen.m_output << "carry_set" << std::to_string(gen.carry_count) << ":\n";
					gen.m_output << "    \n";

					gen.m_output << "mov rax,rsi\n";
					gen.push("rax");
					gen.carry_count++;
				}
				else if (log_lesser_equal->varOr.has_value())
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_lesser_equal->varOr.value()->lhs;
					gen.orCount++;
				}
			}
		};

		LogVisitor visitor = { .gen = *this };
		std::visit(visitor, expr->var);
	}
	void gen_term(NodeTerm* term)
	{
		struct TermVisitor {
			Generator& gen;
			void operator()(const NodeTermIntVal* int_val)
			{
				if (gen.if_stat && !gen.is_bin_expr)
				{
					gen.if_expr.push_back(stoi(int_val->value.value()));
				}
				gen.value = stoi(int_val->value.value());
				gen.m_output << "   mov rax," << int_val->value.value() << '\n';
				gen.push("rax");
			}
			void operator()(const NodeWordCharVal* char_val)
			{
				gen.m_output << "   mov rax,'" << char_val->value.value()<<"'" << '\n';
				gen.push("rax");
			}
			void operator()(const NodeTermOpposet* term_op)
			{
				if (auto int_val = std::get_if<NodeTermIntVal*>(&term_op->var))
				{
					if (stoi((*int_val)->value.value()) != 0)
					{
						gen.m_output << "   mov rax,0" << '\n';
						if (gen.if_stat && !gen.is_bin_expr)
						{
							gen.if_expr.push_back(0);
						}
					}
					else
					{
						gen.m_output << "   mov rax,1" << '\n';
						if (gen.if_stat && !gen.is_bin_expr)
						{
							gen.if_expr.push_back(1);
						}
					}
					gen.push("rax");
				}
				else if (auto var = std::get_if<NodeTermVar*>(&term_op->var))
				{
					if (gen.m_int_vars.contains((*var)->name))
					{
						if (gen.if_stat && !gen.is_bin_expr)
						{
							if (gen.m_int_values.at((*var)->name) != 0)
							{
								gen.if_expr.push_back(0);
							}
							else
							{
								gen.if_expr.push_back(1);
							}
						}
						gen.value = gen.m_int_values.at((*var)->name);
						if (gen.m_int_values.at((*var)->name) == 0)
						{
							gen.m_output << "mov rax,1\n";
							gen.push("rax");
						}
						else
						{
							gen.m_output << "mov rax,0\n";
							gen.push("rax");
						}
					}
				}
				else
				{
					std::cerr << "Invalid Expr!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			void operator()(const NodeTermVar* term_var)
			{
				if (gen.m_int_vars.contains(term_var->name))
				{
					if (gen.if_stat && !gen.is_bin_expr)
					{
						gen.if_expr.push_back(gen.m_int_values.at(term_var->name));
					}
					gen.value = gen.m_int_values.at(term_var->name);
					const auto& var = gen.m_int_vars.at(term_var->name);
					std::stringstream offset;
					offset << "QWORD [rsp + " << (gen.m_stack_size - var - 1) * 8 << "]\n";
					gen.push(offset.str());
				}
				else if (gen.m_char_vars.contains(term_var->name))
				{
					const auto& var = gen.m_char_vars.at(term_var->name);
					std::stringstream offset;
					offset << "QWORD [rsp + " << (gen.m_stack_size - var - 1) * 8 << "]\n";
					gen.push(offset.str());
				}
				else
				{
					gen.value = stoi(term_var->value.value());
					gen.m_output << "   mov rax, " << term_var->value.value() << '\n';
					gen.push("rax");

				}
			}
			void operator()(NodeTermParen* term_paren)
			{
				gen.if_expr.push_back(TokenType::open_paren);
				gen.current_paren++;
				gen.count_paren = true;
				gen.m_bin_expr += "(";
				gen.gen_expr(term_paren->expr);
				gen.m_bin_expr += ")";
				gen.if_expr.push_back(TokenType::close_paren);
				gen.m_paren_count.erase(gen.m_paren_count.begin() + gen.current_paren);
				gen.current_paren--;
				gen.count_paren = false;
			}
			void operator()(NodeLogExpr* log_expr)
			{
				gen.gen_log_expr(log_expr);
			}
		};
		TermVisitor visitor = { .gen = *this };
		std::visit(visitor, term->var);
	}
	void gen_bin_expr(NodeBinExpr* bin_expr)
	{
		is_bin_expr = true;
		struct BinExprVisitor {
			Generator& gen;

			void operator()(const NodeBinExprSub* sub)
			{
				gen.gen_expr(sub->lhs);
				if (gen.m_bin_expr.size() > 0)
				{
					for (int i = gen.m_bin_expr.size() - 1; i >= 0; i++)
					{
						if (gen.m_bin_expr[i] != ' ' && gen.m_bin_expr[i] != ')')
						{
							gen.m_bin_expr += std::to_string(gen.value.value());
							break;
						}
						else if (gen.m_bin_expr[i] == ')')
							break;
					}
				}
				else
					gen.m_bin_expr += std::to_string(gen.value.value());

				gen.m_bin_expr += "-";
				gen.value.reset();
				gen.gen_expr(sub->rhs);
				gen.m_visit_counts[0]--;
				if (gen.count_paren)
					gen.m_paren_count[gen.current_paren]--;

				if (gen.m_visit_counts[0] == 0 || (gen.m_paren_count.size() > 0 && gen.m_paren_count[gen.current_paren] == 0))
				{
					gen.m_bin_expr += std::to_string(gen.value.value());
					if (gen.m_visit_counts[0] == 0)
						gen.m_visit_counts.erase(gen.m_visit_counts.begin());
				}


				gen.pop("rbx");
				gen.pop("rax");
				gen.m_output << "    sub rax, rbx\n";
				gen.push("rax");
			}
			void operator()(const NodeBinExprAdd* add)
			{
				gen.gen_expr(add->lhs);
				if (gen.m_bin_expr.size() > 0)
				{
					for (int i = gen.m_bin_expr.size() - 1; i >= 0; i++)
					{
						if (gen.m_bin_expr[i] != ' ' && gen.m_bin_expr[i] != ')')
						{
							gen.m_bin_expr += std::to_string(gen.value.value());
							break;
						}
						else if (gen.m_bin_expr[i] == ')')
							break;
					}
				}
				else
					gen.m_bin_expr += std::to_string(gen.value.value());
				gen.m_bin_expr += "+";
				gen.value.reset();
				gen.gen_expr(add->rhs);

				gen.m_visit_counts[0]--;
				if (gen.count_paren)
					gen.m_paren_count[gen.current_paren]--;

				if (gen.m_visit_counts[0] == 0 || (gen.m_paren_count.size() > 0 && gen.m_paren_count[gen.current_paren] == 0))
				{
					gen.m_bin_expr += std::to_string(gen.value.value());
					if (gen.m_visit_counts[0] == 0)
						gen.m_visit_counts.erase(gen.m_visit_counts.begin());
				}

				gen.pop("rbx");
				gen.pop("rax");
				gen.m_output << "    add rax, rbx\n";
				gen.push("rax");
			}
			void operator()(const NodeBinExprMulti* multi)
			{
				gen.gen_expr(multi->lhs);
				if (gen.m_bin_expr.size() > 0)
				{
					for (int i = gen.m_bin_expr.size() - 1; i >= 0; i++)
					{
						if (gen.m_bin_expr[i] != ' ' && gen.m_bin_expr[i] != ')')
						{
							gen.m_bin_expr += std::to_string(gen.value.value());
							break;
						}
						else if (gen.m_bin_expr[i] == ')')
							break;
					}
				}
				else
					gen.m_bin_expr += std::to_string(gen.value.value());
				gen.m_bin_expr += "*";
				gen.value.reset();
				gen.gen_expr(multi->rhs);
				gen.m_visit_counts[0]--;

				if (gen.count_paren)
					gen.m_paren_count[gen.current_paren]--;

				if (gen.m_visit_counts[0] == 0 || (gen.m_paren_count.size() > 0 && gen.m_paren_count[gen.current_paren] == 0))
				{
					gen.m_bin_expr += std::to_string(gen.value.value());
					if (gen.m_visit_counts[0] == 0)
						gen.m_visit_counts.erase(gen.m_visit_counts.begin());
				}
				gen.pop("rbx");
				gen.pop("rax");
				gen.m_output << "    mul rbx\n";
				gen.push("rax");
			}
			void operator()(const NodeBinExprDiv* div)
			{
				gen.gen_expr(div->lhs);
				if (gen.m_bin_expr.size() > 0)
				{
					for (int i = gen.m_bin_expr.size() - 1; i >= 0; i++)
					{
						if (gen.m_bin_expr[i] != ' ' && gen.m_bin_expr[i] != ')')
						{
							gen.m_bin_expr += std::to_string(gen.value.value());
							break;
						}
						else if (gen.m_bin_expr[i] == ')')
							break;
					}
				}
				else
					gen.m_bin_expr += std::to_string(gen.value.value());
				gen.m_bin_expr += "/";
				gen.value.reset();
				gen.gen_expr(div->rhs);
				gen.m_visit_counts[0]--;

				if (gen.count_paren)
					gen.m_paren_count[gen.current_paren]--;

				if (gen.m_visit_counts[0] == 0 || (gen.m_paren_count.size() > 0 && gen.m_paren_count[gen.current_paren] == 0))
				{
					gen.m_bin_expr += std::to_string(gen.value.value());
					if (gen.m_visit_counts[0] == 0)
						gen.m_visit_counts.erase(gen.m_visit_counts.begin());
				}
				gen.pop("rbx");
				gen.pop("rax");
				gen.m_output << "    div rbx\n";
				gen.push("rax");
			}
		};

		BinExprVisitor visitor{ .gen = *this };
		std::visit(visitor, bin_expr->var);
	}
	void gen_expr(NodeExpr* expr)
	{
		struct ExprVisitor {
			Generator& gen;
			void operator()(NodeTerm* term)
			{
				gen.gen_term(term);
			}
			void operator()(NodeBinExpr* bin_expr)
			{
				gen.gen_bin_expr(bin_expr);
			}
			void operator()(NodeLogExpr* log_expr)
			{
				gen.gen_log_expr(log_expr);
			}
		};
		ExprVisitor visitor = { .gen = *this };
		std::visit(visitor, expr->var);
	}
	void gen_scope(const NodeScope* scope)
	{
		begin_scope();
		for (NodeStat* stmt : scope->stmts) {
			gen_stat(stmt);
		}
		end_scope();
	}
	void gen_if_pred(const NodeIfPred* pred, const std::string& end_label)
	{
		struct PredVisitor {
			Generator& gen;
			const std::string& end_label;

			void operator()(const NodeIfPredElif* elif) const
			{
				gen.if_stat = true;
				gen.gen_expr(elif->expr);
				gen.is_bin_expr = false;
				while (gen.temp_log_expr != nullptr)
				{
					gen.gen_expr(gen.temp_log_expr.value());
					gen.is_bin_expr = false;
				}
				gen.if_stat = false;
				gen.log_expr_or();
				gen.pop("rax");
				const std::string label = gen.create_label();
				gen.m_output << "    cmp rax,0\n";
				gen.m_output << "    je " << label << "\n";

				gen.m_output << " \n";
				gen.parse_log_expr();
				if (gen.m_values[0] == 1)
				{
					gen.m_values.clear();
					gen.m_output << label << ":\n";
					gen.gen_scope(elif->scope);
					gen.m_bin_expr.clear();
				}
				else if (elif->pred.has_value()) {
					gen.m_values.clear();
					const std::string end_label = gen.create_label();
					gen.m_output << "    jmp " << end_label << "\n";
					gen.m_output << label << ":\n";
					gen.gen_if_pred(elif->pred.value(), end_label);
					gen.m_output << end_label << ":\n";
					gen.m_bin_expr.clear();
				}
				else {
					gen.m_output << label << ":\n";
				}
				gen.m_values.clear();
				gen.m_output << "  \n";
				gen.m_bin_expr.clear();
			}
			void operator()(const NodeIfPredElse* else_) const
			{
				gen.gen_scope(else_->scope);
			}
		};

		PredVisitor visitor{ .gen = *this, .end_label = end_label };
		std::visit(visitor, pred->var);
	}
	void gen_stat(NodeStat* stat)
	{
		struct StatVisitor {
			Generator& gen;
			void operator()(const NodeStatExit* stat_exit)
			{
				gen.gen_expr(stat_exit->expr);
				gen.is_bin_expr = false;
				gen.pop("rdi");
				gen.m_output << "    jmp exit\n";
			}
			void operator()(const NodeStatPrint* stat_print)
			{
                if (stat_print->type == "character" || gen.m_char_vars.contains(stat_print->variableName.value()))
				{
					gen.gen_expr(stat_print->expr);
					gen.pop("rax");

					gen.m_output << "    mov [temp]," << "rax" << "\n";
					gen.m_output << "    mov rax,1" << "\n";
					gen.m_output << "    mov rsi,temp" << "\n";
					gen.m_output << "    mov rdi,1" << "\n";
					gen.m_output << "    mov rdx,1" << "\n";
					gen.m_output << "    syscall\n";
					gen.m_output << "    \n";
				}
				else if (stat_print->type == "integer" || gen.m_int_vars.contains(stat_print->variableName.value()))
				{
					gen.gen_expr(stat_print->expr);
					gen.is_bin_expr = false;
					gen.pop("rax");
					gen.m_output << "    call _printnumberRAX\n";
				}
				else
				{
					std::cerr << "Invalid expr!!" << std::endl;
					exit(EXIT_FAILURE);
				}

				if (stat_print->newLine)
				{
					gen.m_output << "    mov edx,newLineLen" << "\n";
					gen.m_output << "    mov ecx,newLineMsg" << "\n";
					gen.m_output << "    mov ebx,1" << "\n";
					gen.m_output << "    mov eax,4" << "\n";
					gen.m_output << "    int 0x80\n";
					gen.m_output << "    \n";
				}
			}
			void operator()(const NodeStatVar* stat_var)
			{
				if (!gen.m_int_vars.contains(stat_var->name) && !gen.m_char_vars.contains(stat_var->name))
				{
					gen.m_cur_var = stat_var->name;
					gen.gen_expr(stat_var->expr);
					gen.is_bin_expr = false;
					if (!gen.m_bin_expr.empty())
					{
						gen.m_int_values[stat_var->name] = gen.value.value();
						gen.parse_bin_expr();
					}
					else if (stat_var->type != "character")
						gen.m_int_values[stat_var->name] = gen.value.value();
					

					if (stat_var->type != "character")
					{
						gen.m_int_vars[stat_var->name] = gen.m_stack_size - 1;
						if (gen.temp_vars)
						{
							gen.m_int_name.push_back(stat_var->name);
						}
					}
					else if (stat_var->type == "character")
					{
						gen.m_char_vars[stat_var->name] = gen.m_stack_size - 1;
						if (gen.temp_vars)
						{
							gen.m_char_name.push_back(stat_var->name);
						}
					}

					gen.m_bin_expr.clear();
				}
				else
				{
					std::cerr << "Variable Exist!!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			void operator()(const NodeStateEq* stat_eq)
			{
				if (gen.m_int_vars.contains(stat_eq->variableName) || gen.m_char_vars.contains(stat_eq->variableName))
				{
					gen.m_cur_var = stat_eq->variableName;
					gen.gen_expr(stat_eq->expr);
					gen.is_bin_expr = false;

					if (!gen.m_bin_expr.empty())
						gen.parse_bin_expr();
					else if(gen.m_int_values.contains(stat_eq->variableName))
						gen.m_int_values.at(stat_eq->variableName) = gen.value.value();

					if (gen.m_int_vars.contains(stat_eq->variableName))
					{
						gen.m_int_vars.at(stat_eq->variableName) = gen.m_stack_size - 1;
						gen.value = NULL;
					}
					else if (gen.m_char_vars.contains(stat_eq->variableName))
					{
						gen.m_char_vars.at(stat_eq->variableName) = gen.m_stack_size - 1;
					}
				}
				else
				{
					std::cerr << "Cant find variable not declared!!!" << std::endl;
					exit(EXIT_FAILURE);
				}
				gen.m_bin_expr.clear();
			}
			void operator()(const NodeScope* scope) const
			{
				gen.gen_scope(scope);
				gen.is_bin_expr = false;
			}
			void operator()(const NodeStatIf* stmt_if) const
			{
				gen.if_stat = true;
				gen.gen_expr(stmt_if->expr);
				gen.is_bin_expr = false;
				while (gen.temp_log_expr != nullptr)
				{
					gen.gen_expr(gen.temp_log_expr.value());
					gen.is_bin_expr = false;
				}
				gen.if_stat = false;

				gen.log_expr_or();

				gen.pop("rax");
				const std::string label = gen.create_label();
				gen.m_output << "    cmp rax,0\n";
				gen.m_output << "    je " << label << "\n";
				gen.m_output << " \n";

				gen.parse_log_expr();
				gen.m_bin_expr.clear();
				if (gen.m_values[0] == 1)
				{
					gen.m_values.clear();
					gen.gen_scope(stmt_if->scope);
					gen.m_output << label << ":\n";
				}
				else if (stmt_if->pred.has_value()) {
					gen.m_values.clear();
					const std::string end_label = gen.create_label();
					gen.m_output << "    jmp " << end_label << "\n";
					gen.m_output << label << ":\n";
					gen.gen_if_pred(stmt_if->pred.value(), end_label);
					gen.m_bin_expr.clear();
					gen.m_output << end_label << ":\n";
				}
				else {
					gen.m_output << label << ":\n";
				}
				gen.m_output << "  \n";
				gen.m_values.clear();
				gen.m_bin_expr.clear();
			}
			void operator()(const NodeStatIncerement* inc)
			{
				gen.gen_expr(inc->expr);
				gen.pop("rax");
				gen.m_output << "   inc rax\n";
				gen.push("rax");

				if (gen.m_int_vars.contains(inc->variableName))
				{
					gen.m_int_vars.at(inc->variableName) = gen.m_stack_size - 1;
					int value = gen.m_int_values.at(inc->variableName);
					value++;
					gen.m_int_values.at(inc->variableName) = value;
					gen.value = NULL;
				}
				else
				{
					std::cerr << "Cant findd variable!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			void operator()(const NodeStatDecrement* dec)
			{
				gen.gen_expr(dec->expr);
				gen.pop("rax");
				gen.m_output << "   dec rax\n";
				gen.push("rax");

				if (gen.m_int_vars.contains(dec->variableName))
				{
					gen.m_int_vars.at(dec->variableName) = gen.m_stack_size - 1;
					int value = gen.m_int_values.at(dec->variableName);
					value--;
					gen.m_int_values.at(dec->variableName) = value;
					gen.value = NULL;
				}
				else
				{
					std::cerr << "Cant findd variable!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			void operator()(const NodeStatInput* stat_input)
			{
				if (gen.m_int_vars.contains(stat_input->name))
				{
					gen.m_output << "    mov rax,0\n";
					gen.m_output << "    mov rdi,0\n";
					gen.m_output << "    mov rsi,inputNumbers\n";
					gen.m_output << "    mov rdx,1\n";
					gen.m_output << "    syscall\n";
					std::cout << "Press value that you going to press during compelation:" << std::endl;
					gen.m_int_values.at(stat_input->name) = _getch() - 48;
						
					gen.m_output << "    mov rax,[inputNumbers]\n";
					gen.m_output << "    sub rax,48\n";
					gen.push("rax");
					gen.m_int_vars.at(stat_input->name) = gen.m_stack_size - 1;
				}
				else if (gen.m_char_vars.contains(stat_input->name))
				{
					gen.m_output << "    mov rax,0\n";
					gen.m_output << "    mov rdi,0\n";
					gen.m_output << "    mov rsi,inputNumbers\n";
					gen.m_output << "    mov rdx,255\n";
					gen.m_output << "    syscall\n";

					gen.m_output << "    mov rax,[inputNumbers]\n";
					gen.push("rax");
					gen.m_char_vars.at(stat_input->name) = gen.m_stack_size - 1;
				}
				else
				{
					std::cerr << "Cant findd variable!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		};
		StatVisitor visitor = { .gen = *this };
		std::visit(visitor, stat->stat);
	}

	std::string gen_prog()
	{
		m_bss << "section .bss\n stringBuffer resb 100\nstringBufferPos resb 8\ninputNumbers resb 8\n";
		m_data << "section .data\n";
		m_data << "newLineMsg db 0xA, 0xD\n";
		m_data << "newLineLen equ $ - newLineMsg\n";
		m_data << "temp db 'a',0xA,0xD\n";


		m_output << "section .text\n";
		m_output << "global _start\n";
		m_output << "_start:\n";


		for (int i = 0; i < prog.stats.size(); i++)
		{
			gen_stat(prog.stats[i]);
		}

		m_output << "exit:\n";
		m_output << "    mov rax, 60\n";
		m_output << "    syscall\n";
		m_output << "_printnumberRAX:\n";
		m_output << "mov rcx, stringBuffer\n";
		m_output << "mov rbx, 10\n";
		m_output << "mov[rcx], rbx\n";
		m_output << "inc rcx\n";
		m_output << "mov[stringBufferPos], rcx\n";
		m_output << "_printnumberRAXLoop :\n";
		m_output << "mov rdx, 0\n";
		m_output << "mov rbx, 10\n";
		m_output << "div rbx\n";
		m_output << "push rax\n";
		m_output << "add rdx, 48\n";
		m_output << "mov rcx, [stringBufferPos]\n";
		m_output << "mov[rcx], dl\n";
		m_output << "inc rcx\n";
		m_output << "mov[stringBufferPos], rcx\n";
		m_output << "pop rax;\n";
		m_output << "cmp rax, 0\n";
		m_output << "jne _printnumberRAXLoop\n";
		m_output << "_printnumberRAXLoop2 :\n";
		m_output << "mov rcx, [stringBufferPos]\n";
		m_output << "mov rax, 1\n";
		m_output << "mov rdi, 1\n";
		m_output << "mov rsi, rcx\n";
		m_output << "mov rdx, 1\n";
		m_output << "syscall\n";
		m_output << "mov rcx, [stringBufferPos]\n";
		m_output << "dec rcx\n";
		m_output << "mov[stringBufferPos], rcx\n";
		m_output << "cmp rcx, stringBuffer\n";
		m_output << "jge _printnumberRAXLoop2\n";
		m_output << "ret\n";

		std::string output = m_bss.str();
		output += '\n';
		output += m_data.str();
		output += '\n';
		output += m_output.str();
		return output;
	}
private:
	int precedence(char op) {
		if (op == '+' || op == '-')
			return 1;
		if (op == '*' || op == '/')
			return 2;
		return 0;
	}
	int applyOp(int a, int b, char op) {
		switch (op) {
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '/': return a / b;
		}
	}
	int evaluate(string tokens) {
		int i;

		stack <int> values;
		stack <char> ops;

		for (i = 0; i < tokens.length(); i++) {
			if (tokens[i] == ' ')
				continue;
			else if (tokens[i] == '(') {
				ops.push(tokens[i]);
			}

			else if (isdigit(tokens[i])) {
				int val = 0;
				while (i < tokens.length() &&
					isdigit(tokens[i]))
				{
					val = (val * 10) + (tokens[i] - '0');
					i++;
				}

				values.push(val);
				i--;
			}
			else if (tokens[i] == ')')
			{
				while (!ops.empty() && ops.top() != '(')
				{
					int val2 = values.top();
					values.pop();

					int val1 = values.top();
					values.pop();

					char op = ops.top();
					ops.pop();

					values.push(applyOp(val1, val2, op));
				}

				if (!ops.empty())
					ops.pop();
			}
			else
			{

				while (!ops.empty() && precedence(ops.top())
					>= precedence(tokens[i])) {
					int val2 = values.top();
					values.pop();

					int val1 = values.top();
					values.pop();

					char op = ops.top();
					ops.pop();

					values.push(applyOp(val1, val2, op));
				}
				ops.push(tokens[i]);
			}
		}

		while (!ops.empty()) {
			int val2 = values.top();
			values.pop();

			int val1 = values.top();
			values.pop();

			char op = ops.top();
			ops.pop();

			values.push(applyOp(val1, val2, op));
		}

		return values.top();
	}
	void push(std::string reg)
	{
		m_output << "   push " << reg << '\n';
		m_stack_size++;
	}
	void pop(std::string reg)
	{
		m_output << "    pop " << reg << '\n';
		m_stack_size--;
	}
	void begin_scope()
	{
		m_last_index_of_scope.push_back(m_stack_size);
		temp_vars = true;
	}
	void end_scope()
	{
		for (int i = m_last_index_of_scope[m_last_index_of_scope.size() - 1]; i < m_stack_size + 1; i++)
		{
			if (m_int_name.size() > 0)
			{
				m_int_vars.erase(m_int_name[m_int_name.size() - 1]);
				m_int_name.erase(m_int_name.end() - 1);
			}
			else if (m_char_name.size() > 0)
			{
				m_char_vars.erase(m_char_name[m_char_name.size() - 1]);
				m_char_name.erase(m_char_name.end() - 1);
			}
			pop("rax");
		}
		m_last_index_of_scope.erase(m_last_index_of_scope.end() - 1);
		temp_vars = false;
	}
	std::string create_label()
	{
		static int label_count = 0;
		std::stringstream ss;
		ss << "label" << label_count;
		label_count++;
		return ss.str();
	}
	void log_expr_or()
	{
		for (int i = 0; i < orCount; i++)
		{
			pop("rax");
			pop("rbx");
			m_output << "    mov rsi, 1\n";
			m_output << "    or rax, rbx\n";
			m_output << "    jne carry_set" << std::to_string(carry_count) << "\n";

			m_output << "    \n";
			m_output << "    mov rsi, 0\n";
			m_output << "    carry_set" << std::to_string(carry_count) << ":\n";
			m_output << "    \n";

			m_output << "mov rax,rsi\n";
			push("rax");
			carry_count++;
		}
		orCount = 0;
	}
	void parse_log_expr()
	{
		if (if_expr.size() > 1)
		{
			bool is_paren = false;
			TokenType andOr = TokenType::integer;
			for (int i = 0; i < if_expr.size() - 2; i++)
			{
				if (auto lhs = std::get_if<int>(&if_expr[i]))
				{
					i++;
					if (auto log = std::get_if<TokenType>(&if_expr[i]))
					{
						int log_val = -1;
						if (log_prec(*log).has_value())
						{
							log_val = log_prec(*log).value();
							i++;
							if (log_val == 0)
							{
								if (auto rhs = std::get_if<int>(&if_expr[i]))
								{
									i++;
									if (*lhs > *rhs)
										m_values.push_back(1);
									else
										m_values.push_back(0);
								}

							}
							else if (log_val == 1)
							{
								if (auto rhs = std::get_if<int>(&if_expr[i]))
								{
									i++;
									if (*lhs < *rhs)
										m_values.push_back(1);
									else
										m_values.push_back(0);
								}
							}
							else if (log_val == 2)
							{
								if (auto rhs = std::get_if<int>(&if_expr[i]))
								{
									i++;
									if (*lhs == *rhs)
										m_values.push_back(1);
									else
										m_values.push_back(0);
								}
							}
							else if (log_val == 3)
							{
								if (auto rhs = std::get_if<int>(&if_expr[i]))
								{
									i++;
									if (*lhs != *rhs)
										m_values.push_back(1);
									else
										m_values.push_back(0);
								}
							}
							else if (log_val == 4)
							{
								if (auto rhs = std::get_if<int>(&if_expr[i]))
								{
									i++;
									if (*lhs >= *rhs)
										m_values.push_back(1);
									else
										m_values.push_back(0);
								}
							}
							else if (log_val == 5)
							{
								if (auto rhs = std::get_if<int>(&if_expr[i]))
								{
									i++;
									if (*lhs <= *rhs)
										m_values.push_back(1);
									else
										m_values.push_back(0);
								}
							}
						}
						else
						{
							std::cerr << "Invalid expr!]\n";
							exit(EXIT_FAILURE);
						}
					}
					else
					{
						std::cerr << "Invalid expr!]\n";
						exit(EXIT_FAILURE);
					}
				}
				else if (auto open_paren = std::get_if<TokenType>(&if_expr[i]))
				{
					is_paren = true;
					i++;
					TokenType tempandOr = TokenType::integer;
					std::vector<int> temp_values;
					for (int j = i; i < if_expr.size() - 2; i++)
					{
						if (auto lhs = std::get_if<int>(&if_expr[i]))
						{
							i++;
							if (auto log = std::get_if<TokenType>(&if_expr[i]))
							{
								int log_val = -1;
								if (log_prec(*log).has_value())
								{
									log_val = log_prec(*log).value();
									i++;
									if (log_val == 0)
									{
										if (auto rhs = std::get_if<int>(&if_expr[i]))
										{
											i++;
											if (*lhs > *rhs)
												temp_values.push_back(1);
											else
												temp_values.push_back(0);
										}

									}
									else if (log_val == 1)
									{
										if (auto rhs = std::get_if<int>(&if_expr[i]))
										{
											i++;
											if (*lhs < *rhs)
												temp_values.push_back(1);
											else
												temp_values.push_back(0);
										}
									}
									else if (log_val == 2)
									{
										if (auto rhs = std::get_if<int>(&if_expr[i]))
										{
											i++;
											if (*lhs == *rhs)
												temp_values.push_back(1);
											else
												temp_values.push_back(0);
										}
									}
									else if (log_val == 3)
									{
										if (auto rhs = std::get_if<int>(&if_expr[i]))
										{
											i++;
											if (*lhs != *rhs)
												temp_values.push_back(1);
											else
												temp_values.push_back(0);
										}
									}
									else if (log_val == 4)
									{
										if (auto rhs = std::get_if<int>(&if_expr[i]))
										{
											i++;
											if (*lhs >= *rhs)
												temp_values.push_back(1);
											else
												temp_values.push_back(0);
										}
									}
									else if (log_val == 5)
									{
										if (auto rhs = std::get_if<int>(&if_expr[i]))
										{
											i++;
											if (*lhs <= *rhs)
												temp_values.push_back(1);
											else
												temp_values.push_back(0);
										}
									}
								}
								else
								{
									std::cerr << "Invalid expr!]\n";
									exit(EXIT_FAILURE);
								}
							}
							else
							{
								std::cerr << "Invalid expr!]\n";
								exit(EXIT_FAILURE);
							}
						}
						else
						{
							std::cerr << "Invalid expr!]\n";
							exit(EXIT_FAILURE);
						}

						if (temp_values.size() > 1 && tempandOr == TokenType::AND)
						{
							if (temp_values[temp_values.size() - 1] == 1 && temp_values[temp_values.size() - 2] == 1)
							{
								temp_values.erase(temp_values.end() - 2, temp_values.end());
								temp_values.push_back(1);
							}
							else
							{
								temp_values.erase(temp_values.end() - 2, temp_values.end());
								temp_values.push_back(0);
							}
						}

						if (auto open_paren = std::get_if<TokenType>(&if_expr[i]))
						{
							if (*open_paren == TokenType::close_paren)
							{
								i++;
								auto it = find(temp_values.begin(), temp_values.end(), 1);
								if (it != temp_values.end())
								{
									m_values.push_back(1);
								}
								else
								{
									m_values.push_back(0);
								}
								break;
							}
						}

						if (i < if_expr.size())
						{
							if (auto log = std::get_if<TokenType>(&if_expr[i]))
							{
								if (*log == TokenType::AND || *log == TokenType::OR)
								{
									tempandOr = *log;

								}
								else
								{
									std::cerr << "Invalid expr!]\n";
									exit(EXIT_FAILURE);
								}
							}
						}
					}
				}
				else
				{
					std::cerr << "Invalid expr!]\n";
					exit(EXIT_FAILURE);
				}

				if (m_values.size() > 1 && andOr == TokenType::AND)
				{
					if (m_values[m_values.size() - 1] == 1 && m_values[m_values.size() - 2] == 1)
					{
						m_values.erase(m_values.end() - 2, m_values.end());
						m_values.push_back(1);
					}
					else
					{
						m_values.erase(m_values.end() - 2, m_values.end());
						m_values.push_back(0);
					}
				}
				else if (m_values.size() > 1 && log_prec(andOr).has_value())
				{
					int a = log_prec(andOr).value();
					if (a == 0)
					{
						if (m_values[m_values.size() - 1] <  m_values[m_values.size() - 2])
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(1);
						}
						else
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(0);
						}
					}
					if (a == 1)
					{
						if (m_values[m_values.size() - 1] > m_values[m_values.size() - 2])
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(1);
						}
						else
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(0);
						}
					}
					if (a == 2)
					{
						if (m_values[m_values.size() - 1] == m_values[m_values.size() - 2])
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(1);
						}
						else
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(0);
						}
					}
					if (a == 3)
					{
						if (m_values[m_values.size() - 1] != m_values[m_values.size() - 2])
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(1);
						}
						else
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(0);
						}
					}
					if (a == 4)
					{
						if (m_values[m_values.size() - 1] <= m_values[m_values.size() - 2])
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(1);
						}
						else
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(0);
						}
					}
					if (a == 5)
					{
						if (m_values[m_values.size() - 1] >= m_values[m_values.size() - 2])
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(1);
						}
						else
						{
							m_values.erase(m_values.end() - 2, m_values.end());
							m_values.push_back(0);
						}
					}
				}

				if (i < if_expr.size())
				{
					if (auto log = std::get_if<TokenType>(&if_expr[i]))
					{
						if (*log == TokenType::AND || *log == TokenType::OR)
						{
							andOr = *log;
						}
						else if (is_paren && log_prec(*log).has_value())
						{
							andOr = *log;
							is_paren = false;
						}
						else
						{
							std::cerr << "Invalid expr!]\n";
							exit(EXIT_FAILURE);
						}
					}
				}
			}

			auto it = find(m_values.begin(), m_values.end(), 1);
			if (it != m_values.end())
			{
				m_values.clear();
				m_values.push_back(1);
			}
			else
			{
				m_values.clear();
				m_values.push_back(0);
			}
			if_expr.clear();
		}
		else
		{
			if (auto str = std::get_if<std::string>(&if_expr[0]))
				m_values.push_back(m_int_values.at(*str));
			else if (auto val = std::get_if<int>(&if_expr[0]))
				m_values.push_back(*val);
			else
			{
				std::cerr << "Invalid expr!" << std::endl;
				exit(EXIT_FAILURE);
			}

		}
		if_expr.clear();
	}
	void parse_bin_expr()
	{
		m_int_values.at(m_cur_var.value()) = evaluate(m_bin_expr);
		m_cur_var.reset();
		m_bin_expr.clear();
	}
private:
	NodeProg prog;

	std::string m_bin_expr;
	std::stringstream m_bss;
	std::stringstream m_data;
	std::stringstream m_output;

	std::vector<int> m_values;
	std::vector<int> m_paren_count;
	std::vector<int> m_visit_counts;
	std::vector<std::string> m_int_name;
	std::vector<std::string> m_char_name;
	std::vector<size_t> m_last_index_of_scope;
	std::vector<std::variant<int, std::string, TokenType>> if_expr;

	std::unordered_map<std::string, int> m_int_values;
	std::unordered_map<std::string, size_t> m_char_vars;
	std::unordered_map<std::string, size_t> m_int_vars;
	std::unordered_map<std::string, std::string> m_types;

	std::optional<int> value;
	std::optional<std::string> m_cur_var;
	std::optional<NodeExpr*> temp_log_expr = nullptr;

	size_t m_stack_size = 0;

	int orCount = 0;
	int current_paren = -1;
	int carry_count = 0;

	bool if_stat = false;
	bool temp_vars = false;
	bool count_paren = false;
	bool is_bin_expr = false;
};