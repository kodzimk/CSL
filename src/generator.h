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

#include"parser.h"

class Generator
{
public:
	Generator(NodeProg prog)
		: prog(prog)
	{

	}

	void gen_term(NodeTerm* term)
	{
		struct TermVisitor {
			Generator& gen;
			void operator()(const NodeTermIntVal* int_val)
			{
				gen.m_output << "   mov rax," << int_val->value.value() << '\n';
				gen.push("rax");
			}
			void operator()(const NodeTermVar* term_var)
			{
				if (gen.m_vars.contains(term_var->name))
				{
					const auto& var = gen.m_vars.at(term_var->name);
					std::stringstream offset;
					offset << "QWORD [rsp + " << (gen.m_stack_size - var - 1) * 8 << "]\n";
					gen.push(offset.str());
				}
				else
				{
					if (term_var->value.has_value())
					{
						gen.m_output << "   mov raxASA," << term_var->value.value() << '\n';
						gen.push("rax");
					}
				}
			}
			void operator()( NodeTermParen* term_paren)
			{
				gen.gen_expr(term_paren->expr);
			}
		};
		TermVisitor visitor = { .gen = *this };
		std::visit(visitor, term->var);
	}

	void gen_bin_expr(NodeBinExpr* bin_expr)
	{
		struct BinExprVisitor {
			Generator& gen;

			void operator()(const NodeBinExprSub* sub) 
			{
				gen.gen_expr(sub->rhs);
				gen.gen_expr(sub->lhs);
				gen.pop("rax");
				gen.pop("rbx");
				gen.m_output << "    sub rax, rbx\n";
				gen.push("rax");
			}

			void operator()(const NodeBinExprAdd* add) 
			{
				gen.gen_expr(add->rhs);
				gen.gen_expr(add->lhs);
				gen.pop("rax");
				gen.pop("rbx");
				gen.m_output << "    add rax, rbx\n";
				gen.push("rax");
			}

			void operator()(const NodeBinExprMulti* multi) 
			{
				gen.gen_expr(multi->rhs);
				gen.gen_expr(multi->lhs);
				gen.pop("rax");
				gen.pop("rbx");
				gen.m_output << "    mul rbx\n";
				gen.push("rax");
			}

			void operator()(const NodeBinExprDiv* div) 
			{
				gen.gen_expr(div->rhs);
				gen.gen_expr(div->lhs);
				gen.pop("rax");
				gen.pop("rbx");
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
		};
		ExprVisitor visitor = { .gen = *this };
		std::visit(visitor, expr->var);
	}

	void gen_stat(NodeStat* stat)
	{
		struct StatVisitor {
			Generator& gen;
			void operator()(const NodeStatExit* stat_exit)
			{
				gen.gen_expr(stat_exit->expr);
				gen.m_output << "    mov rax,60\n";
				gen.pop("rdi");
				gen.m_output << "    syscall\n";
			}
			void operator()(const NodeStatVar* stat_var)
			{
				if (gen.m_vars.find(stat_var->name) == gen.m_vars.end())
				{
					gen.m_vars[stat_var->name] = gen.m_stack_size;
					gen.gen_expr(stat_var->expr);
				}
				else
				{
					std::cerr << "Variable Exist!!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			void operator()(const NodeStateEq* stat_eq)
			{
				if (gen.m_vars.contains(stat_eq->variableName))
				{
					gen.gen_expr(stat_eq->expr);
					gen.m_vars[stat_eq->variableName] = gen.m_stack_size - 1;
		               
				}
				else
				{
					std::cerr << "Cant find variable not declared!!!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		};
		StatVisitor visitor = { .gen = *this };
		std::visit(visitor, stat->stat);
	}

	std::string gen_prog()
	{
		m_output << "global _start\n";
		m_output << "_start:\n";
		for (int i = 0; i < prog.stats.size(); i++)
		{
			gen_stat(prog.stats[i]);
		}

		std::string output = m_output.str();
		return output;
	}

private:



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


	size_t m_stack_size = 0;
	NodeProg prog;
	std::stringstream m_output;
	std::unordered_map<std::string, size_t> m_vars;
};