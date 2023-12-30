cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include_directories(include)

### widget source files
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/widgets/*.cpp)
message(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
message(SRC_FILES="${SRC_FILES}")

add_executable(CMakeSFMLProject 
    src/gameplay.cpp
    src/gui.cpp
    src/items.cpp
    src/main.cpp
    src/senses.cpp
    src/skills.cpp
    src/start_state.cpp
    src/taskmanager.cpp
    src/title_screen.cpp
    ${SRC_FILES}
    )
target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics)
target_compile_features(CMakeSFMLProject PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET CMakeSFMLProject
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
        VERBATIM)
endif()

install(TARGETS CMakeSFMLProject)
