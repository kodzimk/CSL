#include <iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<optional>
#include<vector>

#include"tokenize.h"


std::string code_generateration(std::vector<Token> tokens)
{
    std::stringstream stream;
    stream << "global _start\n";
    stream << "_start:\n";

    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens.at(i).type == TokenType::ret)
        {
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_val)
            {
                stream << "    mov rax,60\n";
                stream << "    mov rdi," << tokens.at(i + 1).value.value() << "\n";
                stream << "    syscall\n";
                i++;
            }
            else
            {
                std::cout << "Cant find value!!!" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

    }

    std::string str = stream.str();

    return str;
}

int main(int argc,char** argv)
{
	std::string filename;
	std::cout << "Enter file name: ";
	std::getline(std::cin, filename);

    std::string source;
    {
        std::stringstream contents_stream;
        std::fstream input(filename, std::ios::in);
        contents_stream << input.rdbuf();
        source = contents_stream.str();
    }

    Tokenize tokenize(source);

    std::vector<Token> tokens = tokenize.tokenazetion();

    {
        std::fstream out("out.asm", std::ios::out);
        out << code_generateration(tokens);
    }

    system("bash");

	return 0;
}