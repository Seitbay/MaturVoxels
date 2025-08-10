cmake_minimum_required(VERSION 3.10)
project(OpenGLEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Папки с заголовками
include_directories(${CMAKE_SOURCE_DIR}/include)

# Путь к библиотекам
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Ищем исходники .cpp
file(GLOB_RECURSE SOURCES src/*.cpp)

# Добавляем glad.c вручную
set(GLAD_SRC ${CMAKE_SOURCE_DIR}/src/glad.c)

# Создаём исполняемый файл с glad.c
add_executable(OpenGLEngine ${SOURCES} ${GLAD_SRC})

# Линкуем с GLFW и системными библиотеками
target_link_libraries(OpenGLEngine
    glfw3
    opengl32
    gdi32
    user32
    kernel32
)

# Абсолютный путь до папки assets в макрос
add_definitions(-DASSET_DIR="${CMAKE_SOURCE_DIR}/assets/")

# Копирование assets в build при каждой сборке
add_custom_command(TARGET OpenGLEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:OpenGLEngine>/assets
)
