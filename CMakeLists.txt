cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(feodorov-s4-sadp-cp LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCC)
  message(STATUS "Detected GNU compiler. Using specific flags.")
  set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -O0 -ggdb")
  set(CMAKE_C_FLAGS_RELEASE "-march=native -O3")
endif()


find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(JANSSON REQUIRED jansson)
pkg_check_modules(SQLITE REQUIRED sqlite3)

add_library(data
        OBJECT
        src/include/data.h
        src/data.c)
target_include_directories(data
        PRIVATE
        ${GLIB2_INCLUDE_DIRS})

add_library(jsonstore
        OBJECT
        src/include/jsonstore.h
        src/jsonstore.c)
target_include_directories(jsonstore
        PRIVATE
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})

add_library(client
        OBJECT
        src/include/client.h
        src/client.c)
target_include_directories(client
        PRIVATE
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})

add_library(car
        OBJECT
        src/include/car.h
        src/car.c)
target_include_directories(car
        PRIVATE
        ${GLIB2_INCLUDE_DIRS})

add_library(rentrow
        OBJECT
        src/include/rentrow.h
        src/rentrow.c)
target_include_directories(rentrow
        PRIVATE
        ${GLIB2_INCLUDE_DIRS})

add_library(stringify_json
        OBJECT
        src/include/stringify_json.h
        src/stringify_json.c)
target_include_directories(stringify_json
        PRIVATE
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})

add_library(common
        STATIC
        $<TARGET_OBJECTS:data>
        $<TARGET_OBJECTS:jsonstore>
        $<TARGET_OBJECTS:client>
        $<TARGET_OBJECTS:car>
        $<TARGET_OBJECTS:rentrow>
        $<TARGET_OBJECTS:stringify_json>)

list(APPEND _targets
        regclient
        addcar
        listcars
        listclients
        dropclient
        dropcar
        servicecar
        rentcar
        searchcar
        searchclient)

foreach(_target IN LISTS _targets)
  add_executable(${_target} src/${_target}.c)

  target_include_directories(${_target}
          PRIVATE
          ${GLIB2_INCLUDE_DIRS}
          ${JANSSON_INCLUDE_DIRS})

  target_link_libraries(${_target}
          common
          ${GLIB2_LDFLAGS}
          ${JANSSON_LDFLAGS})
endforeach()

