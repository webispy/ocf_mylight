#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "ocf_mylight.h"
#include <pinoxmcommon.h>

static FILE* on_open(const char *path, const char *mode)
{
	DBG("open: path=%s, mode=%s", path, mode);

	return fopen(path, mode);
}

static size_t on_read(void *ptr, size_t size, size_t nmemb, FILE *fp)
{
	DBG("read: size=%zd, nmemb=%zd, fp=0x%p", size, nmemb, fp);

	return fread(ptr, size, nmemb, fp);
}

static size_t on_write(const void *ptr, size_t size, size_t nmemb, FILE *fp)
{
	DBG("write: size=%zd, nmemb=%zd, fp=0x%p", size, nmemb, fp);

	return fwrite(ptr, size, nmemb, fp);
}

static int on_close(FILE *fp)
{
	DBG("close: fp=0x%p", fp);

	return fclose(fp);
}

static int on_unlink(const char *path)
{
	DBG("unlink: path=%s", path);

	return unlink(path);
}

static void on_display_pin(char *pin, size_t length, void *user_data _UNUSED_)
{
	MSG("PIN CODE: '%s' (pin length=%zd)", pin, length);
}

static void on_close_pin(void)
{
	MSG("PIN CLOSED");
}

OCPersistentStorage ps = {
	.open = on_open,
	.read = on_read,
	.write = on_write,
	.close = on_close,
	.unlink = on_unlink
};

int ocf_mylight_security_init()
{
	OCStackResult ret;

	OCRegisterPersistentStorageHandler(&ps);

	ret = SetDisplayPinWithContextCB(on_display_pin, NULL);
	if (ret != OC_STACK_OK) {
		DBG("SetDisplayPinWithContextCB failed! (ret=%d)", ret);
		return -1;
	}

	SetClosePinDisplayCB(on_close_pin);

	ret = SetRandomPinPolicy(8, NUM_PIN);
	if (ret != OC_STACK_OK) {
		DBG("SetRandomPinPolicy failed! (ret=%d)", ret);
		return -1;
	}

	return 0;
}
