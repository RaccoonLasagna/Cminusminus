project("Game")

add_executable(GameGod "FundamentalSystem.cpp")

add_executable(GameGod "main.cpp")


set(BUILD_SHARED_LIBS FALSE CACHE INTERNAL "Use static linking for SFML")

add_subdirectory("lib/SFML")

target_link_libraries(GameGod sfml-graphics sfml-window sfml-system)

set_property(TARGET GameGod PROPERTY CXX_STANDARD 17)