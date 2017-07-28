#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

#include "ocf_mylight.h"

static void on_signal_1(int signo)
{
	bool status;

	MSG("SIGUSR1(%d) - toggle /a/light/0", signo);

	if (ocf_mylight_light_get_status(0, &status) == -1)
		return;

	ocf_mylight_light_set_status(0, !status);
}

static void on_signal_2(int signo)
{
	bool status;

	MSG("SIGUSR2(%d) - toggle /a/light/1", signo);

	if (ocf_mylight_light_get_status(1, &status) == -1)
		return;

	ocf_mylight_light_set_status(1, !status);
}

int ocf_mylight_userinput_init()
{
	struct sigaction usr1, usr2;

	usr1.sa_handler = on_signal_1;
	sigemptyset(&usr1.sa_mask);
	usr1.sa_flags = 0;

	usr2.sa_handler = on_signal_2;
	sigemptyset(&usr2.sa_mask);
	usr2.sa_flags = 0;

	if (sigaction(SIGUSR1, &usr1, 0) == -1) {
		DBG("sigaction failed. (errno=%d)", errno);
		return -1;
	}

	if (sigaction(SIGUSR2, &usr2, 0) == -1) {
		DBG("sigaction failed. (errno=%d)", errno);
		return -1;
	}

	MSG("Control /a/light/{0, 1}  resource by signal");
	MSG(" - 'kill -USR1 %d': toggle the /a/light/0", getpid());
	MSG(" - 'kill -USR2 %d': toggle the /a/light/1", getpid());

	return 0;
}
