set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE  ON)


add_library(btree
        OBJECT
        btree.c)
target_include_directories(btree
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})
target_compile_options(btree PRIVATE "-Wno-unused-parameter")


add_library(hashtab
        OBJECT
        hashtab.c)
target_include_directories(hashtab
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})


add_library(merge_sort
        OBJECT
        merge_sort.c)
target_include_directories(merge_sort
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})


add_library(core
        STATIC
        $<TARGET_OBJECTS:hashtab>
        $<TARGET_OBJECTS:btree>
        $<TARGET_OBJECTS:merge_sort>)
target_link_libraries(core
        INTERFACE
        ${GLIB2_LDFLAGS})
target_include_directories(core
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})
