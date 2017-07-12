#ifndef __MYOCF_H__
#define __MYOCF_H__

#ifndef CONFIG_ARCH_BOARD_ARTIK053
#include "iotivity_config.h"
#endif

#include "ocstack.h"
#include "logger.h"
#include "platform_features.h"
#include "ocpayload.h"

#define _UNUSED_ __attribute__((unused))

#define DBG(fmt,args...) fprintf(stdout, "\e[1;32m<%s:%d> " fmt "\e[0m\n", __FILE__, __LINE__, ##args)
#define MSG(fmt,args...) fprintf(stdout, "\e[1;34m" fmt "\e[0m\n", ##args)

struct ocf_ops
{
	OCEntityHandler get;
	OCEntityHandler put;
	OCEntityHandler post;
	OCEntityHandler del;
	OCEntityHandler register_observe;
	OCEntityHandler deregister_observe;
};

struct ocf_dev_ops
{
	OCDeviceEntityHandler get;
	OCDeviceEntityHandler put;
	OCDeviceEntityHandler post;
	OCDeviceEntityHandler del;
};

OCEntityHandlerResult ocf_mylight_handler(OCEntityHandlerFlag flag,
		OCEntityHandlerRequest *req, void *user_data);
OCEntityHandlerResult ocf_mylight_dev_handler(OCEntityHandlerFlag flag,
		OCEntityHandlerRequest *req, char *uri, void *user_data);

void ocf_mylight_verbose_payload(const char *indent, OCPayload *payload);
void ocf_mylight_verbose_response(OCEntityHandlerResponse *resp);
void ocf_mylight_verbose_request(OCEntityHandlerFlag flag,
		OCEntityHandlerRequest *req);

int ocf_mylight_playform_init();

const char *ocf_mylight_device_get_name();
int ocf_mylight_device_set_name(const char *name);
int ocf_mylight_device_init();

int ocf_mylight_light_init();
void ocf_mylight_light_exit();

int ocf_mylight_configuration_init();

#endif
