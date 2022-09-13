cmake_minimum_required(VERSION 3.22)
project(server_homework_8_2_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(server main.cpp
        MessengerServer.cpp
        ResponsesCreator.cpp
        AbonentsManager.cpp Abonent.cpp Abonent.h)

set(NECESSARY_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/key.pem
        ${CMAKE_CURRENT_SOURCE_DIR}/cert.pem
        ${CMAKE_CURRENT_SOURCE_DIR}/logins.txt
        )

add_custom_command(
        TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${NECESSARY_FILES}
        ${CMAKE_CURRENT_BINARY_DIR})

if(LIBHTTPSERVER_INCLUDE_DIRS AND LIBHTTPSERVER_LIBRARIES)
    set(LIBHTTPSERVER_FOUND TRUE)

else(LIBHTTPSERVER_INCLUDE_DIRS AND LIBHTTPSERVER_LIBRARIES)
    find_path(LIBHTTPSERVER_INCLUDE_DIRS httpserver
            /usr/include
            /user/include/httpserver
	    /usr/local/include/
	    /usr/local/include/httpserver
            )

    find_library(LIBHTTPSERVER_LIBRARIES NAMES httpserver
            PATHS
            /usr/lib
	    /usr/local/lib
	    /opt/local/lib
            )

    if(LIBHTTPSERVER_INCLUDE_DIRS AND LIBHTTPSERVER_LIBRARIES)
        set(LIBHTTPSERVER_FOUND TRUE)
        message(STATUS "Found libhttpserver: ${LIBHTTPSERVER_INCLUDE_DIRS}, ${LIBHTTPSERVER_LIBRARIES}")
    else(LIBHTTPSERVER_INCLUDE_DIRS AND LIBHTTPSERVER_LIBRARIES)
        set(LIBHTTPSERVER_FOUND FALSE)
        message(STATUS "libhttpserver not found.")
    endif(LIBHTTPSERVER_INCLUDE_DIRS AND LIBHTTPSERVER_LIBRARIES)

    mark_as_advanced(LIBHTTPSERVER_INCLUDE_DIRS LIBHTTPSERVER_LIBRARIES)

endif(LIBHTTPSERVER_INCLUDE_DIRS AND LIBHTTPSERVER_LIBRARIES)

target_link_libraries(server ${LIBHTTPSERVER_LIBRARIES})
