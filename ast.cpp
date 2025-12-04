#include "ast.h" // connects this file to ast.h
#include <fstream> // for working with files
#include <iostream> // for printing
#include <sstream> // for working with strings
#include <vector> // for working with vectors
#include <cstdlib> // for malloc and free
#include <new> // for placement new

static ASTNode* makeNode(ASTNodeType type, const std::string& value = "") // creates a new node
{
    void* mem = malloc(sizeof(ASTNode)); // allocates memory for the node
    ASTNode* node = new (mem) ASTNode;  // placement new - calls constructor (ASTNode constructor)
    node->type = type; // sets the type of the node
    node->value = value; // sets the value of the node
    return node; // returns the new node
}

static void freeAST(ASTNode* node) // frees the memory of the node
{
    for (ASTNode* c : node->children)
        freeAST(c);
    node->~ASTNode();  // explicitly call destructor
    free(node);
}

bool LoadTokensFromFile(const char* path, std::vector<Token>& out) // loads tokens from tokenized.txt
{
    std::ifstream in(path); // opens the file
    if (!in.is_open())
    {
        std::cerr << "Failed to open token file: " << path << "\n"; // if file is not open, print error
        return false;
    }

    std::string lineStr; // stores the line string
    while (std::getline(in, lineStr)) // while there is a line
    {
        if (lineStr.empty()) continue; // if line is empty, skip

        std::istringstream iss(lineStr); // creates a new string stream
        Token t; // stores the token
        char colon; // stores the colon
        std::string kind; // stores the kind

        if (!(iss >> t.line >> colon >> kind)) // if line is not found, skip
            continue;

        if (kind == "identifier") { t.kind = TokenKind::Identifier; iss >> t.value; }
        else if (kind == "number") { t.kind = TokenKind::Number; iss >> t.value; }
        else if (kind == "+")      t.kind = TokenKind::Plus;
        else if (kind == "-")      t.kind = TokenKind::Minus;
        else if (kind == "*")      t.kind = TokenKind::Star;
        else if (kind == "/")      t.kind = TokenKind::Slash;
        else if (kind == "=")      t.kind = TokenKind::Assign;
        else if (kind == "(")      t.kind = TokenKind::LParen;
        else if (kind == ")")      t.kind = TokenKind::RParen;
        else if (kind == "EOF" || kind == "end") t.kind = TokenKind::End;
        else { std::cerr << "Unknown token: " << kind << "\n"; return false; } // if kind is unknown, print error

        out.push_back(t); // adds the token to the vector
    }

    if (out.empty() || out.back().kind != TokenKind::End)
    {
        Token t; // stores the token
        t.kind = TokenKind::End; // sets the token type to End
        out.push_back(t); // adds the token to the vector
    }

    return true;
}

class Parser
{
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {} // constructor

    ASTNode* parseProgram() // parses the program
    {
        ASTNode* root = makeNode(ASTNodeType::Program); // creates a new node for the program

        while (!isAtEnd())
        {
            ASTNode* stmt = parseStatement(); // parses the statement
            if (!stmt) break; // if statement is not found, break
            root->children.push_back(stmt); // adds the statement to the node
        }

        return root; // returns the root node
    }

private:
    const std::vector<Token>& tokens; // stores the tokens
    size_t index = 0; // stores the index

    bool isAtEnd() const // checks if at end
    {
        return index >= tokens.size() || tokens[index].kind == TokenKind::End;
    }

    const Token& cur() const { return tokens[index]; } // returns the current token

    bool match(TokenKind k) // checks if token kind is k
    {
        if (isAtEnd() || tokens[index].kind != k) return false;
        index++;
        return true;
    }

    ASTNode* parseStatement() // parses the statement
    {
        if (cur().kind == TokenKind::Identifier)
        {
            if (cur().value == "print") // if value is print
                return parsePrintStatement(); // parse print statement
            return parseAssignStatement(); // parse assign statement
        }
        std::cerr << "Unexpected token on line " << cur().line << "\n"; // if token is unexpected, print error
        return nullptr;
    }

    ASTNode* parseAssignStatement() // parses the assign statement
    {
        const Token& idTok = cur(); // stores the token
        ASTNode* id = makeNode(ASTNodeType::Identifier, idTok.value); // creates a new node for the identifier
        index++; // increments the index

        if (!match(TokenKind::Assign)) // if token kind is not assign
        {
            std::cerr << "Expected '=' on line " << idTok.line << "\n"; // if token is not assign, print error
            return nullptr;
        }

        ASTNode* expr = parseExpression(); // parses the expression
        ASTNode* assign = makeNode(ASTNodeType::Assignment); // creates a new node for the assignment
        assign->children.push_back(id); // adds the identifier to the node
        assign->children.push_back(expr);
        return assign; // returns the assignment node
    }

