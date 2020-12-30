/*
 *  Created on: Dec 30, 2020
 *      Author: Joao Carreira
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <zephyr.h>
#include "octospi_flash_driver.h"

const struct device *octospi_dev;

char DATA_WRITE[] = "Hello Octo-SPI from Zephyr...";

#define BUFFER_SIZE sizeof(DATA_WRITE)
char BUFFER[ BUFFER_SIZE ];


static void test_flash(void *p1, void *p2, void *p3)
{
	octospi_dev = device_get_binding("OCTOSPI_FLASH_DRIVER");

	// Erase the sector
	printk("Erase sector...\n" );
	octospi_flash_erase_sector(octospi_dev, 0 );

	//TODO: Driver not waiting for operation complete
	k_msleep(60);

	// Read to check if is empty
	octospi_flash_read(octospi_dev, 0, BUFFER, BUFFER_SIZE );
	for( int i=0; i < BUFFER_SIZE; ++i ) {
		if( BUFFER[i] != 0xFF ) {
			printk("Error, data not erased!\n");
		}
	}

	// Write data
	octospi_flash_write(octospi_dev, 0, DATA_WRITE, BUFFER_SIZE );

	// Read to check if data was writen.
	octospi_flash_read(octospi_dev, 0, BUFFER, BUFFER_SIZE );
	BUFFER[ BUFFER_SIZE ] = 0;

	for( int i=0; i < BUFFER_SIZE; ++i ) {
		if( BUFFER[i] != DATA_WRITE[i] ) {
			printk("Error, data written doesn't match!\n");
			return;
		}
	}

	printk("Test succeed!\n");

}

void main(void)
{
	k_thread_user_mode_enter(test_flash, NULL, NULL, NULL);
}
