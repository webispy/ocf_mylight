#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "ocf_mylight.h"

static OCDeviceInfo _di =
{
	.specVersion = "core.1.1.0"
};

static OCEntityHandlerResult on_get(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req _UNUSED_, char *uri _UNUSED_,
		void *user_data _UNUSED_)
{
	if (uri)
	{
		if (!strcmp(uri, "/oic/invalid"))
			return OC_EH_RESOURCE_NOT_FOUND;
	}

	DBG("dev on_get");

	/* TODO */

	return OC_EH_OK;
}

static OCEntityHandlerResult on_put_post(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req _UNUSED_, char *uri _UNUSED_,
		void *user_data _UNUSED_)
{
	if (req->method == OC_REST_PUT)
		DBG("dev on_put");
	else if (req->method == OC_REST_POST)
		DBG("dev on_ppost");

	return OC_EH_FORBIDDEN;
}

static OCEntityHandlerResult on_del(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req _UNUSED_, char *uri _UNUSED_,
		void *user_data _UNUSED_)
{
	DBG("dev on_del");

	return OC_EH_FORBIDDEN;
}

struct ocf_dev_ops dev_ops =
{
	.get = on_get,
	.put = on_put_post,
	.post = on_put_post,
	.del = on_del
};

const char *ocf_mylight_device_get_name()
{
	return _di.deviceName;
}

int ocf_mylight_device_set_name(const char *name)
{
	if (!name)
		return -1;

	if (_di.deviceName)
		free(_di.deviceName);

	_di.deviceName = strdup(name);

	OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME,
			_di.deviceName);

	return 0;
}

int ocf_mylight_device_init()
{
	OCStackResult ret;

	OCSetDefaultDeviceEntityHandler(ocf_mylight_dev_handler, &dev_ops);

	_di.deviceName = strdup("ARTIK-053-Light");
	_di.dataModelVersions = OCCreateOCStringLL("res.1.1.0,sh.1.1.0");
	_di.types = OCCreateOCStringLL("oic.d.light");

	ret = OCSetDeviceInfo(_di);
	if (ret != OC_STACK_OK)
	{
		DBG("Device Registration failed! (ret=%d)", ret);
		return -1;
	}

	return 0;
}