    ASTNode* parsePrintStatement() // parses the print statement
    {
        const Token& pTok = cur(); // stores the token
        index++;

        if (!match(TokenKind::LParen))
        {
            std::cerr << "Expected '(' after print\n"; // if token is not left parenthesis, print error
            return nullptr;
        }

        ASTNode* expr = parseExpression(); // parses the expression

        if (!match(TokenKind::RParen))
        {
            std::cerr << "Expected ')'\n"; // if token is not right parenthesis, print error
            freeAST(expr); // frees the memory of the expression
            return nullptr;
        }

        ASTNode* call = makeNode(ASTNodeType::Call, "print"); // creates a new node for the call
        call->children.push_back(expr); // adds the expression to the node
        return call; // returns the call node
    }

    ASTNode* parseExpression() // parses the expression
    {
        ASTNode* node = parseTerm(); // parses the term
        while (!isAtEnd())
        {
            TokenKind k = cur().kind; // stores the token kind
            if (k != TokenKind::Plus && k != TokenKind::Minus)
                break;

            std::string op = (k == TokenKind::Plus ? "+" : "-"); // if token kind is plus, set operator to plus, if token kind is minus, set operator to minus
            index++;

            ASTNode* right = parseTerm(); // parses the term

            ASTNode* bin = makeNode(ASTNodeType::BinaryOp, op); // creates a new node for the binary operation
            bin->children.push_back(node); // adds the node to the node
            bin->children.push_back(right); // adds the right node to the node  
            node = bin; // sets the node to the binary node
        }
        return node;
    }

    ASTNode* parseTerm() // parses the term
    {
        ASTNode* node = parseFactor(); // parses the factor
        while (!isAtEnd())
        {
            TokenKind k = cur().kind; // stores the token kind
            if (k != TokenKind::Star && k != TokenKind::Slash)
                break;

            std::string op = (k == TokenKind::Star ? "*" : "/"); // if token kind is star, set operator to star, if token kind is slash, set operator to slash
            index++;

            ASTNode* right = parseFactor(); // parses the factor

            ASTNode* bin = makeNode(ASTNodeType::BinaryOp, op); // creates a new node for the binary operation
            bin->children.push_back(node); // adds the node to the node
            bin->children.push_back(right); // adds the right node to the node
            node = bin; // sets the node to the binary node
        }
        return node;
    }

    ASTNode* parseFactor() // parses the factor
    {
        const Token& tok = cur(); // stores the token

        if (tok.kind == TokenKind::Number)
        {
            index++;   
            return makeNode(ASTNodeType::NumberLiteral, tok.value); // creates a new node for the number literal
        }

        if (tok.kind == TokenKind::Identifier)
        {
            index++;
            return makeNode(ASTNodeType::Identifier, tok.value); // creates a new node for the identifier
        }

        if (match(TokenKind::LParen))
        {
            ASTNode* expr = parseExpression(); // parses the expression
            if (!match(TokenKind::RParen)) // if token kind is not right parenthesis
                std::cerr << "Expected ')'\n"; // if token is not right parenthesis, print error
            return expr; // returns the expression node
        }

        std::cerr << "Unexpected token " << tok.value << "\n"; // if token is unexpected, print error
        return nullptr;
    }
};

void printASTToFile(const ASTNode* node, std::ofstream& out, int indent = 0) // prints the AST to a file
{
    for (int i = 0; i < indent; i++) out << "  "; // prints the indent

    switch (node->type) // switch statement to print the node type
    {
    case ASTNodeType::Program:      out << "Program"; break;
    case ASTNodeType::Assignment:   out << "Assignment"; break;
    case ASTNodeType::BinaryOp:     out << "BinaryOp " << node->value; break;
    case ASTNodeType::NumberLiteral:out << "Number " << node->value; break;
    case ASTNodeType::Identifier:   out << "Identifier " << node->value; break;
    case ASTNodeType::Call:         out << "Call " << node->value; break;
    }
    out << "\n"; // prints a new line

    for (auto* c : node->children)
        printASTToFile(c, out, indent + 1); // prints the children  
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: ast <tokenized_file>\n";
        return 1;
    }

    std::vector<Token> tokens; // stores the tokens
    if (!LoadTokensFromFile(argv[1], tokens))
        return 1;

    Parser parser(tokens); // creates a new parser
    ASTNode* program = parser.parseProgram(); // parses the program

    std::ofstream out("ast.txt"); // creates a new file
    printASTToFile(program, out); // prints the AST to the file
    out.close();

    freeAST(program); // frees the memory of the AST

    std::cout << "AST written to ast.txt\n"; // prints that the AST was written to the file
    return 0;
}
