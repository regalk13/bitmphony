add_library(chip_core STATIC
    src/AudioSystem.cpp
)

target_include_directories(chip_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(chip_core
    PUBLIC
        SDL2::SDL2
)
