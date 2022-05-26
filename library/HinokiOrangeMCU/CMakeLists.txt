cmake_minimum_required(VERSION 3.0)

project(HOMCU)

add_library(HOMCU OBJECT
    hom/hom_debugger.c
    hom/hom_def.c
    hom/hom_init.c
    hom/hom_time.c
    hom/hom_routine.c
    hom/hom_ticker.c
    hom/hom_sp.c
    hom/hom_state_machine.c
    hom/hom_countdown.c
    hom/hom_button.c
    hom/hom_command.c
)

target_include_directories(HOMCU PUBLIC .)
