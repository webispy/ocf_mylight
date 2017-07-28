#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "ocf_mylight.h"

static OCResourceHandle _mnthandle;

static OCEntityHandlerResult on_get(OCEntityHandlerFlag flag _UNUSED_,
		OCEntityHandlerRequest *req, void *user_data _UNUSED_)
{
	OCEntityHandlerResponse resp;
	OCRepPayload *payload = NULL;

	payload = OCRepPayloadCreate();
	OCRepPayloadAddResourceType(payload, "oic.wk.mnt");
	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.rw");

	OCRepPayloadSetPropBool(payload, "fr", false);
	OCRepPayloadSetPropBool(payload, "rb", false);

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
	bool val = NULL;

	payload = OCRepPayloadCreate();
	OCRepPayloadAddResourceType(payload, "oic.wk.mnt");
	OCRepPayloadAddInterface(payload, "oic.if.baseline");
	OCRepPayloadAddInterface(payload, "oic.if.rw");

	if (OCRepPayloadGetPropBool(input, "fr", &val)) {
		OCRepPayloadSetPropBool(payload, "fr", true);
		/**
		 * Do Factory Reset
		 */
		MSG("Factory Reset request");
	} else {
		OCRepPayloadSetPropBool(payload, "fr", false);
	}

	if (OCRepPayloadGetPropBool(input, "rb", &val)) {
		OCRepPayloadSetPropBool(payload, "rb", true);
		/**
		 * Do Reboot
		 */
		MSG("Reboot request");
	} else {
		OCRepPayloadSetPropBool(payload, "rb", false);
	}

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

static struct ocf_ops mnt_ops = {
	.get = on_get,
	.post = on_post
};

int ocf_mylight_maintenance_init()
{
	OCStackResult ret;

	/**
	 * FIXME: mismatch Spec and CTT
	 *
	 * In the OIC 1.1.2 specification document, the 'oic.if.rw' interface is
	 * correct for the 'oic.wk.mnt' resource type.
	 * However, CTT 1.5.0.0 tools 'Check_13' test cases report FAIL results
	 * because the tool want the interface 'oic.if.r'.
	 */

	// ret = OCCreateResource(&_mnthandle, "oic.wk.mnt", "oic.if.rw",
	ret = OCCreateResource(&_mnthandle, "oic.wk.mnt", "oic.if.r",
			"/oic/mnt", ocf_mylight_handler, &mnt_ops,
			OC_DISCOVERABLE | OC_SECURE);
	if (ret != OC_STACK_OK) {
		DBG("OCCreateResource() failed. (ret=%d)", ret);
		return -1;
	}

	return 0;
}
