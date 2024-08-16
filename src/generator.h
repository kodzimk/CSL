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
	Generator(NodeProg prog,std::unordered_map<std::string,std::string> m_types)
		: prog(prog),
		m_types(m_types)
	{

	}

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
			void operator()(const NodeLogExprGreater* log_greater)
			{
				gen.temp_log_expr = nullptr;
				gen.gen_expr(log_greater->lhs);
				if (gen.if_stat)
				{
					gen.if_expr.push_back(TokenType::greater);
				}
				gen.gen_expr(log_greater->rhs);
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
					gen.if_expr.push_back(TokenType::lesser);
				}
				gen.gen_expr(log_lesser->rhs);
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
					gen.if_expr.push_back(TokenType::equal);
				}
				gen.gen_expr(log_equal->rhs);
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
					gen.if_expr.push_back(TokenType::notequal);
				}
				gen.gen_expr(log_not_equal->rhs);
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
					gen.if_expr.push_back(TokenType::greaterequal);
				}
				gen.gen_expr(log_greater_equal->rhs);
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
					gen.if_expr.push_back(TokenType::lesserequal);
				}
				gen.gen_expr(log_lesser_equal->rhs);
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
				if (gen.if_stat)
				{
					gen.if_expr.push_back(stoi(int_val->value.value()));
				}
				gen.m_output << "   mov rax," << int_val->value.value() << '\n';
				gen.push("rax");
			}
			void operator()(const NodeTermVar* term_var)
			{
			if (gen.m_int_vars.contains(term_var->name))
				{
					if (gen.if_stat)
					{
						gen.if_expr.push_back(term_var->name);
					}
		
					const auto& var = gen.m_int_vars.at(term_var->name);
					std::stringstream offset;
					offset << "QWORD [rsp + " << (gen.m_stack_size - var - 1) * 8 << "]\n";
					gen.push(offset.str());
				}
				else if(find(gen.m_char_vars.begin(), gen.m_char_vars.end(),term_var->eqName) == gen.m_char_vars.end() && find(gen.m_char_vars.begin(), gen.m_char_vars.end(), term_var->name) != gen.m_char_vars.end())
				{
					std::vector<std::string> s = { "'","s","'"};
					gen.m_data << term_var->eqName << " db " << s[0] << s[1] << s[2] << "\n";
					gen.m_char_vars.push_back(term_var->eqName);

					gen.m_output << "    mov rax," << "[" << term_var->name << "]" << '\n';
					gen.m_output << "    mov [" << term_var->eqName << "]," << "rax" << "\n";
					gen.m_output << "    mov [" << term_var->name << "]," << "rax" << "\n";
				}
				else if (find(gen.m_char_vars.begin(), gen.m_char_vars.end(), term_var->eqName) != gen.m_char_vars.end() && find(gen.m_char_vars.begin(), gen.m_char_vars.end(), term_var->name) != gen.m_char_vars.end())
				{
					gen.m_output << "    mov rax," << "[" << term_var->name << "]" << '\n';
					gen.m_output << "    mov [" << term_var->eqName << "]," << "rax" << "\n";
					gen.m_output << "    mov [" << term_var->name << "]," << "rax" << "\n";
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
				if (find(gen.m_char_vars.begin(), gen.m_char_vars.end(), char_val->name) == gen.m_char_vars.end())
				{
					std::vector<std::string> s = { "'",char_val->value.value(),"'" };
					gen.m_data << char_val->name << " db " << s[0] << s[1] << s[2] << "," << "0xA" << "," << "0xD" << "\n";
					gen.m_char_vars.push_back(char_val->name);
				}
				else
				{
					std::vector<std::string> s = { "'",char_val->value.value(),"'" };
					gen.m_output << "    mov [" << char_val->name << "], " << "dword " << s[0] << s[1] << s[2] << "\n";
				}
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
				while (gen.temp_log_expr != nullptr)
				{
					gen.gen_expr(gen.temp_log_expr.value());
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
				}
				else if (elif->pred.has_value()) {
					gen.m_values.clear();
					const std::string end_label = gen.create_label();
					gen.m_output << "    jmp " << end_label << "\n";
					gen.m_output << label << ":\n";
					gen.gen_if_pred(elif->pred.value(), end_label);
					gen.m_output << end_label << ":\n";
				}
				else {
					gen.m_output << label << ":\n";
				}
				gen.m_values.clear();
				gen.m_output << "  \n";
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
				
				gen.pop("rdi");
				gen.m_output << "    jmp exit\n";
			}
			void operator()(const NodeStatPrint* stat_print)
			{
				if (stat_print->variableName.has_value() && gen.m_types.contains(stat_print->variableName.value()) && gen.m_types.at(stat_print->variableName.value()) == "character")
				{
					gen.m_output << "    mov edx,1" << "\n";
					gen.m_output << "    mov ecx," << stat_print->variableName.value() << "\n";
					gen.m_output << "    mov ebx,1" << "\n";
					gen.m_output << "    mov eax,4" << "\n";
					gen.m_output << "    int 0x80\n";
					gen.m_output << "    \n";
				}
				else if (stat_print->type == "character")
				{
					NodeTerm* term = std::get<NodeTerm*>(stat_print->expr->var);
						NodeWordCharVal* val = std::get<NodeWordCharVal*>(term->var);

						gen.m_output << "    mov [temp],"<<"dword '"<< val->value.value() <<"'" <<"\n";
						gen.m_output << "    mov ecx,temp" << "\n";
						gen.m_output << "    mov edx,1" << "\n";
						gen.m_output << "    mov ebx,1" << "\n";
						gen.m_output << "    mov eax,4" << "\n";
						gen.m_output << "    int 0x80\n";
						gen.m_output << "    \n";
				}
				else if (stat_print->type == "integer" || (stat_print->variableName.has_value() && gen.m_types.contains(stat_print->variableName.value()) && gen.m_types.at(stat_print->variableName.value()) == "integer"))
				{
					gen.gen_expr(stat_print->expr);
					gen.pop("rax");
					gen.m_output << "    call _printnumberRAX\n";
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
				if (!gen.m_int_vars.contains(stat_var->name) && find(gen.m_char_vars.begin(), gen.m_char_vars.end(), stat_var->name) == gen.m_char_vars.end())
				{
					if (stat_var->type != "character")
					{
						gen.m_int_vars[stat_var->name] = gen.m_stack_size;
						if(gen.temp_vars)
						  gen.m_int_name.push_back(stat_var->name);
					}

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
				if (gen.m_int_vars.contains(stat_eq->variableName) || find(gen.m_char_vars.begin(), gen.m_char_vars.end(),stat_eq->variableName) != gen.m_char_vars.end())
				{
					gen.gen_expr(stat_eq->expr);
					
					if(gen.m_int_vars.contains(stat_eq->variableName))
					{
						gen.m_int_vars.at(stat_eq->variableName) = gen.m_stack_size-1;
                    }
				}
				else
				{
					std::cerr << "Cant find variable not declared!!!" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			void operator()(const NodeScope* scope) const
			{
				gen.gen_scope(scope);
			}
			void operator()(const NodeStatIf* stmt_if) const
			{
				gen.if_stat = true;
				gen.gen_expr(stmt_if->expr);
				while (gen.temp_log_expr != nullptr)
				{
					gen.gen_expr(gen.temp_log_expr.value());
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
					gen.gen_scope(stmt_if->scope);
					gen.m_output << label << ":\n";
				}
				else if (stmt_if->pred.has_value()) {
					gen.m_values.clear();
					const std::string end_label = gen.create_label();
					gen.m_output << "    jmp " << end_label << "\n";
					gen.m_output << label << ":\n";
					gen.gen_if_pred(stmt_if->pred.value(), end_label);
					gen.m_output << end_label << ":\n";
				}
				else {
					gen.m_output << label << ":\n";
				}
				gen.m_output << "  \n";
				gen.m_values.clear();
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
		m_data << "newLineMsg db 0xA, 0xD\n";
		m_data << "newLineLen equ $ - newLineMsg\n";\
		m_data << "temp db 'a',0xA,0xD\n";

		m_functions << "exit:\n";
		m_functions << "    mov rax, 60\n";
		m_functions << "    syscall\n";

		m_functions << "_printnumberRAX:\n";
		m_functions << "mov rcx, stringBuffer\n";
		m_functions << "mov rbx, 10\n";
		m_functions << "mov[rcx], rbx\n";
		m_functions << "inc rcx\n";
		m_functions << "mov[stringBufferPos], rcx\n";
		m_functions << "_printnumberRAXLoop :\n";
		m_functions << "mov rdx, 0\n";
		m_functions << "mov rbx, 10\n";
		m_functions << "div rbx\n";
		m_functions << "push rax\n";
		m_functions << "add rdx, 48\n";
		m_functions << "mov rcx, [stringBufferPos]\n";
		m_functions << "mov[rcx], dl\n";
		m_functions << "inc rcx\n";
		m_functions << "mov[stringBufferPos], rcx\n";
		m_functions << "pop rax;\n";
		m_functions << "cmp rax, 0\n";
		m_functions << "jne _printnumberRAXLoop\n";
		m_functions << "_printnumberRAXLoop2 :\n";
		m_functions << "mov rcx, [stringBufferPos]\n";
		m_functions << "mov rax, 1\n";
		m_functions << "mov rdi, 1\n";
		m_functions << "mov rsi, rcx\n";
		m_functions << "mov rdx, 1\n";
		m_functions << "syscall\n";
		m_functions << "mov rcx, [stringBufferPos]\n";
		m_functions << "dec rcx\n";
		m_functions << "mov[stringBufferPos], rcx\n";
		m_functions << "cmp rcx, stringBuffer\n";
		m_functions << "jge _printnumberRAXLoop2\n";
		m_functions << "ret\n";
		m_output << "section .text\n";
		m_output << "global _start\n";
		m_output << "_start:\n";
		

		for (int i = 0; i < prog.stats.size(); i++)
		{
			gen_stat(prog.stats[i]);
		}
		std::string output = m_bss.str();
		output += '\n';
		output += m_data.str();
		output += '\n';
		output += m_output.str();
		output += '\n';
		output += m_functions.str();
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
	void begin_scope()
	{
		m_last_index_of_scope.push_back(m_stack_size);
		temp_vars = true;
	}
	void end_scope()
	{
		for (int i = m_last_index_of_scope[m_last_index_of_scope.size() - 1]; i < m_stack_size; i++)
		{
			if (m_int_name.size() > 0)
			{
				m_int_vars.erase(m_int_name[m_int_name.size() - 1]);
				m_int_name.erase(m_int_name.end() - 1);
			}
			pop("rax");
		}
		m_last_index_of_scope.erase(m_last_index_of_scope.end() - 1);
		temp_vars = false;
	}
	std::string create_label()
	{
		std::stringstream ss;
		ss << "label" << m_label_count;
		m_label_count++;
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
		TokenType andOr = TokenType::integer;
		for (int i = 0; i < if_expr.size() - 2; i++)
		{
			if (auto lhs = std::get_if<int>(&if_expr[i]))
			{
				i++;
				if (auto log = std::get_if<TokenType>(&if_expr[i]))
				{
					int val = -1;
					if (log_prec(*log).has_value())
					{
						val = log_prec(*log).value();
						i++;
						if (val == 0)
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
						else if (val == 1)
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
						else if (val == 2)
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
						else if (val == 3)
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
						else if (val == 4)
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
						else if (val == 5)
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

			if (i < if_expr.size())
			{
				if (auto log = std::get_if<TokenType>(&if_expr[i]))
				{
					if (*log == TokenType::AND || *log == TokenType::OR)
						andOr = *log;
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
private:
	std::vector<std::variant<int,std::string, TokenType>> if_expr;
    std::vector<int> m_values;
	size_t m_stack_size = 0;
	NodeProg prog;
	std::stringstream m_output;
	std::stringstream m_data;
	std::stringstream m_bss;
	std::stringstream m_functions;

	struct INT_VAR
	{
		std::string name;
		size_t offset;
	};
	std::unordered_map<std::string, size_t> m_int_vars;
	std::vector<int> m_last_index_of_scope;
	std::vector<std::string> m_int_name;
	std::vector<std::string> m_char_vars;
	std::unordered_map<std::string, std::string> m_types;

	std::optional<NodeExpr*> temp_log_expr = nullptr;

	bool temp_vars = false;
	int m_label_count = 0;
	int carry_count = 0;
	int orCount = 0;
	bool if_stat = false;
};