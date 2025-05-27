# Minishell - Redirection Test Cases

## ⚙️ Legend for Redirection Test Cases
- **stdout**: what is printed to standard output (the console)  
- **[filename]**: content of the named file after the command runs  
- **Exit code**: shell exit status (`echo $?`)  
- **Error**: expected error messages (file open/create errors, permission denied, command not found)

---
| # | Command | Expected Output / Behavior | Checked |
|:-:|:--------|:----------------------------|:--------|
| 1 | `echo hi` | stdout: `hi\n`<br>exit code: `0`<br>no files created | ✅ |
| 2 | `echo hi > out` | `[out]`: `hi\n`<br>stdout empty<br>exit code: `0` | ✅ |
| 3 | `echo again >> out` | `[out]`: `hi\n again\n`<br>stdout empty<br>exit code: `0` | ✅ |
| 4 | `cat < in` | stdout: contents of `in`<br>exit code: `0`<br>`in` unchanged | ✅ |
| 5 | `cat << EOF`<br>`foo`<br>`bar`<br>`EOF` | stdout: `foo\n bar\n`<br>exit code: `0`<br>no files created | ✅ |
| 6 | `echo hi > out1 > out2 > out3` | `[out1]`, `[out2]`: exist and are empty<br>`[out3]`: `hi\n`<br>stdout empty<br>exit code: `0` | ✅ |
| 7 | `echo hi >> out1 >> out2` | `[out1]`: exists and is empty<br>`[out2]`: `hi\n`<br>stdout empty<br>exit code: `0` | ✅ |
| 8 | `ls > in1` | `[in1]`: list of current directory entries (stdout of `ls`)<br>stdout empty<br>exit code: `0` | ✅ |
| 9 | `ls -l > in2` | `[in2]`: detailed list of directory contents<br>stdout empty<br>exit code: `0` | ✅ |
| 10 | `cat < in1 < in2` | stdout: contents of `in2` only<br>exit code: `0`<br>`in1`, `in2` unchanged | ✅ |
| 11 | `grep in < in1 > out` | `[out]`: lines matching `in` from `in1`<br>stdout empty<br>exit code: `0` | ✅ |
| 12 | `grep in < in1 < in2 > out1 >> out2` | reads from `in2`<br>`[out1]`: overwritten with matches<br>`[out2]`: appended with matches<br>stdout empty<br>exit: `0` | ✅ |
| 13 | `echo hi > out1 < in1` | `[out1]`: `hi\n`<br>stdout empty<br>exit code: `0` | ✅ |
| 14 | `cat << EOF > out1`<br>`foo`<br>`bar`<br>`EOF` | `[out1]`: `foo\nbar\n`<br>stdout empty<br>exit code: `0` | ✅ |
| 15 | `cat < out1 < out2 \| grep inc > out3 > out4` | reads from `out2`<br>`[out3]`: exists and is empty<br>`[out4]`: contains lines with `'inc'` from `out2`<br>stdout empty<br>exit: `0` | ✅ |
