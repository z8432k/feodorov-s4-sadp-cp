cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(feodorov-s4-sadp-cp LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -O0 -ggdb")
set(CMAKE_C_FLAGS_RELEASE "-march=native -O3")

set(ENV{PKG_CONFIG_PATH} ~/local/lib/pkgconfig)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(JANSSON REQUIRED jansson)

add_library(data
        OBJECT
        src/include/data.h
        src/data.c)
target_include_directories(data PRIVATE ${GLIB2_INCLUDE_DIRS})

add_library(store
        OBJECT
        src/include/store.h
        src/store.c)
target_include_directories(store PRIVATE ${GLIB2_INCLUDE_DIRS})
target_include_directories(store PRIVATE ${JANSSON_INCLUDE_DIRS})

add_library(client
        OBJECT
        src/include/client.h
        src/client.c)
target_include_directories(client PRIVATE ${GLIB2_INCLUDE_DIRS})

add_library(car
        OBJECT
        src/include/car.h
        src/car.c)
target_include_directories(car PRIVATE ${GLIB2_INCLUDE_DIRS})

add_library(common
        STATIC
        $<TARGET_OBJECTS:data>
        $<TARGET_OBJECTS:store>
        $<TARGET_OBJECTS:client>
        $<TARGET_OBJECTS:car>)

add_executable(regclient src/regclient.c)
target_include_directories(regclient
        PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(regclient
        common
        ${GLIB2_LDFLAGS}
        ${JANSSON_LDFLAGS})

add_executable(addcar src/addcar.c)
target_include_directories(addcar
        PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(addcar
        common
        ${GLIB2_LDFLAGS}
        ${JANSSON_LDFLAGS})

add_executable(listcars src/listcars.c)
target_include_directories(listcars
        PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(listcars
        common
        ${GLIB2_LDFLAGS}
        ${JANSSON_LDFLAGS})

add_executable(listclients src/listclients.c)
target_include_directories(listclients
        PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(listclients
        common
        ${GLIB2_LDFLAGS}
        ${JANSSON_LDFLAGS})

add_executable(dropclient src/dropclient.c)
target_include_directories(dropclient
        PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(dropclient
        common
        ${GLIB2_LDFLAGS}
        ${JANSSON_LDFLAGS})

add_executable(dropcar src/dropcar.c)
target_include_directories(dropcar
        PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(dropcar
        common
        ${GLIB2_LDFLAGS}
        ${JANSSON_LDFLAGS})

add_executable(servicecar src/servicecar.c)
target_include_directories(servicecar
        PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(servicecar
        common
        ${GLIB2_LDFLAGS}
        ${JANSSON_LDFLAGS})

add_executable(rentcar src/rentcar.c)
target_include_directories(rentcar
        PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(rentcar
        common
        ${GLIB2_LDFLAGS}
        ${JANSSON_LDFLAGS})

add_executable(searchcar src/searchcar.c)
target_include_directories(searchcar
        PRIVATE ${GLIB2_INCLUDE_DIRS})
target_link_libraries(searchcar
        common
        ${GLIB2_LDFLAGS}
        ${JANSSON_LDFLAGS})
