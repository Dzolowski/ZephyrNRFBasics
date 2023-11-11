cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(FirstEx)
zephyr_sources_ifdef(CONFIG_USE_PROJ_OVERLAY proj.overlay)

target_sources(app PRIVATE src/main.c)
