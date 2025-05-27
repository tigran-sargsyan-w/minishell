# Minishell
This project is about creating a simple shell. Yes, your own little bash. You will learn a lot about processes and file descriptors.

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
