/*
Copyright 2023 Brandon Lu

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "quantum.h"

void sal_kb_led_out_default(void)
{
	/* LED 1 */
	/* This pin is set Low to turn the SAL "eye" off. */
	writePinLow(B5);
	/* LED 2 */
	writePinHigh(B4);
	/* LED 3 */
	writePinHigh(E6);
	/* LED 4 */
	writePinHigh(D1);
	/* LED 5 */
	writePinHigh(D2);
	/* LED 6 */
	writePinHigh(D3);
}

void keyboard_pre_init_kb(void)
{
	/* LED 1 */
	setPinOutput(B5);
	/* LED 2 */
	setPinOutput(B4);
	/* LED 3 */
	setPinOutput(E6);
	/* LED 4 */
	setPinOutput(D1);
	/* LED 5 */
	setPinOutput(D2);
	/* LED 6 */
	setPinOutput(D3);

	sal_kb_led_out_default();

	keyboard_pre_init_user();
}

void sal_kb_led_out_vec(uint8_t vec)
{
	/* This function accepts a bit vector in vec and outputs it to LEDs. */
	uint8_t bsel = 1;
	/* LED 1 */
	/* This pin is set Low to turn the SAL "eye" off. */
	writePin(B5, vec & bsel);
	/* LED 2 */
	bsel <<= 1;
	writePin(B4, !(vec & bsel));
	/* LED 3 */
	bsel <<= 1;
	writePin(E6, !(vec & bsel));
	/* LED 4 */
	bsel <<= 1;
	writePin(D1, !(vec & bsel));
	/* LED 5 */
	bsel <<= 1;
	writePin(D2, !(vec & bsel));
	/* LED 6 */
	bsel <<= 1;
	writePin(D3, !(vec & bsel));
}


void sal_kb_led_reset_vec(uint8_t idx)
{
	switch (idx)
	{
	/* LED 1 */
	case 0:
		writePinLow(B5);
		break;
	/* LED 2 */
	case 1:
		writePinHigh(B4);
		break;
	/* LED 3 */
	case 2:
		writePinHigh(E6);
		break;
	/* LED 4 */
	case 3:
		writePinHigh(D1);
		break;
	/* LED 5 */
	case 4:
		writePinHigh(D2);
		break;
	/* LED 6 */
	case 5:
		writePinHigh(D3);
		break;
	default: 
		/* do nothing, no LED addressed */
		break;
	}
}

void sal_kb_led_set_vec(uint8_t idx)
{
	switch (idx)
	{
	/* LED 1 */
	case 0:
		writePinHigh(B5);
		break;
	/* LED 2 */
	case 1:
		writePinLow(B4);
		break;
	/* LED 3 */
	case 2:
		writePinLow(E6);
		break;
	/* LED 4 */
	case 3:
		writePinLow(D1);
		break;
	/* LED 5 */
	case 4:
		writePinLow(D2);
		break;
	/* LED 6 */
	case 5:
		writePinLow(D3);
		break;
	default: 
		/* do nothing, no LED addressed */
		break;
	}
}

void sal_kb_led_toggle_vec(uint8_t idx)
{
	switch (idx)
	{
	/* LED 1 */
	case 0:
		togglePin(B5);
		break;
	/* LED 2 */
	case 1:
		togglePin(B4);
		break;
	/* LED 3 */
	case 2:
		togglePin(E6);
		break;
	/* LED 4 */
	case 3:
		togglePin(D1);
		break;
	/* LED 5 */
	case 4:
		togglePin(D2);
		break;
	/* LED 6 */
	case 5:
		togglePin(D3);
		break;
	default: 
		/* do nothing, no LED addressed */
		break;
	}
}

uint32_t sal_kb_led_timeout(uint32_t trigger_time, void *cb_arg)
{
	sal_kb_led_out_default();

	/* Set to 60 seconds to keep the token alive in the system. */
	return 60000;
}

void raw_hid_receive(uint8_t *data, uint8_t length)
{
	static deferred_token token = INVALID_DEFERRED_TOKEN;
	uint8_t sum = 0;

	/* Checksum */
	/* - Sum Data */
	sum = data[0];
	sum += data[1];
	/* - two's completement */
	sum = ~sum;
	sum += 1;
	/* - check sum, return if incorrect */
	if (data[2] != sum)
		return;

	switch(data[0])
	{
	/* Output Data */
	case 1:
		sal_kb_led_out_vec(data[1]);
		break;
	case 2:
		sal_kb_led_reset_vec(data[1]);
		break;
	case 3:
		sal_kb_led_set_vec(data[1]);
		break;
	case 4:
		sal_kb_led_toggle_vec(data[1]);
		break;
	/* Return in case of an invalid command */
	default:
		return;
		break;
	}

	if (token == INVALID_DEFERRED_TOKEN)
	{
		token = defer_exec(3000, sal_kb_led_timeout, NULL);
	}
	else
	{
		extend_deferred_exec(token, 3000);
	}
}

