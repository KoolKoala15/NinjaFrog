cmake_minimum_required(VERSION 3.23)
project(NinjaFrog LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# Download and add SFML as a dependency
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Download and add tmxlite (tiled editor loader) as a dependency
FetchContent_Declare(
    TMXLITE
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG v1.4.4
    SOURCE_SUBDIR tmxlite)
FetchContent_MakeAvailable(TMXLITE)


set(PROJECT_HEADERS 
    include/Utils/Constants.h

    include/Core/AssetManager.h
    include/Core/Game.h
    include/Core/World.h
    include/Core/Level.h
    include/Core/ConfigLoader.h
    include/Core/EnemyManager.h
    include/Core/CollectableManager.h
    include/Core/AudioManager.h

    include/Gameplay/GameObject.h
    include/Gameplay/Entity.h
    include/Gameplay/Player.h
    include/Gameplay/Enemy.h
    include/Gameplay/Collectable.h
    include/Gameplay/Saw.h
    include/Gameplay/Spike.h
    include/Gameplay/Fire.h

    include/Render/SFMLOrthogonalLayer.h

    include/UI/UIButton.h
    include/UI/UIHud.h
    include/UI/UILevelCompleted.h
    include/UI/UILevelSelector.h
    include/UI/UIManager.h
    include/UI/UIMenu.h;
    include/UI/UIPauseScreen.h;
    include/UI/UIScreenMainMenu.h
    include/UI/UIScreeen.h
)

set(PROJECT_SOURCES 

    src/Core/AssetManager.cpp
    src/Core/Game.cpp
    src/Core/World.cpp
    src/Core/Level.cpp
    src/Core/ConfigLoader.cpp
    src/Core/EnemyManager.cpp
    src/Core/CollectableManager.cpp
    src/Core/AudioManager.cpp
    
    src/Gameplay/Entity.cpp
    src/Gameplay/Player.cpp
    src/Gameplay/Enemy.cpp
    src/Gameplay/Collectable.cpp
    src/Gameplay/Saw.cpp
    src/Gameplay/Fire.cpp
    src/Gameplay/Spike.cpp

    src/UI/UIButton.cpp
    src/UI/UIHud.cpp
    src/UI/UILevelCompleted.cpp
    src/UI/UILevelSelector.cpp
    src/UI/UIManager.cpp
    src/UI/UIMenu.cpp
    src/UI/UIPauseScreen.cpp
    src/UI/UIScreenMainMenu.cpp

    src/main.cpp
)

set(PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES}
)

include_directories(${PROJECT_NAME} PRIVATE ${tmxlite_INCLUDE_DIR} include)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio tmxlite)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG_MODE=0)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()
