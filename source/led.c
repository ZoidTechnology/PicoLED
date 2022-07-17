#include "led.h"
#include "hardware/clocks.h"
#include "pico/time.h"

void led_init(PIO pio, uint sm)
{
	uint16_t instructions[] = {
		pio_encode_out(pio_x, 1) | pio_encode_sideset(1, 0) | pio_encode_delay(3),
		pio_encode_jmp_not_x(3) | pio_encode_sideset(1, 1) | pio_encode_delay(2),
		pio_encode_jmp(0) | pio_encode_sideset(1, 1) | pio_encode_delay(2),
		pio_encode_nop() | pio_encode_sideset(1, 0) | pio_encode_delay(2),
	};

	pio_program_t program = {
		.instructions = instructions,
		.length = sizeof(instructions) / sizeof(uint16_t),
		.origin = -1
	};

	uint offset = pio_add_program(pio, &program);
	pio_sm_config config = pio_get_default_sm_config();

	sm_config_set_wrap(&config, offset, offset + program.length - 1);
	sm_config_set_sideset(&config, 1, false, false);
	sm_config_set_out_shift(&config, false, true, 8);
	sm_config_set_fifo_join(&config, PIO_FIFO_JOIN_TX);
	sm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / 8000000.0f);

	pio_sm_init(pio, sm, offset, &config);
	pio_sm_set_enabled(pio, sm, true);
}

void led_write(PIO pio, uint sm, uint pin, uint8_t *data, uint length)
{
	pio_gpio_init(pio, pin);
	pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
	pio_sm_set_sideset_pins(pio, sm, pin);

	for (uint index = 0; index < length; index++)
	{
		pio_sm_put_blocking(pio, sm, data[index] << 24);
	}

	uint32_t mask = 1 << (PIO_FDEBUG_TXSTALL_LSB + sm);
	pio->fdebug = mask;

	while (!pio_sm_is_tx_fifo_empty(pio, sm) || !(pio->fdebug & mask));

	sleep_us(100);
}