set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE  ON)

pkg_check_modules(JANSSON REQUIRED jansson)
pkg_check_modules(SQLITE REQUIRED sqlite3)
pkg_check_modules(PGSQL REQUIRED libpq)

add_library(data
        OBJECT
        include/data.h
        data.c)
target_include_directories(data
        PRIVATE
        ${GLIB2_INCLUDE_DIRS})

add_library(json_store
        OBJECT
        include/storage/json_store.h
        storage/json_store.c)
target_include_directories(json_store
        PRIVATE
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})

add_library(sqlite_store
        OBJECT
        include/storage/sqlite_store.h
        storage/sqlite_store.c)
target_include_directories(sqlite_store
        PRIVATE
        ${GLIB2_INCLUDE_DIRS}
        ${SQLITE_INCLUDE_DIRS})

add_library(pgsql_store
        OBJECT
        include/storage/pgsql_store.h
        storage/psql_store.c)
target_include_directories(pgsql_store
        PRIVATE
        ${GLIB2_INCLUDE_DIRS}
        ${PGSQL_INCLUDE_DIRS})

add_library(client
        OBJECT
        include/client.h
        client.c)
target_include_directories(client
        PRIVATE
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})

add_library(car
        OBJECT
        include/car.h
        car.c)
target_include_directories(car
        PRIVATE
        ${GLIB2_INCLUDE_DIRS})

add_library(rentrow
        OBJECT
        include/rentrow.h
        rentrow.c)
target_include_directories(rentrow
        PRIVATE
        ${GLIB2_INCLUDE_DIRS})

add_library(stringify_json
        OBJECT
        include/stringify_json.h
        stringify_json.c)
target_include_directories(stringify_json
        PRIVATE
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
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})

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
  add_executable(${_target} ${_target}.c)

  target_link_libraries(${_target}
          common)
endforeach()

add_subdirectory(core)
