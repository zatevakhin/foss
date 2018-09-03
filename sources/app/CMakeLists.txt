
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

        core_shading
        core_resources
        core_textures
        core_renderers
        core_geometry 
        core_input
        core_scene
        core_gears 
        core_window

        anax
        imgui 

        GL
        SDL2
        SDL2_image
        boost_system
        boost_filesystem
        assimp
        fmt
    )
