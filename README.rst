.. _blinky-sample:

Zephyr Demo driver for `Macromix` Nor Flash `mx25r6435f` and `stm32l4xx`, `stm32l4s5vitx`

Overview
********

This module is a working in progress for a driver of Macromix ``mx25r6435f`` present in the board ``STM32L4+ Discovery kit IoT`` ``B-L4S5I-IOT01A``
`<https://www.macronix.com/en-us/products/NOR-Flash/Serial-NOR-Flash/Pages/spec.aspx?p=MX25R6435F&m=Serial%20NOR%20Flash&n=PM2138>`
`<https://www.macronix.com/en-us/products/NOR-Flash/Serial-NOR-Flash/Pages/spec.aspx?p=MX25R6435F&m=Serial%20NOR%20Flash&n=PM2138>`

State
******
Wrapps STM Cube API in a custom Zephyr driver

Next Steps
**********
Implement this driver using flashspi API.


Usage
********

.. code-block:: none
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



.. image:: https://raw.githubusercontent.com/jptcarreira/zephyr-demo-wifi/master/zephyr-wifi-demo.png

Requirements
************

A Zephyr development environment configured, see:
`<https://docs.zephyrproject.org/latest/getting_started/index.html>`

A board ``STM32L4+ Discovery kit IoT`` ``B-L4S5I-IOT01A``.


Building and Running
********************

Build and flash Zephyr Image:

.. zephyr-app-commands::
   :zephyr-app: szephyr-demo-wifi
   :board: b_l4s5i_iot01a
   :goals: build flash
   :compact:

After flashing, you should be able to access the console and view the test results.


License
************
This repository is open-sourced under the Apache-2.0 license.
For a list of other open-source components included in this repository, see the file 3rd-party-licenses.txt.

