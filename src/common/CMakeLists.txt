set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE  ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

pkg_check_modules(JANSSON REQUIRED jansson)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/../core/include
        ${CMAKE_CURRENT_LIST_DIR}/storage_drivers/include
        ${GLIB2_INCLUDE_DIRS})

add_library(data
        OBJECT
        data.c)

add_library(main
        OBJECT
        rentcar.c)

add_library(storage
        OBJECT
        storage.c)

add_library(client
        OBJECT
        client.c)

add_library(car
        OBJECT
        car.c)

add_library(rentrow
        OBJECT
        rentrow.c)

add_library(stringify_json
        OBJECT
        stringify_json.c)
target_include_directories(stringify_json
        PRIVATE
        ${JANSSON_INCLUDE_DIRS})

add_library(rentcar
        SHARED
        $<TARGET_OBJECTS:data>
        $<TARGET_OBJECTS:storage>
        $<TARGET_OBJECTS:client>
        $<TARGET_OBJECTS:car>
        $<TARGET_OBJECTS:rentrow>
        $<TARGET_OBJECTS:main>
        $<TARGET_OBJECTS:stringify_json>)
target_link_libraries(rentcar
        INTERFACE
        ${GLIB2_LDFLAGS}
        ${PGSQL_LDFLAGS}
        ${JANSSON_LDFLAGS}
        ${CMAKE_DL_LIBS})
target_include_directories(rentcar
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/storage/include
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})

add_subdirectory(storage_drivers)
