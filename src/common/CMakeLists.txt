set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE  ON)

pkg_check_modules(JANSSON REQUIRED jansson)
pkg_check_modules(PGSQL REQUIRED libpq)

add_library(data
        OBJECT
        data.c)
target_include_directories(data
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/storage/include
        ${GLIB2_INCLUDE_DIRS})

add_library(json_store
        OBJECT
        storage/json_store.c)
target_include_directories(json_store
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/storage/include
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})

add_library(pgsql_store
        OBJECT
        storage/psql_store.c)
target_include_directories(pgsql_store
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/storage/include
        ${GLIB2_INCLUDE_DIRS}
        ${PGSQL_INCLUDE_DIRS})

add_library(client
        OBJECT
        client.c)
target_include_directories(client
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})

add_library(car
        OBJECT
        car.c)
target_include_directories(car
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})

add_library(rentrow
        OBJECT
        rentrow.c)
target_include_directories(rentrow
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})

add_library(stringify_json
        OBJECT
        stringify_json.c)
target_include_directories(stringify_json
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/storage/include
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})

add_library(common
        STATIC
        $<TARGET_OBJECTS:data>
        $<TARGET_OBJECTS:json_store>
        $<TARGET_OBJECTS:pgsql_store>
        $<TARGET_OBJECTS:client>
        $<TARGET_OBJECTS:car>
        $<TARGET_OBJECTS:rentrow>
        $<TARGET_OBJECTS:stringify_json>)
target_link_libraries(common
        INTERFACE
        ${GLIB2_LDFLAGS}
        ${PGSQL_LDFLAGS}
        ${JANSSON_LDFLAGS})
target_include_directories(common
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/storage/include
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})
