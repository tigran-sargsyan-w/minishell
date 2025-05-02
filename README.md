# minishell
This project is about creating a simple shell. Yes, your own little bash. You will learn a lot about processes and file descriptors.

# Minishell - Lexer Test Cases

## 📜 Token Legend
- `WORD(...)` — normal word token
- `PIPE(|)` — pipe token `|`
- `REDIRECT_OUT(>)` — output redirection token `>`
- `REDIRECT_IN(<)` — input redirection token `<`
- Errors are also expected results in some tests.

## How to mark tests manually:
- If the test has not yet been passed, the Checked column shows ⬜ (empty cell).
- If the test is successfully passed, you replace ⬜ with ✅ (check mark).

---

| # | Description | Command | Expected Tokens or Error | Checked |
|:-|:-------------|:--------|:-------------------------|:--------|
| 1 | Basic command with flag | `ls -l` | `[WORD(ls), WORD(-l)]` | ✅ |
| 2 | Echo with multiple words | `echo hello world` | `[WORD(echo), WORD(hello), WORD(world)]` | ✅ |
| 3 | Pipe between two commands | `ls -l \| grep txt` | `[WORD(ls), WORD(-l), PIPE(\|), WORD(grep), WORD(txt)]` | ✅ |
| 4 | Output redirection | `cat file.txt > output.txt` | `[WORD(cat), WORD(file.txt), REDIRECT_OUT(>), WORD(output.txt)]` | ✅ |
| 5 | Input redirection | `sort < input.txt` | `[WORD(sort), REDIRECT_IN(<), WORD(input.txt)]` | ✅ |
| 6 | Double quotes | `echo "hello world"` | `[WORD(echo), WORD(hello world)]` | ✅ |
| 7 | Single quotes | `echo 'single quoted text'` | `[WORD(echo), WORD(single quoted text)]` | ✅ |
| 8 | Nested quotes inside double quotes | `echo "text with 'nested quotes'"` | `[WORD(echo), WORD(text with 'nested quotes')]` | ✅ |
| 9 | Escaped space | `echo hello\ world` | `[WORD(echo), WORD(hello world)]` | ✅ |
| 10 | Escaped pipe character | `echo this\|is\|one\|word` | `[WORD(echo), WORD(this), PIPE(\|), WORD(is), PIPE(\|), WORD(one), PIPE(\|), WORD(word)]` | ✅ |
| 11 | Redirection with quoted filenames | `echo "hello" > "file with space.txt"` | `[WORD(echo), WORD(hello), REDIRECT_OUT(>), WORD(file with space.txt)]` | ✅ |
| 12 | Empty line | `` | `[]` | ✅ |
| 13 | Line with only spaces | `    ` | `[]` | ✅ |
| 14 | Multiple spaces between words | `ls        -l       /tmp` | `[WORD(ls), WORD(-l), WORD(/tmp)]` | ✅ |
| 15 | Unclosed quote error (Handling with parser) | `echo "unclosed quote` | `Error: Unclosed quote` | ✅ |
| 16 | Redirection without a target file (Handling in parser) | `cat file.txt >` | `Error: Missing file after redirection '>'` | ✅ |
| 17 | Pipe at the beginning of the line (Handling in parser) | `\| grep something` | `Error: Pipe at start of input` | ✅ |

---

# Minishell - Parser Test Cases

## 📦 Command Parsing Expectations

- Each `Command` is parsed into a separate `t_cmd` structure.
- `args[]` contains all arguments and the command itself.
- `infile`, `outfile` hold input/output file names.
- `heredoc = 1` if `<<` used.
- `append = 1` if `>>` used.
- Errors should be caught with appropriate error messages.

---

| # | Description | Command | Expected `t_cmd` Structure or Error | Checked |
|:-|:------------|:--------|:-------------------------------------|:--------|
| 1 | Simple command with args | `ls -l /tmp` | `cmd1: args = [ls, -l, /tmp]` | ✅ |
| 2 | Output redirection | `echo hi > file.txt` | `cmd1: args = [echo, hi], outfile = file.txt` | ✅ |
| 3 | Input redirection | `cat < in.txt` | `cmd1: args = [cat], infile = in.txt` | ✅ |
| 4 | Append redirection | `echo test >> log.txt` | `cmd1: args = [echo, test], outfile = log.txt, append = true` | ✅ |
| 5 | Heredoc usage | `cat << EOF` | `cmd1: args = [cat], infile = EOF, heredoc = true` | ✅ |
| 6 | Two commands with pipe | `ls \| wc -l` | `cmd1: args = [ls] → cmd2: args = [wc, -l]` | ✅ |
| 7 | Command chain with multiple pipes | `ls \| grep .c \| wc -l` | `cmd1 → cmd2 → cmd3` with respective args | ✅ |
| 8 | Command with both redirections | `echo test > out.txt < in.txt` | `cmd1: args = [echo, test], infile = in.txt, outfile = out.txt` | ✅ |
| 9 | Missing file after `>` | `echo hello >` | `Error: syntax error near '>'` | ✅ |
| 10 | Missing file after `<` | `cat <` | `Error: syntax error near '<'` | ✅ |
| 11 | Pipe at start | `\| ls` | `Error: syntax error near '\|'` | ✅ |
| 12 | Pipe at end | `echo hi \|` | `Error: syntax error near 'newline'` | ✅ |
| 13 | Double pipe (invalid) | `ls \|\| wc` | `Error: syntax error near '\|'` | ✅ |
| 14 | Redirection only | `>` | `Error: syntax error near '>'` | ✅ |
| 15 | Redirection after pipe | `ls \| > out.txt` | `Error: syntax error near '>'` | ✅ |

---
