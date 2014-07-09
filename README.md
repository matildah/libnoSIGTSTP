calls signal(SIGTSTP, SIG_IGN) before main is called so control-Z (^Z) won't
suspend the program. To prevent the program from doing something like
signal(SIGTSTP, SIG_DFL) during its execution, we also hook signal and
sigaction;

Example:
LD_PRELOAD=nosigtstp.so irssi
