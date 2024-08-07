#pragma once

#include<vector>
#include<variant>
#include<string>
#include<fstream>
#include<optional>
#include<iostream>
#include<sstream>
#include<unordered_map>

#include"parser.h"

class Generator
{
public:
	Generator(NodeProg prog)
		: prog(prog)
	{

	}

	void gen_term(NodeTerm term)
	{
		struct TermVisitor {
			Generator& gen;
			void operator()(const NodeTermIntVal int_val)
			{
				gen.m_output << "   mov rax," << int_val.value.value() << '\n';
				gen.push("rax");
			}
			void operator()(const NodeTermVar term_var)
			{
				if (gen.m_vars.contains(term_var.name))
				{
					const auto& var = gen.m_vars.at(term_var.name);
					std::stringstream offset;
					offset << "QWORD [rsp + " << (gen.m_stack_size - var - 1) * 8 << "]\n";
					gen.push(offset.str());
				}
				else
				{
					gen.m_output << "   mov rax," << term_var.value.value() << '\n';
					gen.push("rax");
				}
			}
		};
		TermVisitor visitor = { .gen = *this };
		std::visit(visitor, term.term);
	}

	void gen_expr(NodeExpr expr)
	{
		struct ExprVisitor {
			Generator& gen;
			void operator()(const NodeTerm term)
			{
				gen.gen_term(term);
			}
		};
		ExprVisitor visitor = { .gen = *this };
		visitor(expr.term);
	}

	void gen_stat(NodeStat stat)
	{
		struct StatVisitor {
			Generator& gen;
			void operator()(const NodeStatExit stat_exit)
			{
				gen.gen_expr(stat_exit.expr);
				gen.m_output << "    mov rax,60\n";
				gen.pop("rdi");
				gen.m_output << "    syscall\n";
			}
			void operator()(const NodeStatVar stat_var)
			{
				if (gen.m_vars.find(stat_var.name) == gen.m_vars.end())
				{
					gen.m_vars[stat_var.name] = gen.m_stack_size;
					gen.gen_expr(stat_var.expr);
				}
				else
				{
					std::cerr << "Variable Exist!!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			void operator()(const NodeStateEq stat_eq)
			{
				if (gen.m_vars.contains(stat_eq.variableName))
				{
					gen.gen_expr(stat_eq.expr);
					gen.push("rax");
					gen.m_vars[stat_eq.variableName] = gen.m_stack_size-1;
		
				}
				else
				{
					std::cerr << "Cant find variable not declared!!!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		};
		StatVisitor visitor = { .gen = *this };
		std::visit(visitor, stat.stat);
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