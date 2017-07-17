#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "ocf_mylight.h"

static int gQuitFlag = 0;

static int server_cb(int argc _UNUSED_, char *argv[] _UNUSED_)
{
	MSG("IoTivity Demo - IOTIVITY version is %s", IOTIVITY_VERSION);
	MSG("OCServer is starting...");

	if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK)
	{
		DBG("OCStack init error");
		return 0;
	}

	ocf_mylight_playform_init();
	ocf_mylight_device_init();
	ocf_mylight_configuration_init();
	ocf_mylight_maintenance_init();
	ocf_mylight_light_init();

	// Break from loop with Ctrl-C
	MSG("Entering ocserver main loop...");

	while (!gQuitFlag)
	{
		usleep(10000);
		if (OCProcess() != OC_STACK_OK)
		{
			DBG("OCStack process error");
			return 0;
		}
	}

	ocf_mylight_light_exit();

	MSG("Exiting ocserver main loop...");

	if (OCStop() != OC_STACK_OK)
		DBG("OCStack process error");

	return 0;
}

static void _timestamp()
{
	struct tm stamp;
	struct timespec tspec;
	char timebuf[32];

	clock_gettime(CLOCK_REALTIME, &tspec);
	localtime_r(&tspec.tv_sec, &stamp);

	strftime(timebuf, sizeof(timebuf), "%m-%d %H:%M:%S", &stamp);
	printf("%s.%03ld", timebuf, tspec.tv_nsec / 1000000);
}

OCEntityHandlerResult ocf_mylight_handler(OCEntityHandlerFlag flag,
		OCEntityHandlerRequest *req, void *user_data)
{
	struct ocf_ops *ops = user_data;
	OCEntityHandlerResult ret = OC_EH_METHOD_NOT_ALLOWED;

	printf("\n");
	_timestamp();
	MSG("\n<New request>");
	ocf_mylight_verbose_request(flag, req);

	if (!ops)
		return ret;

	if (flag & OC_REQUEST_FLAG)
	{
		if (OC_REST_GET == req->method && ops->get)
			ret = ops->get(flag, req, user_data);
		else if (OC_REST_PUT == req->method && ops->put)
			ret = ops->put(flag, req, user_data);
		else if (OC_REST_POST == req->method && ops->post)
			ret = ops->post(flag, req, user_data);
		else if (OC_REST_DELETE == req->method && ops->del)
			ret = ops->del(flag, req, user_data);
	}

	if (flag & OC_OBSERVE_FLAG)
	{
		if (OC_OBSERVE_REGISTER == req->obsInfo.action && ops->register_observe)
			ret = ops->register_observe(flag, req, user_data);
		else if (OC_OBSERVE_DEREGISTER == req->obsInfo.action
				&& ops->deregister_observe)
			ret = ops->deregister_observe(flag, req, user_data);
	}

	MSG("  return code: %d", ret);
	MSG("</New request>\n");

	return ret;
}

OCEntityHandlerResult ocf_mylight_dev_handler(OCEntityHandlerFlag flag,
		OCEntityHandlerRequest *req, char *uri, void *user_data)
{
	struct ocf_dev_ops *ops = user_data;
	OCEntityHandlerResult ret = OC_EH_METHOD_NOT_ALLOWED;

	printf("\n");
	_timestamp();
	MSG("\n<New request for device>");
	ocf_mylight_verbose_request(flag, req);
	MSG("  - uri: %s", uri);

	if (!ops)
		return ret;

	if (flag & OC_REQUEST_FLAG)
	{
		if (OC_REST_GET == req->method && ops->get)
			ret = ops->get(flag, req, uri, user_data);
		else if (OC_REST_PUT == req->method && ops->put)
			ret = ops->put(flag, req, uri, user_data);
		else if (OC_REST_DELETE == req->method && ops->del)
			ret = ops->del(flag, req, uri, user_data);
	}

	MSG("  return code: %d", ret);
	MSG("</New request for device>\n");

	return ret;
}

int ocf_mylight_get_quit_flag()
{
	return gQuitFlag;
}

#ifndef CONFIG_ARCH_BOARD_ARTIK053
int main(int argc, char *argv[])
{
	server_cb(argc, argv);

	return 0;
}
#else
#define IOTIVITY_TEST_STACKSIZE   4096
#define IOTIVITY_TEST_PRI   100
#define IOTIVITY_TEST_SCHED_POLICIY SCHED_RR

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, char *argv[])
#else
int ocf_mylight_main(int argc, char *argv[])
#endif
{
	task_create("ocf_mylight_server", IOTIVITY_TEST_PRI,
			IOTIVITY_TEST_STACKSIZE, server_cb,
			(FAR char * const *)NULL);
	return 0;
}
#endif
