#pragma once
#include <iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<optional>
#include<vector>
#include<variant>
#include <cassert>
#include <algorithm>

#include"tokenize.h"
#include"parser.h"

class Generator
{
public:
	Generator(NodeProg prog)
		: prog(prog)
	{

	}

    void gen_stmt(NodeStat stat)
    {
        struct StmtVisitor {
            Generator& gen;

            void operator()(const NodeStatExit stmt_exit) 
            {
                gen.m_output << "    mov rax, 60\n";
                gen.m_output << "    mov rdi," << stmt_exit.expr.val.value() << "\n";
                gen.m_output << "    syscall\n";
            }
            
        };

        StmtVisitor visitor{ .gen = *this };
        visitor(stat.node);
    }

	std::string gen_prog()
	{
        m_output << "global _start\n";
        m_output << "_start:\n";

        for (int i = 0; i < prog.stats.size(); i++)
        {
            gen_stmt(prog.stats.at(i));
        }

        std::string str = m_output.str();

        return str;
    }
public:
    std::stringstream m_output;
	NodeProg prog;
};