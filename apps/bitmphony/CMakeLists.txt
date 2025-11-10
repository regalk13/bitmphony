add_executable(bitmphony
    main.cpp
)

target_link_libraries(bitmphony
    PRIVATE
        chip_core
        $<IF:$<BOOL:${GME_FOUND}>,PkgConfig::GME,gme>
)

set_target_properties(bitmphony PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)