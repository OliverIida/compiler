#pragma once // tells compiler to read this file only once

#include <fstream> // writing & reading files
#include <iostream> // for printing
#include <string> // for working with strings
#include <cctype> // to check character types

typedef unsigned long u32; // type definition
typedef unsigned long long u64; // type definition
constexpr u32 U32_MAX = 4294967294u; // defines a 32-bit constant integer

// reading the file
struct SourceFile
{
    char* data = nullptr; // points to where the file starts
    u32 size = 0;

    // reads the whole file into memory
    bool Read(const char* path)
    {
        std::ifstream f(path, std::ios::binary | std::ios::ate); // jumping to the end of the file
        if (!f.is_open())
        {
            std::cerr << "Couldn't open the file: " << path << "\n";
            return false;
        }

        std::streamsize fileSize = f.tellg(); // getting the file size
        f.seekg(0, std::ios::beg); // jumping to beginning & start reading 

        if (fileSize <= 0) // checks if file is too small
        {
            std::cerr << "The file is too small\n";
            return false;
        }

        if (fileSize >= U32_MAX) // checks if file is too large
        {
            std::cerr << "The file is too large\n";
            return false;
        }

        size = static_cast<u32>(fileSize);
        data = new char[size + 1]; // getting the memory for file + 1byte

        if (!f.read(data, size)) // reading file into memory
        {
            std::cerr << "Reading the file was unsuccessful\n";
            delete[] data;
            data = nullptr;
            size = 0;
            return false;
        }

        data[size] = '\0'; // adding a zero byte to the end, so it knows where the string ends
        return true;
    }

    ~SourceFile() // destructor frees allocated memory when SourceFile object is destroyed
    {
        delete[] data;
    }
};

// Tokens
enum class TokenType // list of all possible symbols
{
    Invalid,
    Identifier,
    Number,

    Plus,       // +
    Minus,      // -
    Star,       // *
    Slash,      // /
    Assign,     // =

    LParen,     // (
    RParen,     // )

    End         // end of file
};

inline const char* GetTokenTypeName(TokenType t)
{
    switch (t) // check which type is t and return symbol
    {
    case TokenType::Identifier: return "identifier";
    case TokenType::Number:     return "number";
    case TokenType::Plus:       return "+";
    case TokenType::Minus:      return "-";
    case TokenType::Star:       return "*";
    case TokenType::Slash:      return "/";
    case TokenType::Assign:     return "=";
    case TokenType::LParen:     return "(";
    case TokenType::RParen:     return ")";
    case TokenType::End:        return "end";
    default:                    return "invalid";
    }
}

struct TokenPos // stores the source line number
{
    int line = 1;
};

struct Token : TokenPos // inheritance
{
    TokenType type = TokenType::Invalid; // stores the type of token
    std::string value; // stores the value of token
};
