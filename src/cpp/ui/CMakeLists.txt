target_sources(${TARGET_NAME} 
    PUBLIC
    "./core.h"
    "./main_menu_bar.h"
    "./main_menu_bar.cpp"

)

add_subdirectory("./base")
add_subdirectory("./sub_windows")