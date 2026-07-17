cmake_minimum_required(VERSION 3.2)

project(RotMGPad)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

if (UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
endif()

set(PLATFORM_SOURCES "")
if (UNIX AND NOT APPLE)
    list(APPEND PLATFORM_SOURCES
            virtualinput/VirtualMouse_Linux.cpp
            virtualinput/VirtualKeyboard_Linux.cpp
    )
elseif(WIN32)
    list(APPEND PLATFORM_SOURCES
            virtualinput/VirtualMouse_Windows.cpp
            virtualinput/VirtualKeyboard_Windows.cpp
    )
else()
    message(FATAL_ERROR "RotMGPad only has virtual input backends for Linux and Windows.")
endif()

add_executable(RotMGPad
        main.cpp
        globals.hpp
        globals.cpp

        platform/PlatformSleep.hpp

        virtualinput/VirtualMouse.hpp
        virtualinput/VirtualKeyboard.hpp
        virtualinput/Keycodes.hpp
        ${PLATFORM_SOURCES}

        controls/ControllerManager.hpp
        controls/ControllerManager.cpp

        controls/modes/IInputMode.hpp
        controls/modes/InventoryMode.hpp
        controls/modes/InventoryMode.cpp
        controls/modes/PartyMode.hpp
        controls/modes/PartyMode.cpp
        controls/modes/NormalMode.hpp
        controls/modes/NormalMode.cpp
        controls/modes/TeleportMode.hpp
        controls/modes/TeleportMode.cpp
        controls/modes/PickupMode.hpp
        controls/modes/PickupMode.cpp
        controls/modes/VaultMode.hpp
        controls/modes/VaultMode.cpp
        controls/modes/PotionMode.hpp
        controls/modes/PotionMode.cpp
        controls/modes/EquipmentMode.hpp
        controls/modes/EquipmentMode.cpp
        controls/modes/MenuBarMode.hpp
        controls/modes/MenuBarMode.cpp
        controls/modes/PetInventoryMode.hpp
        controls/modes/PetInventoryMode.cpp
        controls/modes/MissionMode.hpp
        controls/modes/MissionMode.cpp
        controls/modes/BattlePassMode.hpp
        controls/modes/BattlePassMode.cpp
        controls/modes/MissionPreviewMode.hpp
        controls/modes/MissionPreviewMode.cpp
        controls/modes/AttributesMode.hpp
        controls/modes/AttributesMode.cpp

        graphics/Renderer.hpp
        graphics/Renderer.cpp

        thirdparty/src/glad.c
)

target_include_directories(RotMGPad PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/include
)

target_link_libraries(RotMGPad PRIVATE
        OpenGL::GL
        SDL2::SDL2
        SDL2::SDL2main
        glfw
        Freetype::Freetype
)

if (UNIX AND NOT APPLE)
    target_include_directories(RotMGPad PRIVATE
            ${X11_INCLUDE_DIR}
    )

    target_link_libraries(RotMGPad PRIVATE
            ${X11_LIBRARIES}
            Xfixes
    )
endif()

#
# Installation
#

include(GNUInstallDirs)

install(TARGETS RotMGPad
        RUNTIME DESTINATION .
)

install(FILES
        ${CMAKE_SOURCE_DIR}/fonts/DejaVuSans.ttf
        DESTINATION fonts
)