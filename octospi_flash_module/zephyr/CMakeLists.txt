# SPDX-License-Identifier: Apache-2.0

if(CONFIG_OCTOSPI_FLASH_DRIVER)
  # Add hello_world_driver.h to the set of global include paths.
  zephyr_include_directories(.)

  zephyr_library()
  zephyr_library_sources(
    mx25r6435f_driver.c
    octospi_flash_driver.c
    )
endif()
