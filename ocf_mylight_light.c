#ifdef __TIZENRT__
#include <tinyara/config.h>
#include <tinyara/gpio.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "ocf_mylight.h"

struct light_resource {
	OCResourceHandle handle;
	bool value;
	char *uri;
	int gpio;
};

static struct light_resource _light[] = {
	{
		.handle = NULL,
		.value = false,
		.uri = "/a/light/0",
		.gpio = 45
	}, {
		.handle = NULL,
		.value = false,
		.uri = "/a/light/1",
		.gpio = 49
	}
};

static void gpio_write(int port, int value)
{
#ifdef CONFIG_ARCH_BOARD_ARTIK053
	char tmp[20];
	int fd;

	snprintf(tmp, 16, "/dev/gpio%d", port);
	fd = open(tmp, O_RDWR);
	if (fd < 0)
	{
		DBG("open(%s) failed.", tmp);
		return;
	}

	MSG("GPIO: '%s' value '%d'", tmp, value);

	ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);

	snprintf(tmp, 2, "%1d", value);
	write(fd, tmp, 2);

	close(fd);
#else
	MSG("GPIO: '%d' value '%d'", port, value);
#endif
}

static int find_light(OCResourceHandle handle)
{
	unsigned int i;

	for (i = 0; i < sizeof(_light) / sizeof(struct light_resource); i++) {
		if (_light[i].handle == handle)
			return i;
	}

	DBG("Can't find light");

	return -1;
}

static OCEntityHandlerResult on_get(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req, void *user_data _UNUSED_)
{
	OCEntityHandlerResponse resp;
	OCRepPayload *payload = NULL;
	int id;

	MSG("received GET request");

	id = find_light(req->resource);
	if (id < 0)
		return OC_EH_ERROR;

	payload = OCRepPayloadCreate();
	OCRepPayloadAddResourceType(payload, "oic.r.switch.binary");
	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.a");

	OCRepPayloadSetUri(payload, _light[id].uri);
	OCRepPayloadSetPropBool(payload, "value", _light[id].value);

	memset(&resp, 0, sizeof(OCEntityHandlerResponse));
	resp.requestHandle = req->requestHandle;
	resp.resourceHandle = req->resource;
	resp.ehResult = OC_EH_OK;
	resp.payload = (OCPayload*) payload;

	ocf_mylight_verbose_response(&resp);

	if (OCDoResponse(&resp) != OC_STACK_OK) {
		DBG("Error sending response");
		OCRepPayloadDestroy(payload);
		return OC_EH_ERROR;
	}

	OCRepPayloadDestroy(payload);

	return OC_EH_OK;
}

static OCEntityHandlerResult on_put_post(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req, void *user_data _UNUSED_)
{
	OCEntityHandlerResponse resp;
	OCRepPayload *payload = NULL;
	int id;
	bool value;

	if (req->method == OC_REST_PUT)
		MSG("received PUT request");
	else if (req->method == OC_REST_POST)
		MSG("received POST request");

	id = find_light(req->resource);
	if (id < 0)
		return OC_EH_ERROR;

	payload = (OCRepPayload *) req->payload;
	if (OCRepPayloadGetPropBool(payload, "value", &value)) {
		_light[id].value = value;
		MSG("value changed to %d", value);
	}

	gpio_write(_light[id].gpio, _light[id].value);

	payload = OCRepPayloadCreate();
	OCRepPayloadAddResourceType(payload, "oic.r.switch.binary");
	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.a");

	OCRepPayloadSetUri(payload, _light[id].uri);
	OCRepPayloadSetPropBool(payload, "value", _light[id].value);

	memset(&resp, 0, sizeof(OCEntityHandlerResponse));
	resp.requestHandle = req->requestHandle;
	resp.resourceHandle = req->resource;
	resp.ehResult = OC_EH_OK;
	resp.payload = (OCPayload*) payload;

	ocf_mylight_verbose_response(&resp);

	if (OCDoResponse(&resp) != OC_STACK_OK) {
		DBG("Sending response failed.");
		OCRepPayloadDestroy(payload);
		return OC_EH_ERROR;
	}

	OCRepPayloadDestroy(payload);

	return OC_EH_OK;
}

static OCEntityHandlerResult on_del(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req _UNUSED_, void *user_data _UNUSED_)
{
	DBG("Forbidden");

	return OC_EH_FORBIDDEN;
}

static OCEntityHandlerResult on_register_observe(
		OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req _UNUSED_, void *user_data _UNUSED_)
{
	DBG("Registration request with observation Id %d", req->obsInfo.obsId);

	/* TODO */

	return OC_EH_FORBIDDEN;
}

static OCEntityHandlerResult on_deregister_observe(
		OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req _UNUSED_, void *user_data _UNUSED_)
{
	DBG("De-registration request for observation Id %d",
			req->obsInfo.obsId);

	/* TODO */

	return OC_EH_FORBIDDEN;
}

static struct ocf_ops light_ops = {
	.get = on_get,
	.put = on_put_post,
	.post = on_put_post,
	.del = on_del,
	.register_observe = on_register_observe,
	.deregister_observe = on_deregister_observe
};

int ocf_mylight_light_init()
{
	OCStackResult ret;
	unsigned int i;

	for (i = 0; i < sizeof(_light) / sizeof(struct light_resource); i++) {
		ret = OCCreateResource(&(_light[i].handle),
				"oic.r.switch.binary", "oic.if.a",
				_light[i].uri, ocf_mylight_handler, &light_ops,
				OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
		if (ret != OC_STACK_OK) {
			DBG("OCCreateResource() failed. (ret=%d)", ret);
			return -1;
		}

		MSG("Light resource created. <id: %u>", i);
		MSG(" - resource: 0x%p", _light[i].handle);
		MSG(" - uri: '%s'", _light[i].uri);
	}

	return 0;
}

void ocf_mylight_light_exit()
{
}
