#include "tusb.h"

const tusb_desc_device_t device_descriptor = {
	.bLength = sizeof(device_descriptor),
	.bDescriptorType = TUSB_DESC_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = TUSB_CLASS_CDC,
	.bDeviceSubClass = 0x00,
	.bDeviceProtocol = 0x00,
	.bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
	.idVendor = 0x2E8A,
	.idProduct = 0x000A,
	.bcdDevice = 0x0000,
	.iManufacturer = 0x01,
	.iProduct = 0x02,
	.iSerialNumber = 0x00,
	.bNumConfigurations = 0x01
};

const uint8_t configuration_descriptors[] = {
	TUD_CONFIG_DESCRIPTOR(1, 2, 0, TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN, 0, 500),
	TUD_CDC_DESCRIPTOR(0, 0, 0x81, 64, 0x02, 0x82, 64)
};

const char *strings[] = {
	"Zoid Technology",
	"PicoLED"
};

const uint8_t *tud_descriptor_device_cb()
{
	return (uint8_t*) &device_descriptor;
}

const uint8_t *tud_descriptor_configuration_cb(uint8_t index)
{
	(void) index; // Avoid unused parameter warning

	return configuration_descriptors;
}

const uint16_t *tud_descriptor_string_cb(uint8_t index, uint16_t language)
{
	(void) language; // Avoid unused parameter warning

	static uint16_t buffer[32];
	uint8_t length;

	if (index == 0)
	{
		buffer[1] = 0x090C;
		length = 1;
	}
	else if (index <= sizeof(strings) / sizeof(strings[0]))
	{
		const char* string = strings[index - 1];

		length = strlen(string);

		for (uint8_t i = 0; i < length; i++)
		{
			buffer[i + 1] = string[i];
		}
	}
	else
	{
		return NULL;
	}

	buffer[0] = (TUSB_DESC_STRING << 8) | (2 * length + 2);

	return buffer;
}