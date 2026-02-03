cmake_minimum_required(VERSION 3.15)
project(Computer_Club_Schedule LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-18")

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy;
    --header-filter=.*
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(SANITIZER_FLAGS
    -fsanitize=address
    -fsanitize=undefined
    -fsanitize=leak
    -fno-omit-frame-pointer
    -fno-sanitize-recover=all
)

set(Flags
    -O0
    -Werror
    -pedantic-errors
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wconversion
    ${SANITIZER_FLAGS}
)

file(GLOB_RECURSE FILES_IN_SRC "src/*")

set(SRC_FILES "")

foreach(FILE ${FILES_IN_SRC})
    if(FILE MATCHES "\\.cpp$" OR FILE MATCHES "\\.c$")
        list(APPEND SRC_FILES ${FILE})
    endif()
endforeach()

file(GLOB_RECURSE INCLUDE_DIRS_AND_FILES LIST_DIRECTORIES true "include/*")

set(INCLUDE_DIRS "")

set(INCLUDE_HEADERS "")

foreach(FILE ${INCLUDE_DIRS_AND_FILES})
    if(IS_DIRECTORY ${FILE})
        list(APPEND INCLUDE_DIRS ${FILE})
    else()
        if(FILE MATCHES "\\.hpp$" OR FILE MATCHES "\\.h$")
            list(APPEND INCLUDE_HEADERS ${FILE})
        endif()
    endif()
endforeach()

add_executable(${PROJECT_NAME} ${SRC_FILES} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${INCLUDE_DIRS}
)

target_compile_options(${PROJECT_NAME} PRIVATE ${Flags})

target_link_options(${PROJECT_NAME} PRIVATE ${SANITIZER_FLAGS})

find_program(CLANG_TIDY NAMES clang-tidy-18)

if(CLANG_TIDY)
    execute_process(
        COMMAND ${CLANG_TIDY} --help
        OUTPUT_VARIABLE TIDY_HELP_OUTPUT
    )

    set(SUPPORTS_JOBS FALSE)
    if(TIDY_HELP_OUTPUT MATCHES "\\-j")
        set(SUPPORTS_JOBS TRUE)
    endif()

    set(TIDY_COMMAND ${CLANG_TIDY}
        ${INCLUDE_HEADERS} ${SRC_FILES}
        --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
        --header-filter=.*
        -p ${CMAKE_BINARY_DIR}
    )

    if(SUPPORTS_JOBS)
        list(APPEND TIDY_COMMAND "-j 0")
    endif()

    add_custom_target(lint
        COMMAND ${TIDY_COMMAND}
        COMMENT "Running clang-tidy"
    )
endif()

add_custom_target(run
    DEPENDS ${PROJECT_NAME}
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
)

file(WRITE ${CMAKE_BINARY_DIR}/run_program.sh
    "#!/bin/bash\n"
    "export ASAN_OPTIONS=\"detect_stack_use_after_return=1:detect_leaks=1:halt_on_error=1\"\n"
    "export UBSAN_OPTIONS=\"print_stacktrace=1:halt_on_error=1\"\n"
    "./${PROJECT_NAME} \"\$@\"\n"
)
execute_process(COMMAND chmod +x ${CMAKE_BINARY_DIR}/run_program.sh)