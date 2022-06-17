set(CMAKE_AUTOMOC ON)

add_subdirectory(projectManager)
# add_subdirectory(cardReader)
# add_subdirectory(ERP)
# add_subdirectory(LED)
# add_subdirectory(ET199)
# add_subdirectory(plcServer)

add_library(sysLib )

target_link_libraries(sysLib
    projectManagerLib)