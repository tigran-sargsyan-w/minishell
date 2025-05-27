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
