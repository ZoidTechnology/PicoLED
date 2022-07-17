#include "tusb.h"
#include "led.h"

uint8_t buffer[4096];

int main()
{
	uint8_t pin;
	uint16_t length = 0;
	uint16_t index;

	tusb_init();
	led_init(pio0, 0);

	while (true)
	{
		tud_task();

		if (!tud_connected())
		{
			continue;
		}

		if (tud_cdc_get_line_state() & 0x01)
		{
			length = 0;
			tud_cdc_read_flush();
		}

		if (length)
		{
			index += tud_cdc_read(buffer + index, length - index);

			if (index == length)
			{
				led_write(pio0, 0, pin, buffer, length);

				tud_cdc_write_char(pin);
				tud_cdc_write_flush();

				length = 0;
			}
		}
		else
		{
			if (tud_cdc_available() > 1)
			{
				uint8_t header[2];

				tud_cdc_read(header, sizeof(header));

				pin = header[0] >> 4;
				length = (header[0] & 0x0F) << 8 | header[1];
				index = 0;
			}
		}
	}
}