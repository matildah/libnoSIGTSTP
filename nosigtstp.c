/* LD_PRELOAD shim that does its best to ensure control-Z (^Z) doesn't work
   to suspend the process */

/*
 * Copyright (c) 2014 Kia
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE
 * USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <signal.h>
#define __USE_GNU
#include <dlfcn.h>

static void no_tstp(void) __attribute__((constructor));
void (*signal(int sig, void (*func)(int))) (int);
int sigaction(int sig, const struct sigaction *act, struct sigaction *oact);



static void
no_tstp(void) {
    signal(SIGTSTP, SIG_IGN);
}

void
(*signal(int sig, void (*func)(int))) (int) {
    void (*(*origsig)(int, void (*)(int))) (int);

    origsig = dlsym(RTLD_NEXT, "signal");

    if (sig == SIGTSTP) {
        return (*origsig)(sig, SIG_IGN);
    }

    return (*origsig)(sig, func);
}


int
sigaction(int sig, const struct sigaction *act, struct sigaction *oact) {
    int (*origaction) (int, const struct sigaction *, struct sigaction *);

    origaction = dlsym(RTLD_NEXT, "sigaction");

    if (sig == SIGTSTP) {
        return (*origaction)(sig, NULL, oact);
    }

    return (*origaction)(sig, act, oact);
}

