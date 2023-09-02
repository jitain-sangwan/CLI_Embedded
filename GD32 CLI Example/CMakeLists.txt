cmake_minimum_required(VERSION 3.25)
project(01_GPIO_Running_LED C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(MDK-ARM)
include_directories(MDK-ARM/RTE)
include_directories(MDK-ARM/RTE/_GD32E230G_START)

add_executable(01_GPIO_Running_LED
        MDK-ARM/RTE/_GD32E230G_START/RTE_Components.h
        MDK-ARM/Variables.h
        cli.c
        cli.h
        cli_cmd.c
        console.c
        console.h
        fifo.c
        fifo.h
        gd32e23x_it.c
        gd32e23x_it.h
        gd32e23x_libopt.h
        main.c
        main.h
        relay.c
        relay.h
        systick.c
        systick.h
        uart_driver.c
        uart_driver.h
        variables.c
        variables.h)
