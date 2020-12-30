/*
 *  Created on: Dec 30, 2020
 *      Author: Joao Carreira
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#ifndef __HELLO_WORLD_DRIVER_H__
#define __HELLO_WORLD_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <device.h>


__subsystem struct octospi_flash_driver_api {

	void (*erase_sector)(const struct device *dev, uint32_t sector );
	void (*read)(const struct device *dev, uint32_t sector, uint8_t* buf, uint32_t length );
	void (*write)(const struct device *dev, uint32_t sector, const uint8_t* buf, uint32_t length );

};

__syscall     void        octospi_flash_erase_sector(const struct device *dev, uint32_t sector);
static inline void z_impl_octospi_flash_erase_sector(const struct device *dev, uint32_t sector)
{
	const struct octospi_flash_driver_api *api = dev->api;
	api->erase_sector(dev, sector);
}

__syscall     void        octospi_flash_read(const struct device *dev, uint32_t sector, uint8_t* buf, uint32_t length);
static inline void z_impl_octospi_flash_read(const struct device *dev, uint32_t sector, uint8_t* buf, uint32_t length)
{
	const struct octospi_flash_driver_api *api = dev->api;
	api->read(dev, sector, buf, length );
}

__syscall     void        octospi_flash_write(const struct device *dev, uint32_t sector, uint8_t* buf, uint32_t length);
static inline void z_impl_octospi_flash_write(const struct device *dev, uint32_t sector, uint8_t* buf, uint32_t length)
{
	const struct octospi_flash_driver_api *api = dev->api;
	api->write(dev, sector, buf, length );
}



#ifdef __cplusplus
}
#endif

#include <syscalls/octospi_flash_driver.h>


#endif /* __HELLO_WORLD_DRIVER_H__ */
