cmake_minimum_required(VERSION 3.1)

project(mkrom C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Werror")

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(mkrom main.c)

ADD_CUSTOM_COMMAND(
  TARGET mkrom
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mkrom.1.txt
  COMMAND a2x --no-xmllint --doctype manpage --format manpage ${CMAKE_CURRENT_SOURCE_DIR}/mkrom.1.txt -v -D ${CMAKE_CURRENT_BINARY_DIR}
  OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/mkrom.1
)

install(TARGETS mkrom DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/mkrom.1
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man1
)
