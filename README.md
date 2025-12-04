# 🔧 A Simplified Compiler

A simple C++ compiler with tokenizer and AST (Abstract Syntax Tree) builder.

---

## 🎯 Features

- 🔤 **Tokenizer** - Breaks the source code into tokens 
- 🌳 **AST Builder** - Builds syntax tree

## 📝 Example

### 📄 `test.txt` (Input)

```
_testVar123 = (1 - 5 + 3 * 3 * 3 + 2) / (4 - 3 + 1)
var2 = 0
_testVar123 = (5 + 12) / 2
```

### ⬇️ `tokenized.txt` (After Tokenizer)

```
1: identifier _testVar123
1: =
1: (
1: number 1
1: -
1: number 5
1: +
1: number 3
1: *
1: number 3
1: *
1: number 3
1: +
1: number 2
1: )
1: /
1: (
1: number 4
1: -
1: number 3
1: +
1: number 1
1: )
2: identifier var2
2: =
2: number 0
3: identifier _testVar123
3: =
3: (
3: number 5
3: +
3: number 12
3: )
3: /
3: number 2
```

### ⬇️ `ast.txt` (After AST Builder)

```
Program
  Assignment
    Identifier _testVar123
    BinaryOp /
      BinaryOp +
        BinaryOp +
          BinaryOp -
            Number 1
            Number 5
          BinaryOp *
            BinaryOp *
              Number 3
              Number 3
            Number 3
        Number 2
      BinaryOp +
        BinaryOp -
          Number 4
          Number 3
        Number 1
  Assignment
    Identifier var2
    Number 0
  Assignment
    Identifier _testVar123
    BinaryOp /
      BinaryOp +
        Number 5
        Number 12
      Number 2
```

---

## 🚀 Usage

```bash
# Tokenize
g++ tokenizer.cpp -o tokenizer
./tokenizer test.txt

# Parse AST
g++ ast.cpp -o ast
./ast tokenized.txt
```

---

## 🔄 Pipeline

```
┌──────────┐      ┌───────────┐      ┌────────────────┐      ┌─────┐      ┌─────────┐
│ test.txt │  →   │ tokenizer │  →   │ tokenized.txt  │  →   │ ast │  →   │ ast.txt │
└──────────┘      └───────────┘      └────────────────┘      └─────┘      └─────────┘
```
