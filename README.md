# Minishell
This project is about creating a simple shell. Yes, your own little bash. You will learn a lot about processes and file descriptors.

---

## 🧪 Test Case Suites

Below are organized test suites covering all core components of the project:

| 🧩 Module                | 🔗 Test Case File |    |
|-------------------------|------------------|-----|
| 🧠 Lexer                | [LEXER.md](tests/LEXER.md) | ❌ |
| 🧷 Parser               | [PARSER.md](tests/PARSER.md) | ✅ |
| 🗣️ Quotes & Expansion  | [QUOTES_AND_EXPANSION.md](tests/QUOTES_AND_EXPANSION.md) | ❌ |
| 🔀 Redirection         | [REDIRECTION.md](tests/REDIRECTION.md) | ✅ |
| ⚙️ Executor            | [EXECUTOR.md](tests/EXECUTOR.md) |       |
| 🧾 Exit Status (`$?`)  | [EXIT_STATUS.md](tests/EXIT_STATUS.md) |     |

---

## 🧪 Builtin Detection Script

To test **unsupported builtin commands** (i.e., those your minishell is **not required** to implement), run:

```bash
./tests/test_builtins.sh
```

This script will:
- Attempt to run various shell builtins like `source`, `bind`, `declare`, etc.
- Show the output or error for each one.
- Help confirm that your shell gracefully reports errors or ignores unsupported commands.

📎 Location: `tests/test_builtins.sh`