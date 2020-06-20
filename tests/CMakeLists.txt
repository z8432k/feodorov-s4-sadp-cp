set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE  ON)

list(APPEND _tests
  test_hashtab
  test_merge_sort)

foreach(_test IN LISTS _tests)
  add_executable(${_test} ${_test}.c)

  target_include_directories(${_test}
    PRIVATE
      ${GLIB2_INCLUDE_DIRS}
      ${JANSSON_INCLUDE_DIRS})

  target_link_libraries(${_test}
    common
    ${GLIB2_LDFLAGS}
    ${JANSSON_LDFLAGS})
endforeach()

add_executable(test_btree btree/btree_main_test.c)
target_link_libraries(test_btree
  common)

enable_testing()

add_test(
  NAME hashtab_test
  COMMAND $<TARGET_FILE:test_hashtab>)

add_test(
  NAME merge_sort_test
  COMMAND $<TARGET_FILE:test_merge_sort>)

add_test(
  NAME btree_test
  COMMAND $<TARGET_FILE:test_btree>)

add_test(
  NAME test_btree
  COMMAND $<TARGET_FILE:test_btree>)