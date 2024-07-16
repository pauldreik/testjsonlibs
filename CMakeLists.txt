cmake_minimum_required(VERSION 3.29)

project(adhocjson LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt REQUIRED)

add_executable(main_fmt main_fmt.cpp)
target_compile_options(main_fmt PUBLIC -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable)
target_link_libraries(main_fmt PUBLIC fmt::fmt)

find_package(daw-json-link REQUIRED)

add_executable(main_daw main_daw.cpp)
target_compile_options(main_daw PUBLIC -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable)
target_link_libraries(main_daw PUBLIC daw::daw-json-link)

find_package(Boost REQUIRED)
add_executable(main_boost main_boostjson.cpp)
target_compile_options(main_boost PUBLIC -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable)
target_link_libraries(main_boost PUBLIC boost::boost)

find_package(glaze REQUIRED)
add_executable(main_glaze main_glaze.cpp)
target_compile_options(main_glaze PUBLIC -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable)
target_link_libraries(main_glaze PUBLIC glaze::glaze)



include(GNUInstallDirs)
install(TARGETS main_fmt
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
