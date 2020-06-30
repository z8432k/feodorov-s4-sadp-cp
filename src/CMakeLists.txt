set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE  ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

list(APPEND _targets
        regclient
        addcar
        listcars
        listclients
        dropclient
        dropcar
        servicecar
        rent
        searchcar
        searchclient)

foreach(_target IN LISTS _targets)
  add_executable(${_target} ${_target}.c)

  target_link_libraries(${_target}
          rentcar
          core)
endforeach()

add_subdirectory(core)
add_subdirectory(common)
