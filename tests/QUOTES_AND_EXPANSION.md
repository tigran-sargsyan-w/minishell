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
| 3  | `echo '$HOME'`                   | `$HOME\n`, <br> exit code `0`                                 | ✅ |
| 4  | `echo pre"$HOME"post`            | `pre<HOME-value>post\n`, <br> exit code `0`                   | ❌ |
| 5  | `echo pre'$HOME'post`            | `pre$HOMEpost\n`, <br> exit code `0`                          | ❌ |
| 6  | `echo "$USER:$HOME"`             | `<USER-value>:<HOME-value>\n`, <br> exit code `0`             | ✅ |
| 7  | `echo $UNSET_VAR`                | `\n` (empty), <br> exit code `0`                              | ✅ |
| 8  | `echo "$UNSET_VAR"`              | `\n` (empty), <br> exit code `0`                              | ✅ |
| 9  | `echo $?`                        | `<last_status>\n`, <br> exit code `0`                         | ✅ |
| 10 | `echo "Status=$?"`               | `Status=<last_status>\n`, <br> exit code `0`                  | ✅ |
| 11 | `echo "Cost is 5$"`              | `Cost is 5$\n`, <br> exit code `0`                            | ✅ |
| 12 | `echo ${HOME}`                   | `${HOME}\n`, <br> exit code `0`                               | ✅ |
| 13 | `echo $-foo`                     | `-foo\n`, <br> exit code `0`                                  | ❓ |
| 14 | `echo a$HOMEb${UNSET}_$?`        | `a<HOME-value>b_<last_status>\n`, <br> exit code `0`          | ❓ |
| 15 | `echo "Path is $USER/bin:$HOME"` | `Path is <USER-value>/bin:<HOME-value>\n`, <br> exit code `0` | ✅ |
| 16 | `ls $HOME`                       | Directory listing of `$HOME`, <br> exit code `0`              | ✅ |
| 17 | `ls "$HOME"`                     | Directory listing of `$HOME`, <br> exit code `0`              | ✅ |
| 18 | `ls '$HOME'`                     | `ls: cannot access '$HOME': No such file or directory\n`, <br> exit code `2` | ✅ |
| 19 | `touch $PWD/testfile`            | no stdout, <br> creates file `$PWD/testfile`, exit code `0`   | ✅ |
| 20 | `rm $PWD/testfile`               | no stdout, <br> removes file `$HOME/testfile`, exit code `0`  | ✅ |
| 21 | `touch "$PWD"/testfile`          | no stdout, <br> creates file `$PWD/testfile`, exit code `0`   | ❌ |
| 22 | `rm "$PWD"/testfile`             | no stdout, <br> removes file `$HOME/testfile`, exit code `0`  | ❌ |
