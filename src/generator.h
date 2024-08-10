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
				if (gen.m_int_vars.contains(term_var->name))
				{
					const auto& var = gen.m_int_vars.at(term_var->name);
					std::stringstream offset;
					offset << "QWORD [rsp + " << (gen.m_stack_size - var - 1) * 8 << "]\n";
					gen.push(offset.str());
				}
				else if(gen.m_char_vars.contains(term_var->eqName) && gen.m_char_vars.contains(term_var->name))
				{
					gen.m_output << "    mov rax," << "[" << term_var->name << "]" << '\n';

					gen.m_output << "    mov [" << term_var->eqName << "]," << "rax" << "\n";
				}
				else
				{
						gen.m_output << "   mov rax, " <<  term_var->value.value() << '\n';
						gen.push("rax");
					
				}
			}
			void operator()(NodeTermParen* term_paren)
			{
				gen.gen_expr(term_paren->expr);
			}
			void operator()(const NodeWordCharVal* char_val)
			{
				if (!gen.m_char_vars.contains(char_val->name))
				{
					std::vector<std::string> s = { "'",char_val->value.value(),"'" };
					gen.m_data << char_val->name << " db " << s[0] << s[1] << s[2] << "\n";
					gen.m_char_vars[char_val->name] = 0;
				}
				else
				{
					std::vector<std::string> s = { "'",char_val->value.value(),"'" };
					gen.m_output << "    mov [" << char_val->name << "], " << "dword " << s[0] << s[1] << s[2] << "\n";
				}
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
			void operator()(const NodeStatPrint* stat_print)
			{
				gen.m_output << "    mov edx," << 1 << "\n";
				gen.m_output << "    mov ecx," << stat_print->variableName << "\n";
				gen.m_output << "    mov ebx," << 1 << "\n";
				gen.m_output << "    mov eax," << 4 << "\n";
				gen.m_output << "    int 0x80\n";
			}
			void operator()(const NodeStatVar* stat_var)
			{
				if (gen.m_int_vars.find(stat_var->name) == gen.m_int_vars.end() && gen.m_char_vars.find(stat_var->name) == gen.m_char_vars.end())
				{
					if(stat_var->type != "char")
					   gen.m_int_vars[stat_var->name] = gen.m_stack_size;

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
				if (gen.m_int_vars.contains(stat_eq->variableName) || gen.m_char_vars.contains(stat_eq->variableName))
				{
					gen.gen_expr(stat_eq->expr);

					if(gen.m_int_vars.contains(stat_eq->variableName))
					gen.m_int_vars.at(stat_eq->variableName) = gen.m_stack_size - 1;
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
		m_data << "section .data\n";

		m_output << "section .text\n";
		m_output << "global _start\n";
		m_output << "_start:\n";
		

		for (int i = 0; i < prog.stats.size(); i++)
		{
			gen_stat(prog.stats[i]);
		}

		
		std::string output = m_data.str();
		output += '\n';
		output += m_output.str();

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
	std::stringstream m_data;
	std::unordered_map<std::string,size_t> m_int_vars;
	std::unordered_map<std::string, int> m_char_vars;
};