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
	Generator(NodeProg prog, std::unordered_map<std::string, std::string> m_types)
		: prog(prog),
		m_types(m_types)
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
					if (gen.log_stat)
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
					if (gen.log_stat)
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

					if (gen.is_bin_expr && gen.m_bin_expr.size() > 2)
					{
						if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
							gen.m_bin_expr.push_back(')');

						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::greater);
				
				gen.gen_expr(log_greater->rhs);
				if (gen.is_bin_expr && gen.m_bin_expr.size() > 2)
				{
					if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
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
					if (gen.log_stat)
					{
						gen.if_expr.push_back(TokenType::AND);
					}

					gen.gen_expr(log_greater->varAnd.value()->lhs);
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
					if (gen.log_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_greater->varOr.value()->lhs;
					gen.orCount++;

						gen.is_bin_expr = false;
						while (gen.temp_log_expr != nullptr)
						{
							gen.gen_expr(gen.temp_log_expr.value());
							gen.is_bin_expr = false;
						}
				}
			}
			void operator()(const NodeLogExprLesser* log_lesser)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_lesser->lhs);
					if (gen.is_bin_expr && gen.m_bin_expr.size() > 2)
					{
						if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
							gen.m_bin_expr.push_back(')');
						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::lesser);

				gen.gen_expr(log_lesser->rhs);
				if (gen.is_bin_expr && gen.m_bin_expr.size() > 2)
				{
					if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
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
					if (gen.log_stat)
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
					if (gen.log_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_lesser->varOr.value()->lhs;
					gen.orCount++;
						gen.is_bin_expr = false;
						while (gen.temp_log_expr != nullptr)
						{
							gen.gen_expr(gen.temp_log_expr.value());
							gen.is_bin_expr = false;
						}
				}
			}
			void operator()(const NodeLogExprEqual* log_equal)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_equal->lhs);
					if (gen.is_bin_expr && gen.m_bin_expr.size() > 2)
					{
						if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
							gen.m_bin_expr.push_back(')');
						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::equal);
				gen.gen_expr(log_equal->rhs);
				if (gen.is_bin_expr && gen.m_bin_expr.size() > 2)
				{
					if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
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
					if (gen.log_stat)
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
					if (gen.log_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_equal->varOr.value()->lhs;
					gen.orCount++;
						gen.is_bin_expr = false;
						while (gen.temp_log_expr != nullptr)
						{
							gen.gen_expr(gen.temp_log_expr.value());
							gen.is_bin_expr = false;
						}
				}
			}
			void operator()(const NodeLogExprNotEqual* log_not_equal)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_not_equal->lhs);
					if (gen.is_bin_expr && gen.m_bin_expr.size() > 2)
					{
						if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
							gen.m_bin_expr.push_back(')');
						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::notequal);
				gen.gen_expr(log_not_equal->rhs);
				if (gen.is_bin_expr && gen.m_bin_expr.size() > 2){
					if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
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
					if (gen.log_stat)
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
					if (gen.log_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_not_equal->varOr.value()->lhs;
					gen.orCount++;
						gen.is_bin_expr = false;
						while (gen.temp_log_expr != nullptr)
						{
							gen.gen_expr(gen.temp_log_expr.value());
							gen.is_bin_expr = false;
						}
				}

			}
			void operator()(const NodeLogExprGreaterEqual* log_greater_equal)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_greater_equal->lhs);
					if (gen.is_bin_expr && gen.m_bin_expr.size() > 2)
					{
						if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
							gen.m_bin_expr.push_back(')');
						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::greaterequal);

				gen.gen_expr(log_greater_equal->rhs);
				if (gen.is_bin_expr && gen.m_bin_expr.size() > 2)
				{
					if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
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
					if (gen.log_stat)
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
					if (gen.log_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_greater_equal->varOr.value()->lhs;
					gen.orCount++;
						gen.is_bin_expr = false;
						while (gen.temp_log_expr != nullptr)
						{
							gen.gen_expr(gen.temp_log_expr.value());
							gen.is_bin_expr = false;
						}

				}
			}
			void operator()(const NodeLogExprLesserEqual* log_lesser_equal)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_lesser_equal->lhs);
					if (gen.is_bin_expr && gen.m_bin_expr.size() > 2)
					{
						if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
							gen.m_bin_expr.push_back(')');
						gen.if_expr.push_back(gen.evaluate(gen.m_bin_expr));
						gen.m_bin_expr.clear();
						gen.is_bin_expr = false;
					}
					gen.if_expr.push_back(TokenType::lesserequal);
				
				gen.gen_expr(log_lesser_equal->rhs);
				if (gen.is_bin_expr && gen.m_bin_expr.size() > 2)
				{
					if (count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), '(') - 1 == count(gen.m_bin_expr.begin(), gen.m_bin_expr.end(), ')'))
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
					if (gen.log_stat)
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
					if (gen.log_stat)
					{
						gen.if_expr.push_back(TokenType::OR);
					}
					gen.temp_log_expr = log_lesser_equal->varOr.value()->lhs;
					gen.orCount++;
						gen.is_bin_expr = false;
						while (gen.temp_log_expr != nullptr)
						{
							gen.gen_expr(gen.temp_log_expr.value());
							gen.is_bin_expr = false;
						}
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
				if (gen.log_stat && !gen.is_bin_expr)
				{
					gen.if_expr.push_back(stoi(int_val->value.value()));
				}
				gen.value = int_val->value.value();
				gen.m_output << "   mov rax," << int_val->value.value() << '\n';
				gen.push("rax");

				if (gen.m_cur_var.has_value() && gen.m_arr_vars.contains(gen.m_cur_var.value()) && (gen.m_arr_vars.at(gen.m_cur_var.value()).type == "integer" || gen.m_arr_vars.at(gen.m_cur_var.value()).type == "boolean"))
				{
					gen.m_arr_vars.at(gen.m_cur_var.value()).arr[gen.m_arr_vars.at(gen.m_cur_var.value()).current_size] = gen.m_stack_size - 1;
				}
			}
			void operator()(const NodeTermStringVal* string_val)
			{
				if (gen.log_stat && !gen.is_bin_expr)
				{
					int value = stoi(string_val->value);
					gen.if_expr.push_back(value);
					gen.m_output << "   mov rax," << value << "" << '\n';
					gen.push("rax");
				}
				else
				gen.value = string_val->value;
			}
			void operator()(const NodeWordCharVal* char_val)
			{
				if (!gen.log_stat)
				{
					gen.value = char_val->value.value();
					gen.m_output << "   mov rax,'" << char_val->value.value() << "'" << '\n';
					gen.push("rax");
				}
				else
				{
					int value = int(char_val->value.value()[0]) - 48;
					gen.if_expr.push_back(value);
					gen.m_output << "   mov rax," <<value << "" << '\n';
					gen.push("rax");
				}

				if (gen.m_cur_var.has_value() && gen.m_arr_vars.contains(gen.m_cur_var.value()) && gen.m_arr_vars.at(gen.m_cur_var.value()).type == "character")
				{
					gen.m_arr_vars.at(gen.m_cur_var.value()).arr[gen.m_arr_vars.at(gen.m_cur_var.value()).current_size] = gen.m_stack_size-1;
				}
			}
			void operator()(const NodeTermOpposet* term_op)
			{
				if (auto int_val = std::get_if<NodeTermIntVal*>(&term_op->var))
				{
					if (stoi((*int_val)->value.value()) != 0)
					{
						gen.m_output << "   mov rax,0" << '\n';
						if (gen.log_stat && !gen.is_bin_expr)
						{
							gen.if_expr.push_back(0);
						}
					}
					else
					{
						gen.m_output << "   mov rax,1" << '\n';
						if (gen.log_stat && !gen.is_bin_expr)
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
						if (gen.log_stat && !gen.is_bin_expr)
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
						gen.value = std::to_string(gen.m_int_values.at((*var)->name));
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
					if (gen.log_stat && !gen.is_bin_expr && gen.m_int_values.contains(term_var->name))
					{
						gen.if_expr.push_back(gen.m_int_values.at(term_var->name));
					}
					gen.value = std::to_string(gen.m_int_values.at(term_var->name));
					const auto& var = gen.m_int_vars.at(term_var->name);
					std::stringstream offset;
					offset << "QWORD [rsp + " << (gen.m_stack_size - var - 1) * 8 << "]\n";
					gen.push(offset.str());
					if (gen.m_cur_var.has_value() && gen.m_arr_vars.contains(gen.m_cur_var.value()) && (gen.m_arr_vars.at(gen.m_cur_var.value()).type == "integer" || gen.m_arr_vars.at(gen.m_cur_var.value()).type == "boolean"))
					{
						gen.m_arr_vars.at(gen.m_cur_var.value()).arr[gen.m_arr_vars.at(gen.m_cur_var.value()).current_size] = gen.m_stack_size-1;
					}
				}
				else if (gen.m_char_vars.contains(term_var->name))
				{
					if (gen.log_stat && !gen.is_bin_expr)
					{
						int value = int(gen.m_char_values.at(gen.m_cur_var.value())) - 48;
						gen.if_expr.push_back(value);
						gen.m_output << "   mov rax," << value << '\n';
						gen.push("rax");
					}
					else
					{
						gen.value.value()[0] = gen.m_char_values.at(gen.m_cur_var.value());
					    const auto& var = gen.m_char_vars.at(term_var->name);
					    std::stringstream offset;
					    offset << "QWORD [rsp + " << (gen.m_stack_size - var - 1) * 8 << "]\n";
					    gen.push(offset.str());
					}
					if (gen.m_cur_var.has_value() && gen.m_arr_vars.contains(gen.m_cur_var.value()) && gen.m_arr_vars.at(gen.m_cur_var.value()).type == "character")
					{
						gen.m_arr_vars.at(gen.m_cur_var.value()).arr[gen.m_arr_vars.at(gen.m_cur_var.value()).current_size] = gen.m_stack_size-1;
					}
				}
				else if (gen.m_string_vars.contains(term_var->name))
				{
					
					std::string text;
					for (int i = 0; i < gen.m_string_vars.at(term_var->name).values.size(); i++)
					{
						text += gen.m_string_vars.at(term_var->name).values[i];
					}
					if (gen.log_stat && !gen.is_bin_expr)
					{
						int value = 0;
						for (int i = 0; i < text.size(); i++)
						{
							value += int(text[i]) - 48;
						}
						gen.if_expr.push_back(value);
						gen.m_output << "   mov rax," << value << "" << '\n';
						gen.push("rax");
					}
					else
					{
						gen.m_output << "mov rax,'" << text << "'" << std::endl;
						gen.value = text;
					}
				}
				else if (gen.m_arr_vars.contains(term_var->name))
				{
					if (gen.m_arr_vars.at(term_var->name).current_size < 0 || gen.m_arr_vars.at(term_var->name).current_size >= gen.m_arr_vars.at(term_var->name).size)
					{
						std::cerr << "Out of range of array!!!" << std::endl;
						exit(EXIT_FAILURE);
					}
					if (gen.log_stat && !gen.is_bin_expr)
					{
						gen.if_expr.push_back(gen.m_arr_vars.at(term_var->name).values[gen.m_arr_vars.at(term_var->name).current_size]);
					}
					if (gen.is_bin_expr)
					{
						gen.value = std::to_string(gen.m_arr_vars.at(term_var->name).values[gen.m_arr_vars.at(term_var->name).current_size]);
					}
					const auto& var = gen.m_arr_vars.at(term_var->name).arr[gen.m_arr_vars.at(term_var->name).current_size];
					std::stringstream offset;
					offset << "QWORD [rsp + " << (gen.m_stack_size - var - 1) * 8 << "]\n";
					gen.push(offset.str());
				}
				else
				{
					gen.value = term_var->value.value();
					gen.m_output << "   mov rax, " << term_var->value.value() << '\n';
					gen.push("rax");

				}
			}
			void operator()(NodeTermParen* term_paren)
			{
				bool paren = false;
				if (gen.log_stat && !gen.is_bin_expr)
				{
					gen.if_expr.push_back(TokenType::open_paren);
					paren = true;
				}

				gen.m_bin_expr += "(";
				gen.gen_expr(term_paren->expr);
				if (gen.value.has_value() && gen.is_bin_expr)
				{
					gen.m_bin_expr += gen.value.value();
					gen.value.reset();
				}

				if (gen.log_stat && !gen.is_bin_expr && paren)
					gen.if_expr.push_back(TokenType::close_paren);

				gen.m_bin_expr += ")";
			}
			void operator()(NodeLogExpr* log_expr)
			{
				gen.gen_log_expr(log_expr);
			}
			void operator()(NodeTermArray* array)
			{
				int size = -1;
				if (gen.m_int_vars.contains(array->size))
				{
					size = gen.m_int_values.at(array->size);
				}
				else
				{
					size = stoi(array->size);
				}
				if (gen.m_arr_vars.contains(array->name))
				{
					if (gen.m_arr_vars.at(array->name).size <= size || size < 0)
					{
						std::cerr << "Out of range of array!!!" << std::endl;
						exit(EXIT_FAILURE);
					}
				}

				if (!gen.m_arr_vars.contains(array->name) && !gen.is_bin_expr)
				{
					std::vector<size_t> arr;
					std::vector<int> values;
					for (int i = 0; i < stoi(array->size); i++)
					{
						values.push_back(0);
						arr.push_back(gen.m_stack_size);
						gen.m_output << "   mov rax,0\n";
						gen.push("rax");
					}
					gen.m_arr_vars[array->name].size = stoi(array->size);
					gen.m_arr_vars[array->name].arr = arr;
					gen.m_arr_vars[array->name].values = values;
					gen.m_arr_vars[array->name].type = array->type;
					if (gen.temp_vars)
					{
						gen.m_temp_vars.push_back(array->name);
					}
				}
				else if(array->expr != nullptr)
				{
					if (gen.m_types.at(array->name) == array->equalType)
					{			
						gen.m_cur_var.reset();
						gen.gen_expr(array->expr);
						gen.m_arr_vars.at(array->name).arr[size] = gen.m_stack_size - 1;
						if (gen.m_bin_expr.size() > 0)
						{
							gen.m_arr_vars.at(array->name).values[size] = gen.evaluate(gen.m_bin_expr);
							gen.m_bin_expr.clear();
						}
						else if (gen.if_expr.size() > 0)
						{
							gen.parse_log_expr();
							gen.m_arr_vars.at(array->name).values[size] = gen.parse_log_expr();
							gen.m_bin_expr.clear();
						}
						else if (gen.value.has_value())
						{
							gen.m_arr_vars.at(array->name).values[size] = stoi(gen.value.value());
							gen.value.reset();
						}
					}
					else
					{
						std::cerr << "Invalid equalization!!!" << std::endl;
						exit(EXIT_FAILURE);
					}
				}
				else if((size > -1 && size < gen.m_arr_vars.at(array->name).size) || gen.is_bin_expr || gen.log_stat)
				{
					if (gen.log_stat && !gen.is_bin_expr)
					{
						gen.if_expr.push_back(gen.m_arr_vars.at(array->name).values[size]);
					}
					gen.value = std::to_string(gen.m_arr_vars.at(array->name).values[size]);
					const auto& var = gen.m_arr_vars.at(array->name).arr[size];
					std::stringstream offset;
					offset << "QWORD [rsp + " << (gen.m_stack_size - var - 1) * 8 << "]\n";
					gen.push(offset.str());
				}
			}
			void operator()(NodeTermString* str)
			{
				int size = -1;
				if (str->size.has_value()&&gen.m_int_vars.contains(str->size.value()))
				{
					size = gen.m_int_values.at(str->size.value());
				}
				else if(str->size.has_value())
				{
					size = stoi(str->size.value());
				}
				if (str->name.size() > 0 && !gen.m_string_vars.contains(str->name))
				{
					std::vector<char> values;
					gen.gen_expr(str->expr);
					for (int i = 0; i < gen.value.value().size(); i++)
					{
						values.push_back(gen.value.value()[i]);
					}
					gen.m_string_vars[str->name].size = gen.value.value().size();
					gen.m_string_vars[str->name].values = values;
					std::string text;
					for (int i = 0; i < gen.m_string_vars.at(str->name).size; i++)
					{
						text += gen.m_string_vars.at(str->name).values[i];
					}
					gen.m_output << "    mov rax,'" << text << "'" << std::endl;
					gen.push("rax");
					if (gen.temp_vars)
						gen.m_temp_vars.push_back(str->name);
				}
				else if (gen.m_string_vars.contains(str->name) && str->expr != nullptr && size == -1)
				{
					std::vector<char> values;
					gen.gen_expr(str->expr);
					for (int i = 0; i < gen.value.value().size(); i++)
					{
						values.push_back(gen.value.value()[i]);
					}
					gen.m_string_vars.at(str->name).size = gen.value.value().size();
					gen.m_string_vars.at(str->name).values = values;
				}
				else if (gen.m_string_vars.contains(str->name) && str->expr != nullptr && size != -1)
				{
					if (size >= gen.m_string_vars.at(str->name).values.size() || size < -1)
					{
						std::cerr << "Out of range!" << std::endl;
						exit(EXIT_FAILURE);
					}
					gen.gen_expr(str->expr);
					if (gen.value.value().size() == 1)
					{
						gen.m_string_vars.at(str->name).values[size] = gen.value.value()[0];
						gen.value.reset();
					}
					else
					{
						std::cerr << "cant equal string with character!" << std::endl;
						exit(EXIT_FAILURE);
					}
				}
				else if (str->expr == nullptr)
				{
					if (size >= gen.m_string_vars.at(str->name).values.size() || size < -1)
					{
						std::cerr << "Out of range!" << std::endl;
						exit(EXIT_FAILURE);
					}
					std::string text;
					for (int i = 0; i < gen.m_string_vars.at(str->name).size; i++)
					{
							text += gen.m_string_vars.at(str->name).values[i];
					}
					if (gen.log_stat && !gen.is_bin_expr)
					{
						gen.value = std::to_string(int(text[size]) - 48);
						gen.m_output << "   mov rax," << gen.value.value()<< '\n';
						gen.push("rax");
					}
					else
					{
						gen.m_output << "mov rax,'" << text << "'" << std::endl;
						gen.value = text;
					}
				}
				else if (str->expr != nullptr && size == -1 && !gen.m_string_vars.contains(str->name))
				{
					std::string text;
					if (auto s = std::get_if<NodeTerm*>(&str->expr->var))
					{
						if (auto text1 = std::get_if<NodeTermStringVal*>(&((*s)->var)))
						{
							text = (*text1)->value;
						}
					}

					if (gen.log_stat && !gen.is_bin_expr)
					{
						int value = 0;
						for (int i = 0; i < text.size(); i++)
						{
							value += int(text[i]) - 48;
						}
						gen.if_expr.push_back(value);
						gen.m_output << "   mov rax," << value << "" << '\n';
						gen.push("rax");
					}
					else
					{
						gen.value = text;
					}
				}
				else
				{
					std::cerr << "Wtf expression!" << std::endl;
					exit(EXIT_FAILURE);
				}
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
							gen.m_bin_expr += gen.value.value();
							break;
						}
						else if (gen.m_bin_expr[i] == ')')
							break;
					}
				}
				else
					gen.m_bin_expr += gen.value.value();

				gen.m_bin_expr += "-";
				gen.value.reset();
				gen.gen_expr(sub->rhs);
				if (auto exp_lhs = std::get_if<NodeBinExpr*>(&sub->rhs->var))
				{

				}
				else
				{
					if (gen.value.has_value())
						gen.m_bin_expr += gen.value.value();
					gen.value.reset();
				}


				gen.pop("rbx");
				gen.pop("rax");
				gen.m_output << "    sub rax, rbx\n";
				gen.push("rax");
			}
			void operator()(const NodeBinExprPercent* percent)
			{
				std::string niga;
				gen.gen_expr(percent->lhs);
				if (gen.m_bin_expr.size() > 0)
				{
					for (int i = gen.m_bin_expr.size() - 1; i >= 0; i++)
					{
						if (gen.m_bin_expr[i] != ' ' && gen.m_bin_expr[i] != ')')
						{
							gen.m_bin_expr += gen.value.value();
							niga += gen.value.value();
							break;
						}
						else if (gen.m_bin_expr[i] == ')')
							break;
					}
				}
				else
				{
					niga += gen.value.value();
					gen.m_bin_expr += gen.value.value();
				}

				gen.m_bin_expr += "%";
				gen.value.reset();
				gen.gen_expr(percent->rhs);
				if (auto exp_lhs = std::get_if<NodeBinExpr*>(&percent->rhs->var))
				{
					
				}
				else
				{
					if (gen.value.has_value())
						gen.m_bin_expr += gen.value.value();
					gen.value.reset();
				}

				for (int i = 0; i < gen.m_bin_expr.size(); i++)
				{
					if (gen.m_bin_expr[i] == '%')
					{
						niga += gen.m_bin_expr[i];
						for (int j = i + 1; j < gen.m_bin_expr.size(); j++)
						{
							if (gen.m_bin_expr[j] != ' ' && !isdigit(gen.m_bin_expr[j]))
								break;
							niga += gen.m_bin_expr[j];
						}
						break;
				    }
				}


				int val = gen.evaluate(niga);
				if (val == 0)
				{
					gen.m_output << "    mov rax,0\n";
				}
				else
				{
					gen.m_output << "    mov rax,1\n";
				}
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
							gen.m_bin_expr += gen.value.value();
							break;
						}
						else if (gen.m_bin_expr[i] == ')')
							break;
					}
				}
				else
					gen.m_bin_expr += gen.value.value();
				gen.m_bin_expr += "+";
				gen.value.reset();
				gen.gen_expr(add->rhs);
				if (auto exp_lhs = std::get_if<NodeBinExpr*>(&add->rhs->var))
				{

				}
				else
				{
					if (gen.value.has_value())
						gen.m_bin_expr += gen.value.value();
					gen.value.reset();
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
							gen.m_bin_expr += gen.value.value();
							break;
						}
						else if (gen.m_bin_expr[i] == ')')
							break;
					}
				}
				else
					gen.m_bin_expr += gen.value.value();
				gen.m_bin_expr += "*";
				gen.value.reset();
				gen.gen_expr(multi->rhs);
				if (auto exp_lhs = std::get_if<NodeBinExpr*>(&multi->rhs->var))
				{

				}
				else
				{
					if (gen.value.has_value())
						gen.m_bin_expr += gen.value.value();
					gen.value.reset();
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
							gen.m_bin_expr += gen.value.value();
							break;
						}
						else if (gen.m_bin_expr[i] == ')')
							break;
					}
				}
				else
					gen.m_bin_expr += gen.value.value();
				gen.m_bin_expr += "/";
				gen.value.reset();
				gen.gen_expr(div->rhs);
				if (auto exp_lhs = std::get_if<NodeBinExpr*>(&div->rhs->var))
				{

				}
				else
				{
					if (gen.value.has_value())
						gen.m_bin_expr += gen.value.value();
					gen.value.reset();
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
		static std::vector<size_t> m_last_index_of_scope;
		begin_scope(m_last_index_of_scope);
		for (NodeStat* stmt : scope->stmts) {
			gen_stat(stmt);
		}
		end_scope(m_last_index_of_scope);
	}
	void gen_if_pred(const NodeIfPred* pred, const std::string& end_label)
	{
		struct PredVisitor {
			Generator& gen;
			const std::string& end_label;

			void operator()(const NodeIfPredElif* elif) const
			{
				gen.log_stat = true;
				gen.gen_expr(elif->expr);
				gen.is_bin_expr = false;
				gen.log_stat = false;
				gen.log_expr_or();
				gen.pop("rax");
				const std::string label = gen.create_label();
				gen.m_output << "    cmp rax,0\n";
				gen.m_output << "    je " << label << "\n";

				gen.m_output << " \n";
				gen.m_bin_expr.clear();
				if (gen.parse_log_expr() == 1)
				{
					gen.m_output << label << ":\n";
					gen.gen_scope(elif->scope);
					gen.m_bin_expr.clear();
				}
				else if (elif->pred.has_value()) {
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
			void operator()(const NodeStatWhile* stat_while)
			{
				while (true)
				{
					gen.log_stat = true;
					gen.gen_expr(stat_while->expr);
					gen.is_bin_expr = false;
					gen.log_stat = false;

					gen.log_expr_or();

					gen.pop("rax");
			     	const std::string label = gen.create_label();
					gen.m_output << "    cmp rax,0\n";
					gen.m_output << "    je " << label << "\n";
					gen.m_output << " \n";

					gen.m_bin_expr.clear();
					if (gen.parse_log_expr() == 1)
					{
						gen.gen_scope(stat_while->scope);
						gen.m_output << "  \n";
						gen.m_bin_expr.clear();
						gen.m_output << label << ":\n";
					}
					else
					{
						gen.m_output << label << ":\n";
						break;
					}
				}
				gen.m_output << "  \n";
				gen.m_bin_expr.clear();
			}
			void operator()(const NodeStatRemoveString* remove_string)
			{
				if (gen.m_string_vars.contains(remove_string->name) )
				{
					int size = -1;
					if (gen.m_int_vars.contains(remove_string->pos))
					{
						size = gen.m_int_values.at(remove_string->pos);
					}
					else
					{
						size = stoi(remove_string->pos);
					}
					if (size < 0 || size > gen.m_string_vars.at(remove_string->name).size)
					{
						std::cerr << "Out of range!" << std::endl;
						exit(EXIT_FAILURE);
					}

					gen.m_string_vars.at(remove_string->name).values.erase(gen.m_string_vars.at(remove_string->name).values.begin() + size);
					gen.m_string_vars.at(remove_string->name).size--;
				}
			}
			void operator()(const NodeStatPrint* stat_print)
			{
                if (stat_print->type == "character" || (stat_print->variableName.has_value() && gen.m_char_vars.contains(stat_print->variableName.value())))
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
				else if (stat_print->type == "integer"  || (stat_print->variableName.has_value() && gen.m_int_vars.contains(stat_print->variableName.value())))
				{
					gen.gen_expr(stat_print->expr);
					gen.is_bin_expr = false;
					gen.pop("rax");
					gen.m_output << "    call _printnumberRAX\n";
				}
				else if (stat_print->type == "boolean" && (stat_print->variableName.has_value() && gen.m_int_vars.contains(stat_print->variableName.value())))
				{
					if (gen.m_int_values.at(stat_print->variableName.value()) == 0)
					{
						gen.m_output << "    mov [temp],dword 'false'\n";
						gen.m_output << "    mov rax,1" << "\n";
						gen.m_output << "    mov rsi,temp" << "\n";
						gen.m_output << "    mov rdi,1" << "\n";
						gen.m_output << "    mov rdx,1" << "\n";
						gen.m_output << "    syscall\n";
						gen.m_output << "    \n";
					}
					else
					{
						gen.m_output << "    mov [temp],dword 'true'\n";
						gen.m_output << "    mov rax,1" << "\n";
						gen.m_output << "    mov rsi,temp" << "\n";
						gen.m_output << "    mov rdi,1" << "\n";
						gen.m_output << "    mov rdx,1" << "\n";
						gen.m_output << "    syscall\n";
						gen.m_output << "    \n";
					}
				}
				else if (stat_print->type == "variable")
				{
					if (gen.m_types.at(stat_print->variableName.value()) == "integer")
					{
						gen.gen_expr(stat_print->expr);
						gen.is_bin_expr = false;
						gen.pop("rax");
						gen.m_output << "    call _printnumberRAX\n";
					}
					else if (gen.m_types.at(stat_print->variableName.value()) == "character")
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
					else if (gen.m_types.at(stat_print->variableName.value()) == "boolean")
					{
						gen.gen_expr(stat_print->expr);
						gen.is_bin_expr = false;
						gen.pop("rax");
						gen.m_output << "    call _printnumberRAX\n";
					}
					else if (gen.m_types.at(stat_print->variableName.value()) == "string" && gen.m_string_vars.contains(stat_print->variableName.value()))
					{
						std::string text;
						for (int i = 0; i < gen.m_string_vars.at(stat_print->variableName.value()).size; i++)
						{
							text += gen.m_string_vars.at(stat_print->variableName.value()).values[i];
						}
						
						for (int i = 0; i < text.size(); i++)
						{
						gen.m_output << "    mov [temp + " <<i<<"], " << "byte '" << text[i] << "'" << "\n";
						}
						gen.m_output << "    mov rax,1" << "\n";
						gen.m_output << "    mov rsi,temp" << "\n";
						gen.m_output << "    mov rdi,1" << "\n";
						gen.m_output << "    mov rdx," << text.size()<< "\n";
						gen.m_output << "    syscall\n";
						gen.m_output << "    \n";
					}
				}
				else if (stat_print->type == "string")
				{
					gen.gen_expr(stat_print->expr);

					for (int i = 0; i < gen.value.value().size(); i++)
					{
						gen.m_output << "    mov [temp + " << i << "], " << "byte '" << gen.value.value()[i] << "'" << "\n";
					}
					gen.m_output << "    mov rax,1" << "\n";
					gen.m_output << "    mov rsi,temp" << "\n";
					gen.m_output << "    mov rdi,1" << "\n";
					gen.m_output << "    mov rdx," << gen.value.value().size() << "\n";
					gen.m_output << "    syscall\n";
					gen.m_output << "    \n";
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
				if (!gen.m_int_vars.contains(stat_var->name) && !gen.m_char_vars.contains(stat_var->name) && !gen.m_arr_vars.contains(stat_var->name))
				{
					gen.m_cur_var = stat_var->name;
					gen.gen_expr(stat_var->expr);
					gen.is_bin_expr = false;
					if (!gen.m_bin_expr.empty())
					{
						gen.m_int_values[stat_var->name] = stoi(gen.value.value());
						gen.parse_bin_expr();
					}
					else if (stat_var->type != "character" && stat_var->type != "string" && !gen.m_arr_vars.contains(stat_var->name) && stat_var->type != "string")
						gen.m_int_values[stat_var->name] = stoi(gen.value.value());
					

					if (stat_var->type != "character" && stat_var->type != "string"  &&!gen.m_arr_vars.contains(stat_var->name))
					{
						gen.m_int_vars[stat_var->name] = gen.m_stack_size - 1;
						if (gen.temp_vars)
						{
							gen.m_temp_vars.push_back(stat_var->name);
						}
					}
					else if (stat_var->type == "character" && stat_var->type != "string" &&!gen.m_arr_vars.contains(stat_var->name))
					{
						gen.m_char_values[stat_var->name] = gen.value.value()[0];
						gen.value.reset();
						gen.m_char_vars[stat_var->name] = gen.m_stack_size - 1;
						if (gen.temp_vars)
						{
							gen.m_temp_vars.push_back(stat_var->name);
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
				if (gen.m_int_vars.contains(stat_eq->variableName) || gen.m_char_vars.contains(stat_eq->variableName) || gen.m_arr_vars.contains(stat_eq->variableName) || gen.m_string_vars.contains(stat_eq->variableName))
				{
					gen.m_cur_var = stat_eq->variableName;
					gen.gen_expr(stat_eq->expr);
					gen.is_bin_expr = false;

	
					if (!gen.m_bin_expr.empty() && gen.m_int_values.contains(stat_eq->variableName))
						gen.parse_bin_expr();
					else if (gen.m_string_vars.contains(stat_eq->variableName) && !gen.m_bin_expr.empty())
					{
						gen.string_add();
					}
					else if(gen.m_int_values.contains(stat_eq->variableName))
						gen.m_int_values.at(stat_eq->variableName) = stoi(gen.value.value());

					if (gen.m_int_vars.contains(stat_eq->variableName))
					{
						gen.m_int_vars.at(stat_eq->variableName) = gen.m_stack_size - 1;
						gen.value.reset();
					}
					else if (gen.m_char_vars.contains(stat_eq->variableName))
					{
						gen.m_char_values.at(stat_eq->variableName) = gen.value.value()[0];
						gen.value.reset();
						gen.m_char_vars.at(stat_eq->variableName) = gen.m_stack_size - 1;
					}
					else if (gen.m_arr_vars.contains(stat_eq->variableName) && !gen.m_bin_expr.empty())
					{
						gen.m_arr_vars.at(stat_eq->variableName).arr[gen.m_arr_vars.at(stat_eq->variableName).current_size] = gen.m_stack_size - 1;
						gen.m_arr_vars.at(stat_eq->variableName).values[gen.m_arr_vars.at(stat_eq->variableName).current_size] = gen.evaluate(gen.m_bin_expr);
						gen.m_bin_expr.clear();
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
				gen.log_stat = true;
				gen.gen_expr(stmt_if->expr);
				gen.is_bin_expr = false;
				gen.log_stat = false;

				gen.log_expr_or();

				gen.pop("rax");
				const std::string label = gen.create_label();
				gen.m_output << "    cmp rax,0\n";
				gen.m_output << "    je " << label << "\n";
				gen.m_output << " \n";

				gen.m_bin_expr.clear();
				if (gen.parse_log_expr() == 1)
				{
					gen.gen_scope(stmt_if->scope);
					gen.m_output << label << ":\n";
				}
				else if (stmt_if->pred.has_value()) {
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
				gen.m_bin_expr.clear();
			}
			void operator()(const NodeStatIncerement* inc)
			{
				if (gen.m_int_vars.contains(inc->variableName))
				{
				    gen.gen_expr(inc->expr);
				    gen.pop("rax");
				    gen.m_output << "   inc rax\n";
				    gen.push("rax");
					gen.m_int_vars.at(inc->variableName) = gen.m_stack_size - 1;
					int value = gen.m_int_values.at(inc->variableName);
					value++;
					gen.m_int_values.at(inc->variableName) = value;
					gen.value.reset();
				}
				else if (gen.m_arr_vars.contains(inc->variableName))
				{
					gen.m_arr_vars.at(inc->variableName).current_size++;
					if (gen.m_arr_vars.at(inc->variableName).current_size >= gen.m_arr_vars.at(inc->variableName).size)
					{
						std::cerr << "Out of range of arrat!" << std::endl;
						exit(EXIT_FAILURE);
					}
				}
				else
				{
					std::cerr << "Cant findd variable!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			void operator()(const NodeStatDecrement* dec)
			{
				if (gen.m_int_vars.contains(dec->variableName))
				{
				gen.gen_expr(dec->expr);
				gen.pop("rax");
				gen.m_output << "   dec rax\n";
				gen.push("rax");

					gen.m_int_vars.at(dec->variableName) = gen.m_stack_size - 1;
					int value = gen.m_int_values.at(dec->variableName);
					value--;
					gen.m_int_values.at(dec->variableName) = value;
					gen.value.reset();
				}
				else if (gen.m_arr_vars.contains(dec->variableName))
				{
					gen.m_arr_vars.at(dec->variableName).current_size--;
					if (gen.m_arr_vars.at(dec->variableName).current_size < 0)
					{
						std::cerr << "Out of range of arrat!" << std::endl;
						exit(EXIT_FAILURE);
					}
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
					std::cout << "Press value that you going to press during compelation value name = [" << stat_input->name << "]" << std::endl;
					gen.m_int_values.at(stat_input->name) = _getch() - 48;
					std::cout << "Value:" << gen.m_int_values.at(stat_input->name) << std::endl;
						
					gen.m_output << "    mov rax," << gen.m_int_values.at(stat_input->name)<<"\n";
					gen.push("rax");
					gen.m_int_vars.at(stat_input->name) = gen.m_stack_size - 1;
				}
				else if (gen.m_char_vars.contains(stat_input->name))
				{
					std::cout << "Press value that you going to press during compelation value name = [" << stat_input->name << "]" << std::endl;
					char value = _getch();
					std::cout << "Value:" << value << std::endl;
					gen.m_char_vars.at(stat_input->name) = gen.m_stack_size;
					gen.m_output << "    mov rax,'"<<value << "'" << "\n";
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
		m_bss << "section .bss\n stringBuffer resb 100\nstringBufferPos resb 8\n";
		m_data << "section .data\n";
		m_data << "text       times 255 db 0\n";
		m_data << "textSize   equ $ - text\n";
		m_data << "newLineMsg dw   0xA, 0xD\n";
		m_data << "newLineLen equ $ - newLineMsg\n";
		m_data << "temp dq  '',0xA,0xD\n";


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
		if (op == '%')
			return 3;
		return 0;
	}
	int applyOp(int a, int b, char op) {
		switch (op) {
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '/': return a / b;
		case '%': return a % b;
		}
	}
	int evaluate(std::string tokens) {
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
	void begin_scope(std::vector<size_t>& m_last_index_of_scope)
	{
		m_last_index_of_scope.push_back(m_stack_size);
		temp_vars = true;
	}
	void end_scope(std::vector<size_t>& m_last_index_of_scope)
	{
		if (!m_temp_vars.empty())
		{
			for (int i = m_last_index_of_scope[m_last_index_of_scope.size() - 1]; i < m_stack_size;)
			{
				if (m_int_vars.contains(m_temp_vars[m_temp_vars.size() - 1]))
				{
					m_int_vars.erase(m_temp_vars[m_temp_vars.size() - 1]);
					pop("rax");

				}
				else if (m_char_vars.contains(m_temp_vars[m_temp_vars.size() - 1]))
				{
					m_char_vars.erase(m_temp_vars[m_temp_vars.size() - 1]);
					m_char_values.erase(m_temp_vars[m_temp_vars.size() - 1]);
					pop("rax");
				}
				else if (m_arr_vars.contains(m_temp_vars[m_temp_vars.size() - 1]))
				{
					for (int i = m_arr_vars.at(m_temp_vars[m_temp_vars.size() - 1]).size - 1; i >= 0; i--)
					{
						pop("rax");
					}
					m_arr_vars.erase(m_temp_vars[m_temp_vars.size() - 1]);
				}
				else if (m_string_vars.contains(m_temp_vars[m_temp_vars.size() - 1]))
				{
					m_string_vars.erase(m_temp_vars[m_temp_vars.size() - 1]);
					pop("rax");
				}
				m_temp_vars.erase(m_temp_vars.end() - 1);
			}
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
	int parse_log_expr()
	{
		std::vector<int> m_values;
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
		return m_values[0];
	}
	void parse_bin_expr()
	{
		m_int_values.at(m_cur_var.value()) = evaluate(m_bin_expr);
		m_cur_var.reset();
		m_bin_expr.clear();
	}
	void string_add()
	{
		std::vector<char> text;
		for (int i = 0; i < m_bin_expr.size(); i++)
		{
			if (m_bin_expr[i] != '+')
			{
				text.push_back(m_bin_expr[i]);
			}
		}

		m_string_vars.at(m_cur_var.value()).size = text.size();
		m_string_vars.at(m_cur_var.value()).values = text;
		m_cur_var.reset();
		m_bin_expr.clear();
	}
private:
	NodeProg prog;
	struct Arr_Var
	{
		int size = -1;
		std::vector<size_t> arr;
		std::vector<int> values;
		std::string type;
		int current_size = 0;
	}; 
	struct STR_Var
	{
		int size = -1;
		std::vector<char> values;
	};

	std::string m_bin_expr;
	std::stringstream m_bss;
	std::stringstream m_data;
	std::stringstream m_output;

	std::vector<std::string> m_temp_vars;
	std::vector<std::variant<int, std::string, TokenType>> if_expr;
	

	std::unordered_map<std::string, int> m_int_values;
	std::unordered_map<std::string, size_t> m_int_vars;
	std::unordered_map<std::string, size_t> m_char_vars;
	std::unordered_map<std::string, char> m_char_values;
	std::unordered_map<std::string, Arr_Var> m_arr_vars;
	std::unordered_map<std::string, std::string> m_types;
	std::unordered_map<std::string, STR_Var> m_string_vars;

	std::optional<std::string> value;
	std::optional<std::string> m_cur_var;
	std::optional<NodeExpr*> temp_log_expr = nullptr;

	size_t m_stack_size = 0;

	int orCount = 0;
	int carry_count = 0;

	bool log_stat = false;
	bool temp_vars = false;
	bool is_bin_expr = false;
};