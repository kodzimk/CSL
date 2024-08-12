

#include"generator.h"


int main(int argc,char** argv)
{
    int a = 1 > 2;
    std::cout << a << std::endl;
	std::string filename = "main.csl";
	//std::cout << "Enter file name: ";
	//std::getline(std::cin, filename);

    std::string source;
    {
        std::stringstream contents_stream;
        std::fstream input(filename, std::ios::in);
        contents_stream << input.rdbuf();
        source = contents_stream.str();
    }

    Tokenize tokenize(source);

    std::vector<Token> tokens = tokenize.tokenazetion();
    Parser parser(tokens);

    std::optional<NodeProg> prog = parser.parse_prog();

    if (!prog.has_value())
    {
        exit(EXIT_FAILURE);
        std::cerr << "Problem with generation code!!!" << std::endl;
    }

    {
        Generator generator(prog.value(),parser.m_vars);
        std::fstream file("out.asm", std::ios::out);
        file << generator.gen_prog();;
    }


    system("bash");

	return 0;
}