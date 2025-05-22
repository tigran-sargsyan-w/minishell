#!/bin/sh

# List of all standard bash-biltins, except echo, cd, pwd, env, export, unset, and exit:
builtins=". source [ alias bg bind break builtin case command compgen \
complete continue declare dirs disown enable eval exec fc fg getopts \
hash help history jobs kill let local logout popd printf pushd read \
readonly return set shift shopt suspend test times trap type typeset \
ulimit umask unalias wait"

# Temporary files for capturing output
TMP_OUT=$(mktemp)
TMP_ERR=$(mktemp)

for b in $builtins; do
  echo "=== Testing '$b' ==="

# Run the builtin and capture the return code
  status=$(
    printf "%s\nexit\n" "$b" \
      | ./minishell >"$TMP_OUT" 2>"$TMP_ERR"
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
done

# Remove temporary files
rm -f "$TMP_OUT" "$TMP_ERR"
