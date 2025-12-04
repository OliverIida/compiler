#pragma once // tells compiler to read this file only once

#include <string> // for working with strings
#include <vector> // for working with vectors

// Tokens read from tokenized.txt
enum class TokenKind // list of all possible token types
{
    Identifier,
    Number,
    Plus,
    Minus,
    Star,
    Slash,
    Assign,
    LParen,
    RParen,
    End,
};

struct Token
{
    int line = 0; // stores the line number
    TokenKind kind = TokenKind::End; // stores the type of the token
    std::string value; // only for identifiers and numbers
};

enum class ASTNodeType // list of all possible node types
{
    Program,
    Assignment,
    BinaryOp,
    NumberLiteral,
    Identifier,
    Call,
};

struct ASTNode
{
    ASTNodeType type;
    std::string value;                // operator, identifier, literal
    std::vector<ASTNode*> children;   // C++ vector is OK
};
