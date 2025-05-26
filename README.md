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

# Minishell - Executor Test Cases

## ⚙️ Execution Behavior Legend
- Output content (e.g. stdout to screen or file)
- Exit status (retrieved by running `echo $?`)
- Errors such as "command not found" or permission denied are expected in some tests.

---

| # | Description | Command | Expected Output / Behavior | Checked |
|:-|:------------|:--------|:----------------------------|:--------|
| 1 | Simple echo | `echo hello` | `hello\n`, exit code `0` | ✅ |
| 2 | Multiple arguments | `echo one two three` | `one two three\n`, exit code `0` | ✅ |
| 3 | Output redirection (truncate) | `echo hi > out.txt` | `out.txt` contains `hi\n` | ✅ |
| 4 | Output redirection (append) | `echo again >> out.txt` | `out.txt` now contains `hi\nagain\n` | ✅ |
| 5 | Input redirection | `cat < out.txt` | Outputs file content, exit code `0` | ✅ |
| 6 | Pipe between two commands | `echo hello \| wc -c` | `6\n`, exit code `0` | ✅ |
| 7 | Pipe chain (3 commands) | `seq 1 5 \| grep 3 \| wc -l` | `1\n`, exit code `0` | ✅ |
| 8 | Redirection and pipe together | `cat < out.txt \| wc -w > count.txt` | `count.txt` contains word count, e.g. `2\n` | ✅ |
| 9 | Command not found | `nosuchcommand` | Error `command not found`, exit code `127` | ✅ |
| 10 | Permission denied | `./non_exec_file` | Error: permission denied, exit code `126` | ✅ |
| 11 | Pipe with empty command after | `echo hi \|` | Syntax error, exit code `2` | ✅ |
| 12 | Pipe at the start of input | `\| grep something` | Syntax error, exit code `2` | ✅ |

---

# Minishell - Redirection Test Cases

## ⚙️ Legend for Redirection Test Cases
- **stdout**: what is printed to standard output (the console)  
- **[filename]**: content of the named file after the command runs  
- **Exit code**: shell exit status (`echo $?`)  
- **Error**: expected error messages (file open/create errors, permission denied, command not found)

---
| # | Command | Expected Output / Behavior | Checked |
|:-:|:---------------------------------------------------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------|:--------|
| 1 | `echo hi` | stdout: `hi\n`<br>exit code: `0`<br>no files created | ✅ |
| 2 | `echo hi > out.txt` | `[out.txt]`: `hi\n`<br>stdout empty<br>exit code: `0` | ✅ |
| 3 | `echo again >> out.txt` | `[out.txt]`: `hi\n again\n`<br>stdout empty<br>exit code: `0` | ✅ |
| 4 | `cat < infile.txt` | stdout: contents of `infile.txt`<br>exit code: `0`<br>`infile.txt` unchanged | ✅ |
| 5 | `cat << EOF`<br>`foo`<br>`bar`<br>`EOF` | stdout: `foo\n bar\n`<br>exit code: `0`<br>no files created | ✅ |
| 6 | `echo hi > out1 > out2 > out3` | `[out1]`, `[out2]`: exist and are empty<br>`[out3]`: `hi\n`<br>stdout empty<br>exit code: `0` | ✅ |
| 7 | `echo hi >> out1 >> out2` | `[out1]`: exists and is empty<br>`[out2]`: `hi\n`<br>stdout empty<br>exit code: `0` | ✅ |
| 8 | `cat < in1.txt < in2.txt` | stdout: contents of `in2.txt` only<br>exit code: `0`<br>`in1.txt`, `in2.txt` unchanged | ✅ |
| 9 | `grep foo < in.txt > out.txt` | `[out.txt]`: lines matching `foo` from `in.txt`<br>stdout empty<br>exit code: `0` | ✅ |
| 10 | `grep foo < in1.txt < in2.txt > out1.txt >> out2.txt` | reads from `in2.txt`<br>`[out1.txt]`: overwritten with matches<br>`[out2.txt]`: appended with matches<br>stdout empty<br>exit code: `0` | ✅ |
| 11 | `echo hi > out1.txt < in.txt` | `[out1.txt]`: `hi\n`<br>stdout empty<br>exit code: `0` | ✅ |
| 12 | `cat << EOF > out.txt` <br>`foo`<br>`bar`<br>`EOF` | `[out.txt]`: `foo\n bar\n` <br>stdout empty<br>exit code: `0` | ✅ |
| 13 | `cat < out1 < out2 \| grep o > out3 > out4` | reads from `out2`<br>`[out3]`: exists and is empty<br>`[out4]`: contains lines with 'o' from `out2`<br>stdout empty<br>exit code: `0` | ✅ |

