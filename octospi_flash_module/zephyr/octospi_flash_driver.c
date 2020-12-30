/*
 *  Created on: Dec 30, 2020
 *      Author: Joao Carreira
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "../../octospi_flash_module/zephyr/octospi_flash_driver.h"

#include <zephyr/types.h>
#include <syscall_handler.h>

#include "mx25r6435f_driver.h"

/**
 * This is a minimal example of an out-of-tree driver
 * implementation. See the header file of the same name for details.
 */

static struct octospi_flash_dev_data {
	uint8_t state;
	QSPI_t data;
} data;


#define QUADSPI_CLK_Pin GPIO_PIN_10
#define QUADSPI_CLK_GPIO_Port GPIOE
#define QUADSPI_NCS_Pin GPIO_PIN_11
#define QUADSPI_NCS_GPIO_Port GPIOE
#define OQUADSPI_BK1_IO0_Pin GPIO_PIN_12
#define OQUADSPI_BK1_IO0_GPIO_Port GPIOE
#define QUADSPI_BK1_IO1_Pin GPIO_PIN_13
#define QUADSPI_BK1_IO1_GPIO_Port GPIOE
#define QUAD_SPI_BK1_IO2_Pin GPIO_PIN_14
#define QUAD_SPI_BK1_IO2_GPIO_Port GPIOE
#define QUAD_SPI_BK1_IO3_Pin GPIO_PIN_15
#define QUAD_SPI_BK1_IO3_GPIO_Port GPIOE


static int init(const struct device *dev)
{

	//TODO: Change Initialization to used Zephyr Pin Mux
	struct octospi_flash_dev_data *d = dev->data;
	QSPI_t* q = &d->data;

	// Initialize hardware
	q->qspi = OCTOSPI1;
	q->handle.Instance = q->qspi;

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN OCTOSPI1_MspInit 0 */

	/* USER CODE END OCTOSPI1_MspInit 0 */
	/* Peripheral clock enable */
	__HAL_RCC_OSPIM_CLK_ENABLE();
	__HAL_RCC_OSPI1_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	/**OCTOSPI1 GPIO Configuration
	PE10     ------> OCTOSPIM_P1_CLK
	PE11     ------> OCTOSPIM_P1_NCS
	PE12     ------> OCTOSPIM_P1_IO0
	PE13     ------> OCTOSPIM_P1_IO1
	PE14     ------> OCTOSPIM_P1_IO2
	PE15     ------> OCTOSPIM_P1_IO3
	*/
	GPIO_InitStruct.Pin = QUADSPI_CLK_Pin|QUADSPI_NCS_Pin|OQUADSPI_BK1_IO0_Pin|QUADSPI_BK1_IO1_Pin
						  |QUAD_SPI_BK1_IO2_Pin|QUAD_SPI_BK1_IO3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	// Initialize Chip Hardware
	BSP_QSPI_Init( q );

	return 0;
}

static void erase_sector_impl(const struct device *dev, uint32_t sector)
{
	struct octospi_flash_dev_data *d = dev->data;
	QSPI_t q = d->data;
	BSP_QSPI_Erase_Sector( &q, sector );
}

static void read_impl(const struct device *dev, uint32_t sector, uint8_t* buf, uint32_t length)
{
	struct octospi_flash_dev_data *d = dev->data;
	QSPI_t q = d->data;
	BSP_QSPI_Read(&q, buf, sector, length );
}

static void write_impl(const struct device *dev, uint32_t sector, const uint8_t* buf, uint32_t length)
{
	struct octospi_flash_dev_data *d = dev->data;
	QSPI_t q = d->data;
	BSP_QSPI_Write( &q, buf, sector, length );
}


#ifdef CONFIG_USERSPACE

static inline void z_vrfy_octospi_flash_erase_sector(const struct device *dev, uint32_t sector)
{
	Z_OOPS(Z_SYSCALL_DRIVER_OCTOSPI_FLASH_ERASE_SECTOR(dev, erase_sector));
	z_impl_octospi_flash_erase_sector(dev, sector);
}

static inline void z_vrfy_octospi_flash_read(const struct device *dev, uint32_t sector, uint8_t* buf, uint32_t length)
{
	Z_OOPS(Z_SYSCALL_DRIVER_OCTOSPI_FLASH_READ(dev, read));
	z_impl_octospi_flash_read(dev, sector, buf, length);
}

static inline void z_vrfy_octospi_flash_write(const struct device *dev, uint32_t sector, const uint8_t* buf, uint32_t length)
{
	Z_OOPS(Z_SYSCALL_DRIVER_OCTOSPI_FLASH_WRITE(dev, write));
	z_impl_octospi_flash_write(dev, sector, buf, length);
}


#include <syscalls/hello_world_print_mrsh.c>
#endif /* CONFIG_USERSPACE */



DEVICE_DEFINE(octospi_flash, "OCTOSPI_FLASH_DRIVER",
		    init, device_pm_control_nop, &data, NULL,
		    PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEVICE,
		    &((struct octospi_flash_driver_api){
				.erase_sector = erase_sector_impl,
				.read 		  = read_impl,
				.write 		  = write_impl
			}));
