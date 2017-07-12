#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "ocf_mylight.h"

static OCPlatformInfo platformInfo =
{
	.platformID = "0A3E0D6F-DBF5-404E-8719-D6880042463A",
	.manufacturerName = "GeekRedLed",
	.manufacturerUrl = "http://geekredled.com",
	.modelNumber = "ARTIK053",
	.dateOfManufacture = "2017-07-11",
	.platformVersion = "TINYARA_1.0",
	.operatingSystemVersion = "fakerOS",
	.hardwareVersion = "ARTIK_053",
	.firmwareVersion = "TEST_1.0",
	.supportUrl = "http://geekredled.com",
	.systemTime = "2017-07-11T16:06:00+09:00"
};

int ocf_mylight_playform_init()
{
	OCStackResult ret;

	ret = OCSetPlatformInfo(platformInfo);
	if (ret != OC_STACK_OK)
	{
		DBG("Platform Registration failed! (ret=%d)", ret);
		return -1;
	}

	return 0;
}
