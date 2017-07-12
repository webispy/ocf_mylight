#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "ocf_mylight.h"

static OCDeviceInfo deviceInfo =
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
		{
			return OC_EH_RESOURCE_NOT_FOUND;
		}
	}

	DBG("dev on_get");

	return OC_EH_OK;
}

static OCEntityHandlerResult on_put(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req _UNUSED_, char *uri _UNUSED_,
		void *user_data _UNUSED_)
{
	DBG("dev on_put");

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
	.put = on_put,
	.post = on_put,
	.del = on_del
};

const char *ocf_mylight_device_get_name()
{
	return deviceInfo.deviceName;
}

int ocf_mylight_device_set_name(const char *name)
{
	if (!name)
		return -1;

	free(deviceInfo.deviceName);
	deviceInfo.deviceName = strdup(name);

	OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME,
			deviceInfo.deviceName);

	return 0;
}

int ocf_mylight_device_init()
{
	OCStackResult ret;

	OCSetDefaultDeviceEntityHandler(ocf_mylight_dev_handler, &dev_ops);

	deviceInfo.deviceName = strdup("ARTIK-053-Light");
	deviceInfo.dataModelVersions = OCCreateOCStringLL("res.1.1.0,sh.1.1.0");
	deviceInfo.types = OCCreateOCStringLL("oic.d.light");

	ret = OCSetDeviceInfo(deviceInfo);
	if (ret != OC_STACK_OK)
	{
		DBG("Device Registration failed!");
		exit(EXIT_FAILURE);
	}

	return 0;
}
