set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE  ON)


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
add_subdirectory(common)
