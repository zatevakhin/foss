
add_subdirectory(base)
add_subdirectory(entities)
add_subdirectory(input)
add_subdirectory(renderers)
add_subdirectory(shading)
add_subdirectory(systems)

cmake_auxiliary_add_execultable(foss.app hpp cpp)
target_link_libraries(foss.app
        app_input
        app_systems
        app_renderers
        app_shading
        app_entities_windows
        app_entities_3d_objects

        base_shading
        base_resources
        base_textures
        base_renderers
        base_geometry
        base_input
        base_scene
        base_gears
        base_window
        base_math

        imgui

        GL
        SDL2
        SDL2_image
        boost_system
        boost_filesystem
        assimp
        fmt
    )
