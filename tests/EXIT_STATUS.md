# Minishell - Exit Status Test Cases

## 🧾 Exit Code Behavior Expectations

- `$?` must reflect the exit code of the **last executed foreground command**.
- Builtins, external commands, errors, and signals should all update `$?` properly.
- Exit codes are important for shell logic, pipelines, and scripting.

---

| #  | Description                          | Command                           | Expected `$?` Result        | Checked |
|:--:|:-------------------------------------|:----------------------------------|:----------------------------|:-------:|
| 1  | Successful command                   | `true`                            | `0`                         | ❌ |
| 2  | Unsuccessful command                 | `false`                           | `1`                         | ❌ |
| 3  | Command with missing file            | `ls /nonexistent`                 | `2` or `1`                  | ❌ |
| 4  | Custom exit code                     | `exit 42`                         | `42`                        | ❌ |
| 5  | `cat` with no input (Ctrl+D)         | `cat` → Ctrl+D                    | `0`                         | ❌ |
| 6  | Invalid command                      | `not_a_command`                   | `127`                       | ❌ |
| 7  | Exit inside pipeline                 | `true \| exit 5`                  | `5`                         | ❌ |
| 8  | `cd` to nonexistent directory        | `cd /does/not/exist`              | `1`                         | ❌ |
| 9  | Builtin success                      | `export VAR=42`                   | `0`                         | ❌ |
| 10 | Unset nonexistent variable           | `unset VAR_NOT_SET`               | `0`                         | ❌ |
| 11 | Simple echo                          | `echo Hello`                      | `0`                         | ❌ |
| 12 | Heredoc with valid delimiter         | `cat << EOF`...`EOF`              | `0`                         | ❌ |
| 13 | Redirect to invalid file             | `ls > /no_perm/file.txt`          | `1`                         | ❌ |
| 14 | Background command (if supported)    | `sleep 1 &` then `echo $?`        | `0`                         | ❌ |
| 15 | Killed by signal                     | `kill -s KILL $$`                 | `137` (`128 + 9`)           | ❌ |
| 16 | Signal INT ignored (bonus/signal)    | `trap "" INT; kill -s INT $$`     | `130`                       | ❌ |
