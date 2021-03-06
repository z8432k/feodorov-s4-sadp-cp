set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE  ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


add_library(btree
        OBJECT
        btree.c)
target_include_directories(btree
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})

add_library(avltree
        OBJECT
        avltree.c)
target_include_directories(avltree
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})


add_library(hashtab
        OBJECT
        hashtab.c)
target_include_directories(hashtab
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})


add_library(layerlist
        OBJECT
        layerlist.c)
target_include_directories(layerlist
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})

add_library(skiplist
        OBJECT
        skiplist.c)
target_include_directories(skiplist
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

add_library(wordsearch
        OBJECT
        wordsearch.c)
target_include_directories(wordsearch
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})

add_library(core
        SHARED
        $<TARGET_OBJECTS:hashtab>
        $<TARGET_OBJECTS:btree>
        $<TARGET_OBJECTS:merge_sort>
        $<TARGET_OBJECTS:layerlist>
        $<TARGET_OBJECTS:skiplist>
        $<TARGET_OBJECTS:wordsearch>
        $<TARGET_OBJECTS:avltree>)
target_link_libraries(core
        INTERFACE
        ${GLIB2_LDFLAGS})
target_include_directories(core
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${GLIB2_INCLUDE_DIRS})
