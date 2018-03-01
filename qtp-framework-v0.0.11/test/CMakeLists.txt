set(SERVER_SOURCE_FILES test_server.cc)
set(CLIENT_SOURCE_FILES test_client.cc)
set(UC_SOURCE_FILES test_uc.cc)

include_directories(. sbe)

add_executable(test_server ${SERVER_SOURCE_FILES})
target_link_libraries(test_server qtp_framework event_core)

add_executable(test_client ${CLIENT_SOURCE_FILES})
target_link_libraries(test_client qtp_framework event_core)

add_executable(test_uc ${UC_SOURCE_FILES})
target_link_libraries(test_uc qtp_framework event_core)