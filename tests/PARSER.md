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
