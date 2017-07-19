#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>

#include "ocf_mylight.h"

struct conf_data {
	char *name;
	char *loc;
	char *locn;
	char *currency;
	char *region;
};

static struct conf_data _cd;
static OCResourceHandle _confhandle;

static OCEntityHandlerResult on_get(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req, void *user_data _UNUSED_)
{
	OCEntityHandlerResponse resp;
	OCRepPayload *payload = NULL;

	payload = OCRepPayloadCreate();
	OCRepPayloadAddResourceType(payload, "oic.wk.con");
	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.rw");

	OCRepPayloadSetPropString(payload, "n", _cd.name);
	OCRepPayloadSetPropString(payload, "loc", _cd.loc);
	OCRepPayloadSetPropString(payload, "locn", _cd.locn);
	OCRepPayloadSetPropString(payload, "c", _cd.currency);
	OCRepPayloadSetPropString(payload, "r", _cd.region);

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

static OCEntityHandlerResult on_post(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req, void *user_data _UNUSED_)
{
	OCEntityHandlerResponse resp;
	OCRepPayload *payload = NULL;
	OCRepPayload *input = (OCRepPayload *) req->payload;
	char *val = NULL;

	if (OCRepPayloadGetPropString(input, "n", &val)) {
		free(_cd.name);
		_cd.name = val;

		ocf_mylight_device_set_name(_cd.name);
	}

	if (OCRepPayloadGetPropString(input, "loc", &val)) {
		free(_cd.loc);
		_cd.loc = val;
	}

	if (OCRepPayloadGetPropString(input, "locn", &val)) {
		free(_cd.locn);
		_cd.locn = val;
	}

	if (OCRepPayloadGetPropString(input, "c", &val)) {
		free(_cd.currency);
		_cd.currency = val;
	}

	if (OCRepPayloadGetPropString(input, "r", &val)) {
		free(_cd.region);
		_cd.region = val;
	}

	payload = OCRepPayloadCreate();
	OCRepPayloadAddResourceType(payload, "oic.wk.con");
	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.rw");

	OCRepPayloadSetPropString(payload, "n", _cd.name);
	OCRepPayloadSetPropString(payload, "loc", _cd.loc);
	OCRepPayloadSetPropString(payload, "locn", _cd.locn);
	OCRepPayloadSetPropString(payload, "c", _cd.currency);
	OCRepPayloadSetPropString(payload, "r", _cd.region);

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

static struct ocf_ops conf_ops =
{
	.get = on_get,
	.post = on_post
};

int ocf_mylight_configuration_init()
{
	OCStackResult ret;

	_cd.name = strdup(ocf_mylight_device_get_name());
	_cd.loc = strdup("37.532600,127.024612");
	_cd.locn = strdup("My home");
	_cd.region = strdup("Korea (Gyeonggi)");
	_cd.currency = strdup("KRW");

	ret = OCCreateResource(&_confhandle, "oic.wk.con", "oic.if.rw",
			"/oic/con", ocf_mylight_handler, &conf_ops,
			OC_DISCOVERABLE | OC_SECURE);
	if (ret != OC_STACK_OK) {
		DBG("OCCreateResource() failed. (ret=%d)", ret);
		return -1;
	}

	return 0;
}
