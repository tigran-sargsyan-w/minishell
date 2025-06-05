# Minishell - Exit Status Test Cases

## 🧾 Exit Code Behavior Expectations

- `$?` must reflect the exit code of the **last executed foreground command**.
- Builtins, external commands, errors, and signals should all update `$?` properly.
- Exit codes are important for shell logic, pipelines, and scripting.

---

| #  | Description                          | Command                           | Expected `$?` Result        | Checked |
|:--:|:-------------------------------------|:----------------------------------|:----------------------------|:-------:|
| 1  | Successful command                   | `true`                            | `0`                         | ✅ |
| 2  | Unsuccessful command                 | `false`                           | `1`                         | ✅ |
| 3  | Command with missing file            | `ls /nonexistent`                 | `2` or `1`                  | ✅ |
| 4  | Custom exit code                     | `exit 42`                         | `42`                        | ✅ |
| 5  | `cat` with no input (Ctrl+D)         | `cat` → Ctrl+D                    | `0`                         | ✅ |
| 6  | Invalid command                      | `not_a_command`                   | `127`                       | ✅ |
| 7  | Exit inside pipeline                 | `true \| exit 5`                  | `5`                         | ❌ |
| 8  | `cd` to nonexistent directory        | `cd /does/not/exist`              | `1`                         | ✅ |
| 9  | Builtin success                      | `export VAR=42`                   | `0`                         | ✅ |
| 10 | Unset nonexistent variable           | `unset VAR_NOT_SET`               | `0`                         | ✅ |
| 11 | Simple echo                          | `echo Hello`                      | `0`                         | ✅ |
| 12 | Heredoc with valid delimiter         | `cat << EOF`...`EOF`              | `0`                         | ✅ |
| 13 | Redirect to invalid file             | `ls > /no_perm/file.txt`          | `1`                         | ❌ |
| 14 | Background command (if supported)    | `sleep 1 &` then `echo $?`        | `0`                         | ✅ |
| 15 | Killed by signal                     | `bash -c 'kill -s KILL $$'`       | `137` (`128 + 9`)           | ✅ |
| 16 | Pipe: true \| false                  | `true \| false`                   | `1`                         | ✅ |
| 17 | Pipe: false \| true                  | `false \| true`                   | `0`                         | ✅ |
| 18 | SIGKILL self                         | `bash -c 'kill -9 $$'`            | `137` (`128+9`)             | ✅ |
| 19 | SIGINT (Ctrl+C) on sleep             | `sleep 5` then Ctrl+C             | `130` (`128+2`)             | ✅ |
| 20 | SIGQUIT (Ctrl+\\) on sleep           | `sleep 5` then Ctrl+\             | `131` (`128+3`)             | ✅ |
| 21 | Subshell exit code                   | `bash -c "exit 77"`               | `77`                        | ✅ |
| 22 | Grouped command exit                 | `(exit 3)`                        | `3`                         | ✅ |
| 23 | Multiple piped results               | `false \| true \| false`          | `1`                         | ✅ |
| 24 | Pipe at the beginning                | `\| ls`                           | `2`                         | ❌ |
| 25 | Logical OR with missing  command     | `ls \|\|`                         | `2`                         | ❌ |
| 26 | Unclosed double quote                | `echo "unterminated`              | `2`                         | ❌ |
| 27 | Redirect without filename            | `>`                               | `2`                         | ❌ |
| 28 | Heredoc without delimiter            | `cat <<`                          | `2`                         | ❌ |
| 29 | Double AND with missing command      | `cd && && ls`                     | `2`                         | ❌ |
| 30 | Unsupported redirect operator        | `ls >\| file`                     | `2`                         | ❌ |
| 31 | Redirect without target              | `echo >`                          | `2`                         | ❌ |