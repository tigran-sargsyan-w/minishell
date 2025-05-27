# Minishell - Executor Test Cases

## ⚙️ Execution Behavior Legend

- Output content (e.g. stdout to screen or file)
- Exit status (retrieved by running `echo $?`)
- Errors such as "command not found" or permission denied are expected in some tests.

---

| # | Description | Command | Expected Output / Behavior | Checked |
|:-|:------------|:--------|:----------------------------|:--------|
| 1 | Simple echo | `echo hello` | `hello\n`, <br> exit code `0` | ✅ |
| 2 | Multiple arguments | `echo one two three` | `one two three\n`, <br> exit code `0` | ✅ |
| 3 | Output redirection (truncate) | `echo hi > out.txt` | `out.txt` contains `hi\n` | ✅ |
| 4 | Output redirection (append) | `echo again >> out.txt` | `out.txt` now contains `hi\nagain\n` | ✅ |
| 5 | Input redirection | `cat < out.txt` | Outputs file content, <br> exit code `0` | ✅ |
| 6 | Pipe between two commands | `echo hello \| wc -c` | `6\n`, <br> exit code `0` | ✅ |
| 7 | Pipe chain (3 commands) | `seq 1 5 \| grep 3 \| wc -l` | `1\n`, <br> exit code `0` | ✅ |
| 8 | Redirection and pipe together | `cat < out.txt \| wc -w > count.txt` | `count.txt` contains word count, e.g. `2\n` | ✅ |
| 9 | Command not found | `nosuchcommand` | Error `command not found`, <br> exit code `127` | ✅ |
| 10 | Permission denied | `./non_exec_file` | Error: permission denied, <br> exit code `126` | ✅ |
| 11 | Pipe with empty command after | `echo hi \|` | Syntax error, <br> exit code `2` | ✅ |
| 12 | Pipe at the start of input | `\| grep something` | Syntax error, <br> exit code `2` | ✅ |
