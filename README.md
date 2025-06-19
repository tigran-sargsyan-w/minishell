# Minishell 🐚✨

✅ **Completed**: Mandatory (101/100)  

![42 Logo](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTXfAZMOWHDQ3DKE63A9jWhIqQaKcKqUIXvzg&s)

**Minishell** is a tiny Unix shell written in C for the 42 School curriculum.  
It reproduces an essential subset of *bash* behaviour — command parsing, environment-variable expansion, redirections, pipelines, built-ins and proper signal handling — all without forking off to an external parser. The goal is to give you hands-on experience with lexical analysis, recursive-descent parsing, process control (`fork` / `execve` / `waitpid`), file-descriptor juggling and the subtleties of POSIX signals.

![Minishell](https://i.ibb.co/GL2Spnf/image.png)

---

## 📚 Table of Contents
- [Description](#description)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [How It Works](#how-it-works)
- [Tests](#tests)
- [Troubleshooting](#troubleshooting)

---

## 📝 Description

`minishell` launches an interactive prompt that lets you execute commands just like in your favourite terminal.  
Under the hood the program first tokenises the user input, expands shell variables (`$HOME`, `$?`, …) and quotes, builds an abstract syntax tree, and finally executes it while respecting Unix semantics for pipes and redirections.  
The mandatory scope follows the subject of 42 School exactly; bonus features can be toggled in the Makefile.

---

## 🚀 Features

| 🗂️ Category         | ✨ Details |
|---------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Built-ins**       | `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` |
| **Pipelines `\|`**   | Unlimited number of commands connected with pipes (`ls -l \| grep .c \| wc -l`) |
| **Redirections**    | `>`, `>>`, `<`, `<<` (here-doc with proper Ctrl-C cancel) |
| **Quoting Rules**   | Handles single quotes, double quotes and escaped characters exactly like *bash* |
| **Environment Vars**| `$VAR` / `${VAR}` expansion, `$?` exit-status expansion |
| **Signal Handling** | Ctrl-C interrupts the current command without quitting the shell; Ctrl-\ behaves like *bash*; heredoc aborts cleanly on Ctrl-C |
| **Executable Search** | Respects `PATH`, fallback to relative/absolute paths |
| **Memory-Safe**     | No leaks (checked with Valgrind 🩻), clean exit on EOF or `exit` |
| **Bonus Switches**  | Toggle wildcard expansion, logical operators `&&` / `\|\|`, parentheses and more via **make bonus** (*if implemented*) |

---

## 🛠️ Requirements

- **GCC / Clang** supporting C99  
- **Make** utility  
- **GNU Readline** (mandatory subject requirement)  
- **Unix-like OS** (Linux 🐧 or macOS 🍎 tested)

---

## 📦 Installation

```
git clone https://github.com/tigran-sargsyan-w/minishell.git
cd minishell
make           # builds mandatory version
# make bonus   # builds bonus version (optional)
```

`libft` is included as a git subdirectory and will be built automatically.

---

## ▶️ Usage

```
./minishell
minishell$ echo "Hello, World! 🌍"
Hello, World! 🌍
minishell$ ls | grep '\.c$' > sources.txt
minishell$ cat <<EOF
> multi-line
> heredoc
> example
> EOF
multi-line
heredoc
example
minishell$ echo "last exit code was $?"
last exit code was 0
minishell$ exit
```

---

## 🔍 How It Works

1. **Lexer** — splits the input line into tokens (words, operators, quoted strings), respecting escaping rules.  
2. **Parser** — builds a syntax tree of simple commands, I/O redirections and pipes; performs environment-variable expansion and quote removal.  
3. **Executor**  
   * creates the required number of processes with `fork()`;  
   * sets up pipes and redirections (`dup2`);  
   * runs built-ins in the parent process when possible (`cd`, `export`, `exit`);  
   * waits for the last child and stores its exit status in `$?`.  
4. **Signal Layer** — installs custom handlers for `SIGINT` and `SIGQUIT` to mimic *bash* behaviour both in the main prompt and inside heredocs.  
5. **Resource Cleanup** — frees every allocated structure and closes all file descriptors before returning to the prompt (or exiting).

---

## 🗄️ Repository Layout

```
.
├── Makefile
├── include/          # public headers
├── libft/            # custom C standard-library re-implementation
├── src/
│   ├── builtin/      # shell built-ins
│   ├── env/          # env-list abstraction
│   ├── executor/     # pipeline & redirection logic
│   ├── lexer/        # lexical analyser
│   ├── minishell_utils/ # prompt loop, signals, helpers
│   ├── parser/       # AST builder and expander
│   └── minishell.c   # Entry point
└── tests/            # markdown specs & scripts for regression tests
```
---

## ✅ Tests

Below are organized test suites covering all core components of the project:

| 🧩 Module                | 🔗 Test Case File |    |
|-------------------------|------------------|-----|
| 🧠 Lexer                | [LEXER.md](tests/LEXER.md) | ✅ |
| 🧷 Parser               | [PARSER.md](tests/PARSER.md) | ✅ |
| 🗣️ Quotes & Expansion  | [QUOTES_AND_EXPANSION.md](tests/QUOTES_AND_EXPANSION.md) | ✅ |
| 🔀 Redirection         | [REDIRECTION.md](tests/REDIRECTION.md) | ✅ |
| ⚙️ Executor            | [EXECUTOR.md](tests/EXECUTOR.md) | ✅ |
| 🧾 Exit Status (`$?`)  | [EXIT_STATUS.md](tests/EXIT_STATUS.md) | ✅ |

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

---

## 🧩 Troubleshooting

| 😖 Symptom | 🛠️ Checklist |
|-----------|--------------|
| **Command not found / wrong binary** | Is your `$PATH` set correctly? Use `env | grep ^PATH=` inside minishell. |
| **Redirection fails with “Bad file descriptor”** | Verify file-descriptor lifetime; ensure you close unused pipe ends. |
| **Ctrl-C kills the whole shell** | Make sure the parent process resets the default handler **after** forking. |
| **Memory leaks reported by Valgrind** | Double-check cleanup of the AST and environment list on each loop iteration. |
| **Heredoc ignores variables** | Variable expansion happens *before* heredoc delimiter matching — re-evaluate your lexer rules. |

---

Happy hacking — may your prompt never segfault! 🐚💥
