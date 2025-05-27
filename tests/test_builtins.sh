#!/bin/sh

# 🛡 Path to the project root (assuming this script is in tests/)
PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
MINISHELL="${PROJECT_DIR}/minishell"

# 📛 Check if the executable file exists
if [ ! -x "$MINISHELL" ]; then
  echo "Error: minishell not found or not executable at $MINISHELL"
  exit 1
fi

# 📦 List of all built-in bash commands, excluding echo, cd, pwd, env, export, unset, exit
builtins=". source [ alias bg bind break builtin case command compgen \
complete continue declare dirs disown enable eval exec fc fg getopts \
hash help history jobs kill let local logout popd printf pushd read \
readonly return set shift shopt suspend test times trap type typeset \
ulimit umask unalias wait"

# 🧪 Files for temporary output
TMP_OUT="$(mktemp "${PROJECT_DIR}/tests/tmp_out.XXXXXX")"
TMP_ERR="$(mktemp "${PROJECT_DIR}/tests/tmp_err.XXXXXX")"

# 🔁 Test each command
i=1
for b in $builtins; do
  echo "[$i] === Testing '$b' ==="

  status=$(
    printf "%s\nexit\n" "$b" \
      | "$MINISHELL" >"$TMP_OUT" 2>"$TMP_ERR"
    echo $?
  )

  echo "-- STDOUT --"
  if [ -s "$TMP_OUT" ]; then
    sed 's/^/    /' "$TMP_OUT"
  else
    echo "    (empty)"
  fi

  echo "-- STDERR --"
  if [ -s "$TMP_ERR" ]; then
    sed 's/^/    /' "$TMP_ERR"
  else
    echo "    (empty)"
  fi

  echo "Exit status: $status"
  echo

  i=$((i + 1))
done

# 🧹 Cleanup
rm -f "$TMP_OUT" "$TMP_ERR"

