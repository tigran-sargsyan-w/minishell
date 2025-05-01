#include "builtins.h"
#include "stddef.h"

int run_builtin(int argc, char **argv, t_env_list *env)
{
    static const t_builtins_array builtins_arr[] = {
        { "echo",   builtin_echo   },
        { "cd",     builtin_cd     },
        { "pwd",    builtin_pwd    },
        { "env",    builtin_env    },
        { "export", builtin_export },
        { "unset",  builtin_unset  },
        { "exit",   builtin_exit   },
        { NULL,     NULL           }
    };
}