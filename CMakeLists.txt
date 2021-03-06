cmake_minimum_required(VERSION 3.3.2)
if(APPLE)
  cmake_policy(SET CMP0042 NEW)
endif()

if(UNIX AND NOT APPLE)
	set(FCGIFUNC_LINUX TRUE)
else(UNIX AND NOT APPLE)
	set(FCGIFUNC_MACOSX TRUE)
	set(FCGIFUNC_CMAKE_MACOSX_RPATH 1)
endif(UNIX AND NOT APPLE)

set(PROJ_FCGIFUNC cgi)
project(cgi 
	VERSION 2.1
	LANGUAGES C)

# Some tweak parameters
SET(PWD ${CMAKE_CURRENT_SOURCE_DIR})

IF (NOT DEFINED CMAKE_INSTALL_BINDIR)
SET(CMAKE_INSTALL_BINDIR /usr/local/bin)
ENDIF (NOT DEFINED CMAKE_INSTALL_BINDIR)

IF (NOT DEFINED CMAKE_INSTALL_LIBDIR)
SET(CMAKE_INSTALL_LIBDIR /usr/local/lib)
ENDIF (NOT DEFINED CMAKE_INSTALL_LIBDIR)

IF (NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
SET(CMAKE_INSTALL_INCLUDEDIR /usr/local/include)
ENDIF(NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)

MESSAGE(STATUS "cgi LIBRARY DIRECTORY located at ${CMAKE_INSTALL_LIBDIR}")
message(STATUS "Current SOURCES DIRECTORY located at ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Current Binary DIRECTORY located at ${CMAKE_CURRENT_BINARY_DIR}")

SET(LIBPATH /usr/lib /usr/local/lib)

include_directories(include /usr/local/include)

add_definitions(-D_XOPEN_SOURCE)


# add_definitions(-DFDYMEMDETECT=true)

# for static libary find first priority
# find_library(FCGI_LIB NAMES libfcgi.a PATHS ${LIBPATH})
# for dynamic library find second priority
find_library(FCGI_LIB NAMES fcgi PATHS ${LIBPATH})
if(FCGI_LIB)
	message(STATUS "fcgi found at ${FCGI_LIB}. fcgi support is compiled in.")
	add_definitions(-DFCGI=true)
else(FCGI_LIB)
	message( FATAL_ERROR "fcgi not found. Please install fcgi")
endif(FCGI_LIB)

find_library(THREAD_LIB NAMES pthread PATHS ${LIBPATH})
if(THREAD_LIB)
else(THREAD_LIB)
	message( FATAL_ERROR "pthread not found. Please install pthread")
endif(THREAD_LIB)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wstrict-prototypes -Wmissing-prototypes")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-declarations -Wshadow -Wpointer-arith -Wcast-qual")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wsign-compare -Iinclude -std=c11 -pedantic")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

# Not need to export dynamic symbol out, -rdyanmic only for user application
# SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-rdynamic") # LDFLAGS=
#  gcc -fPIC -pie -o libcgi.so src/cgi_core.c src/cgi_pool.c -Iinclude -Wl,-E -Wl,--whole-archive -lfcgi -latomic -ldl -Wl,--no-whole-archive

if(UNIX AND NOT APPLE)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pie -Wl,-E")
else(UNIX AND NOT APPLE)
add_definitions(-DFCGIFUNC_MACOSX=true)
endif(UNIX AND NOT APPLE)

# uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

SET (SRCS cgi.c)
SET (HDRS cgi.h)

add_library(${PROJ_FCGIFUNC}.static STATIC ${HDRS} ${SRCS})
set_target_properties(${PROJ_FCGIFUNC}.static PROPERTIES OUTPUT_NAME ${PROJ_FCGIFUNC})

add_library(${PROJ_FCGIFUNC} SHARED ${HDRS} ${SRCS})

link_directories(/usr/local/lib /usr/lib)

# target_link_libraries(cgi ${PROJ_FCGIFUNC})
# target_link_libraries(cgi fcgi dl)

if(UNIX AND NOT APPLE)
# target_link_libraries(${PROJ_FCGIFUNC} ${LIB_DL}) #static link to fcgi
target_link_libraries(${PROJ_FCGIFUNC} fcgi dl pthread)  #dynamic link to -ldl
# target_link_libraries(${PROJ_FCGIFUNC}.static ${LIB_DL}) #static link 
target_link_libraries(${PROJ_FCGIFUNC}.static fcgi dl pthread) #dynamic link to -ldl
else(UNIX AND NOT APPLE)
# target_link_libraries(${PROJ_FCGIFUNC} ${LIB_DL}) #static link to fcgi
target_link_libraries(${PROJ_FCGIFUNC} fcgi dl pthread)  #dynamic link to -ldl
# target_link_libraries(${PROJ_FCGIFUNC}.static ${LIB_DL}) #static link 
target_link_libraries(${PROJ_FCGIFUNC}.static fcgi dl pthread) #dynamic link to -ldl
endif(UNIX AND NOT APPLE)


install(TARGETS ${PROJ_FCGIFUNC} EXPORT ${PROJ_FCGIFUNC}
		ARCHIVE DESTINATION lib${LIB_SUFFIX}
		LIBRARY DESTINATION lib${LIB_SUFFIX}
		# RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		)
install(TARGETS ${PROJ_FCGIFUNC}.static EXPORT ${PROJ_FCGIFUNC}
		ARCHIVE DESTINATION lib${LIB_SUFFIX}
		LIBRARY DESTINATION lib${LIB_SUFFIX}
		# RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		)
install(FILES cgi.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install(TARGETS cgi EXPORT cgi DESTINATION ${CMAKE_INSTALL_BINDIR})
install(CODE "execute_process(COMMAND ldconfig)")

# IF (${CMAKE_BUILD_TYPE} MATCHES "Debug")
# 	add_definitions(-D__DEBUG__)
# 	SET(CMAKE_C_FLAGS "-Wall -O0 -fPIC ${CMAKE_C_FLAGS}")
# 	SET(CMAKE_CXX_FLAGS "-Wall -O0 -fPIC ${CMAKE_CXX_FLAGS}")
# ELSE (${CMAKE_BUILD_TYPE} MATCHES "Debug")
# 	SET(CMAKE_C_FLAGS "-Wall -O2 -fPIC ${CMAKE_C_FLAGS}")
# 	SET(CMAKE_CXX_FLAGS "-Wall -O2 -fPIC ${CMAKE_CXX_FLAGS}")
# ENDIF (${CMAKE_BUILD_TYPE} MATCHES "Debug")
