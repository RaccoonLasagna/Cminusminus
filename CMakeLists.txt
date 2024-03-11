cmake_minimum_required(VERSION 3.20.0)
project("Game")

add_executable(GameGod 
    "Gui.h"
    "Gui.cpp"
    "FundamentalSystem.cpp"
    "main.cpp"
)

set(BUILD_SHARED_LIBS FALSE CACHE INTERNAL "Use static linking for SFML")

add_subdirectory("lib/SFML")

target_link_libraries(GameGod PRIVATE sfml-graphics sfml-window sfml-system)

set_target_properties(GameGod PROPERTIES CXX_STANDARD 17)
set_target_properties(GameGod PROPERTIES CXX_STANDARD_REQUIRED TRUE)

# ถ้าหากว่าเกิด _setjump ให้กด shift+P แล้วทำการ select ของ cmake จากนั้นก็เลือกเวอร์ชั่นของ vscode อันบนสุด