# Minishell - Lexer Test Cases

## 📜 Token Legend (based on `t_token_type`)

- `WORD(...)` — generic word
- `SQUOTED(...)` — single-quoted string (literal)
- `DQUOTED(...)` — double-quoted string (supports `$`)
- `PIPE(|)` — pipe token
- `REDIR_IN(<)` — input redirection
- `REDIR_OUT(>)` — output redirection
- `HEREDOC(<<)` — heredoc start
- `APPEND(>>)` — output append
- Errors are expected in some tests.

---

| #  | Description                            | Command                                | Expected Tokens or Error                                                   | Checked |
|----|----------------------------------------|----------------------------------------|-----------------------------------------------------------------------------|---------|
| 1  | Basic command with flag                | `ls -l`                                | `[WORD(ls), WORD(-l)]`                                                     | ✅      |
| 2  | Echo with multiple words               | `echo hello world`                     | `[WORD(echo), WORD(hello), WORD(world)]`                                   | ✅      |
| 3  | Pipe between two commands              | `ls -l \| grep txt`                    | `[WORD(ls), WORD(-l), PIPE(\|), WORD(grep), WORD(txt)]`                     | ✅      |
| 4  | Output redirection                     | `cat file.txt > output.txt`           | `[WORD(cat), WORD(file.txt), REDIR_OUT(>), WORD(output.txt)]`             | ✅      |
| 5  | Input redirection                      | `sort < input.txt`                    | `[WORD(sort), REDIR_IN(<), WORD(input.txt)]`                              | ✅      |
| 6  | Double quoted string                   | `echo "hello world"`                  | `[WORD(echo), DQUOTED(hello world)]`                                       | ✅      |
| 7  | Single quoted string                   | `echo 'single quoted text'`           | `[WORD(echo), SQUOTED(single quoted text)]`                                | ✅      |
| 8  | Nested quotes inside double quotes     | `echo "text with 'nested quotes'"`    | `[WORD(echo), DQUOTED(text with 'nested quotes')]`                         | ✅      |
| 9  | Escaped space                          | `echo hello\ world`                   | `[WORD(echo), WORD(hello\),WORD(world)]`                                          | ✅      |
| 10 | Escaped pipe character (treated literally) | `echo this\|is\|one\|word`       | `[WORD(echo), WORD(this\|is\|one\|word)]`                                     | ✅      |
| 11 | Redirection with quoted filenames      | `echo "hello" > "file with space.txt"`| `[WORD(echo), DQUOTED(hello), REDIR_OUT(>), DQUOTED(file with space.txt)]` | ✅      |
| 12 | Empty line                             | ``                                     | `[]`                                                                       | ✅      |
| 13 | Line with only spaces                  | `    `                                 | `[]`                                                                       | ✅      |
| 14 | Multiple spaces between words          | `ls        -l       /tmp`             | `[WORD(ls), WORD(-l), WORD(/tmp)]`                                         | ✅      |
| 15 | Unclosed double quote                  | `echo "unclosed`                      | `Error: Unclosed double quote` (detected in parser)                        | ❌      |
| 16 | Redirection without a target file      | `cat file.txt >`                      | `Error: Missing file after redirection '>'` (parser)                       | ✅      |
| 17 | Pipe at the beginning of the line      | `\| grep something`                   | `Error: Pipe at start of input` (parser)                                   | ✅      |
| 18 | Heredoc operator                       | `cat << limiter`                      | `[WORD(cat), HEREDOC(<<), WORD(limiter)]`                                  | ✅      |
| 19 | Append redirection                     | `echo hi >> log.txt`                  | `[WORD(echo), WORD(hi), APPEND(>>), WORD(log.txt)]`                        | ✅      |
| 20 | Mixed redirections                     | `cmd < in.txt >> out.txt`             | `[WORD(cmd), REDIR_IN(<), WORD(in.txt), APPEND(>>), WORD(out.txt)]`        | ✅      |
