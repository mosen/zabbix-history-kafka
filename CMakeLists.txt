cmake_minimum_required(VERSION 3.14)
project(zbxrdkafka C)

set(CMAKE_C_STANDARD 99)
# The sources directory must point to a checked out AND ./configure`d zabbix source tree
set(ZABBIX_SOURCES /path/to/zabbix/)
set(RDKAFKA_HEADERS /usr/local/include/librdkafka)
set(ZABBIX_MODULES /opt/zabbix42/lib/modules)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(Config REQUIRED)

set(SOURCES
        src/libzbxrdkafka.c
        src/libzbxrdkafka.h
        src/config.c
        src/producer.c
        )

set(TEST_SOURCES ${SOURCES} src/test.c)

set(LIBS
        ${CONFIG_LIBRARY}
        /usr/local/lib/librdkafka.dylib
        )

# TARGETS

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})

add_executable(${PROJECT_NAME}-test ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}-test ${LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC
        "${ZABBIX_SOURCES}/include"
        "${RDKAFKA_HEADERS}"
        "${LIBCONFIG_INCLUDE_DIRS}"
        )

target_include_directories(${PROJECT_NAME}-test PUBLIC
        "${ZABBIX_SOURCES}/include"
        "${RDKAFKA_HEADERS}"
        "${LIBCONFIG_INCLUDE_DIRS}"
        )

# Necessary to stop Apple compiler complaining about undefined symbols linking to zabbix modules
IF(APPLE)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    SET_TARGET_PROPERTIES(${PROJECT_NAME}-test PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
ENDIF(APPLE)

# INSTALL

install(TARGETS ${PROJECT_NAME} DESTINATION ${ZABBIX_MODULES})
