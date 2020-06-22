set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE  ON)

list(APPEND _tests
  test_hashtab
  test_merge_sort
  test_btree)

foreach(_test IN LISTS _tests)
  add_executable(${_test} ${_test}.c)

  target_compile_options(${_test}
          PRIVATE
          "-Wno-unused-function -Wno-unused-variable")

  target_include_directories(${_test}
    PRIVATE)

  target_link_libraries(${_test}
    core)
endforeach()

add_executable(test_btree_new btree/btree_main_test.c)
target_link_libraries(test_btree_new
  core)

add_executable(test_layerlist layerlist/layerlist_main_test.c)
target_link_libraries(test_layerlist
        core)

add_executable(test_skiplist skiplist/skiplist_main_test.c)
target_compile_options(test_skiplist
        PRIVATE
        "-Wno-unused-function" "-Wno-unused-variable")
target_link_libraries(test_skiplist
        core)

add_executable(test_wordsearch wordsearch/wordsearch_main_test.c)
target_compile_options(test_wordsearch
        PRIVATE
        "-Wno-unused-function" "-Wno-unused-variable")
target_link_libraries(test_wordsearch
        core)

enable_testing()

add_test(
  NAME hashtab_test
  COMMAND $<TARGET_FILE:test_hashtab>)

add_test(
  NAME merge_sort_test
  COMMAND $<TARGET_FILE:test_merge_sort>)

add_test(
  NAME test_btree
  COMMAND $<TARGET_FILE:test_btree>)

add_test(
  NAME test_btree_new
  COMMAND $<TARGET_FILE:test_btree_new>)

add_test(
        NAME test_layerlist
        COMMAND $<TARGET_FILE:test_layerlist>)

add_test(
        NAME test_skiplist
        COMMAND $<TARGET_FILE:test_skiplist>)

add_test(
        NAME test_wordsearch
        COMMAND gtester --verbose $<TARGET_FILE:test_wordsearch>)
