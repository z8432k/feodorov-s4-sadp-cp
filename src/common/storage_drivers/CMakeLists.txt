set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE  ON)

pkg_check_modules(PGSQL REQUIRED libpq)

add_library(rentcar_json_store
        MODULE
        json_store.c)
target_include_directories(rentcar_json_store
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/../../core/include
        ${CMAKE_CURRENT_LIST_DIR}/../include
        ${CMAKE_CURRENT_LIST_DIR}/storage/include
        ${GLIB2_INCLUDE_DIRS}
        ${JANSSON_INCLUDE_DIRS})
target_link_libraries(rentcar_json_store
        INTERFACE
        ${GLIB2_LDFLAGS}
        ${JANSSON_LDFLAGS})

add_library(rentcar_pgsql_store
        MODULE
        pgsql_store.c)
target_include_directories(rentcar_pgsql_store
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/../../core/include
        ${CMAKE_CURRENT_LIST_DIR}/../include
        ${CMAKE_CURRENT_LIST_DIR}/storage/include
        ${GLIB2_INCLUDE_DIRS}
        ${PGSQL_INCLUDE_DIRS})
target_link_libraries(rentcar_pgsql_store
        INTERFACE
        ${GLIB2_LDFLAGS}
        ${PGSQL_LDFLAGS})
