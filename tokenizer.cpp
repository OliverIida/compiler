#include <unordered_map> // Hashmap import
#include "include.h" // connects this file to include.h
#include <fstream> // for writing to file

struct Compiler
{
    SourceFile source; // this var stores the file in memory until Compiler works

    char* filePos = nullptr; // pointer to where we are now
    char* fileEnd = nullptr; // pointer showing where the file ends
    Token token; // here we save the info of the new "word"

    bool Init(const char* filepath)
    {
        if (!source.Read(filepath)) // reads file into memory
            return false;

        filePos = source.data; // shows the beginning of the text
        fileEnd = source.data + source.size; // shows the ending of the text + 1 

        token.line = 1; // beginning line is line 1
        return NextToken(); // calling out NextToken to find the first word
    }

    bool NextToken()
    {
        while (filePos < fileEnd)
        {
            char c = *filePos; // looking at the symbol we are pointing at
            if (c == ' ' || c == '\t' || c == '\r')
            {
                ++filePos; // skip to next char
            }
            else if (c == '\n')
            {
                ++filePos;
                ++token.line; // line changed, add 1 
            }
            else
            {
                break;
            }
        }

        if (filePos >= fileEnd)
        {
            token.type = TokenType::End; // set token type to End
            token.value.clear();
            return true;
        }

        char c = *filePos;

        if (std::isalpha((unsigned char)c) || c == '_')
        {
            char* start = filePos; // keep in mind where word started
            ++filePos;
            while (filePos < fileEnd)
            {
                char d = *filePos;
                if (std::isalnum((unsigned char)d) || d == '_')
                    ++filePos;
                else
                    break;
            }

            token.type = TokenType::Identifier;
            token.value.assign(start, filePos - start); // saves the value of word
            return true;
        }

        if (std::isdigit((unsigned char)c) || c == '.')
        {
            char* start = filePos;
            ++filePos;
            while (filePos < fileEnd)
            {
                char d = *filePos;
                if (std::isdigit((unsigned char)d) || d == '.')
                    ++filePos;
                else
                    break;
            }

            token.type = TokenType::Number;
            token.value.assign(start, filePos - start); // saves the value of number
            return true;
        }

        token.value.clear();

        switch (c) // reading symbols and assigning types
        {
        case '+':
            token.type = TokenType::Plus;
            break;
        case '-':
            token.type = TokenType::Minus;
            break;
        case '*':
            token.type = TokenType::Star;
            break;
        case '/':
            token.type = TokenType::Slash;
            break;
        case '=':
            token.type = TokenType::Assign;
            break;
        case '(':
            token.type = TokenType::LParen;
            break;
        case ')':
            token.type = TokenType::RParen;
            break;
        default:
            std::cerr << "Unknown symbol" << c 
                      << "' (ASCII: " << (int)c << ") on line " << token.line << "\n";
            return false;
        }

        ++filePos; // increase position by 1
        return true;
    }
};

int main(int argc, char* argv[]) // main entry point
{
    if (argc < 2)
    {
        std::cerr << "Usage: compiler <input_file>\n";
        return 1;
    }

    Compiler compiler;
    if (!compiler.Init(argv[1])) // making new compiler, reading file
        return 1;

    std::ofstream out("tokenized.txt"); // setting up the file to write to
    if (!out.is_open())
    {
        std::cerr << "Failed to open tokenized.txt for writing\n"; // if file is not open, print error
        return 1;
    }
    
    do
    {
        out << compiler.token.line << ": " << GetTokenTypeName(compiler.token.type); // writing line number and token type

        if (compiler.token.type == TokenType::Identifier ||
            compiler.token.type == TokenType::Number)
        {
            out << " " << compiler.token.value; // writing token value if it exists
        }

        out << "\n"; // writing new line

        if (!compiler.NextToken()) // if next token is not found, print error
            return 1;

    } while (compiler.token.type != TokenType::End); // while token type is not End, keep writing

    return 0; // program ran successfully
}