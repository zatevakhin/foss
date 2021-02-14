
add_subdirectory(geometry)
add_subdirectory(resources)
add_subdirectory(entities)
add_subdirectory(input)
add_subdirectory(shading)
add_subdirectory(systems)
add_subdirectory(window)
add_subdirectory(textures)
add_subdirectory(scene)
add_subdirectory(gears)
add_subdirectory(math)


cmake_auxiliary_add_execultable(foss.app hpp cpp)
target_link_libraries(foss.app
        app_input
        app_systems
        app_window
        app_entities_windows
        app_resources
        app_geometry
        app_textures
        app_scene
        app_shading
        app_gears
        app_math

        imgui

        GL
        SDL2
        SDL2_image
        assimp
        fmt
    )