---
# Minishell - Quotes & Expansions Test Cases

## ⚙️ Quotes & Expansion Behavior Legend
- Output content (e.g. stdout, showing `\n` as newline)  
- Exit status (retrieved by running `echo $?`)  
- Single quotes: literal, no variable expansion  
- Double quotes: allow variable expansion  
- Unquoted: allow variable expansion  
- Escaped or invalid `$`: treated as literal  

---

| #  | Command                          | Expected Output / Behavior                                    | Checked |
|:--:|:---------------------------------|:--------------------------------------------------------------|:--------|
| 1  | `echo $HOME`                     | `<HOME-value>\n`, <br> exit code `0`                          | ✅ |
| 2  | `echo "$HOME"`                   | `<HOME-value>\n`, <br> exit code `0`                          | ✅ |
| 3  | `echo '$HOME'`                   | `$HOME\n`, <br> exit code `0`                                 | ❌ |
| 4  | `echo pre"$HOME"post`            | `pre<HOME-value>post\n`, <br> exit code `0`                   | ❌ |
| 5  | `echo pre'$HOME'post`            | `pre$HOMEpost\n`, <br> exit code `0`                          | ❌ |
| 6  | `echo "$USER:$HOME"`             | `<USER-value>:<HOME-value>\n`, <br> exit code `0`             | ✅ |
| 7  | `echo $UNSET_VAR`                | `\n` (empty), <br> exit code `0`                              | ✅ |
| 8  | `echo "$UNSET_VAR"`              | `\n` (empty), <br> exit code `0`                              | ✅ |
| 9  | `echo $?`                        | `<last_status>\n`, <br> exit code `0`                         | ✅ |
| 10 | `echo "Status=$?"`               | `Status=<last_status>\n`, <br> exit code `0`                  | ✅ |
| 11 | `echo "Cost is 5$"`              | `Cost is 5$\n`, <br> exit code `0`                            | ✅ |
| 12 | `echo ${HOME}`                   | `${HOME}\n`, <br> exit code `0`                               | ❌ |
| 13 | `echo $-foo`                     | `-foo\n`, <br> exit code `0`                                  | ❌ |
| 14 | `echo a$HOMEb${UNSET}_$?`        | `a<HOME-value>b_<last_status>\n`, <br> exit code `0`          | ❌ |
| 15 | `echo "Path is $USER/bin:$HOME"` | `Path is <USER-value>/bin:<HOME-value>\n`, <br> exit code `0` | ✅ |
| 16 | `ls $HOME`                       | Directory listing of `$HOME`, <br> exit code `0`              | ✅ |
| 17 | `ls "$HOME"`                     | Directory listing of `$HOME`, <br> exit code `0`              | ✅ |
| 18 | `ls '$HOME'`                     | `ls: cannot access '$HOME': No such file or directory\n`, <br> exit code `2` | ✅ |
| 19 | `touch $PWD/testfile`            | no stdout, <br> creates file `$PWD/testfile`, exit code `0`   | ✅ |
| 20 | `rm $PWD/testfile`               | no stdout, <br> removes file `$HOME/testfile`, exit code `0`  | ✅ |
| 21 | `touch "$PWD"/testfile`          | no stdout, <br> creates file `$PWD/testfile`, exit code `0`   | ❌ |
| 22 | `rm "$PWD"/testfile`             | no stdout, <br> removes file `$HOME/testfile`, exit code `0`  | ❌ |
