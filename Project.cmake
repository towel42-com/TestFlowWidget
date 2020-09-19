#
# (c) Copyright 2004 - 2015 Blue Pearl Software Inc.
# All rights reserved.
#
# This source code belongs to Blue Pearl Software Inc.
# It is considered trade secret and confidential, and is not to be used
# by parties who have not received written authorization
# from Blue Pearl Software Inc.
#
# Only authorized users are allowed to use, copy and modify
# this software provided that the above copyright notice
# remains in all copies of this software.
#
#
#
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTWEBKIT TRUE )
SET( QT_USE_QTSQL TRUE )

AddQtIncludes( Gui )
AddQtIncludes( WebKit )
AddQtIncludes( Sql )
AddQtIncludes( Widgets )
AddQtIncludes( WebKitWidgets )
QT5_WRAP_CPP(qtproject_MOC_SRCS ${qtproject_H})
QT5_WRAP_UI(qtproject_UIS_H ${qtproject_UIS})

IF(WIN32)
    IF( ${BITSIZE} EQUAL 32 )
	    SET(CMAKE_CXX_FLAGS_DEBUG          "/D_DEBUG /RTC1 /Gm /MDd /ZI /W3 /Od     /w34700 /w34701 /w34715 /w34716 /w34717 /w44800 /wd4251 /wd4231 ")
	    SET(CMAKE_C_FLAGS_DEBUG            "/D_DEBUG /RTC1 /Gm /MDd /ZI /W3 /Od     /w34700 /w34701 /w34715 /w34716 /w34717 /w44800 /wd4251 /wd4231 ")
	    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /Zi /O2 /Ob1 /DNDEBUG               /w34700 /w34701 /w34715 /w34716 /w34717 /w44800 /wd4251 /wd4231 /DQT_NO_DEBUG_OUTPUT")
	    SET(CMAKE_C_FLAGS_RELWITHDEBINFO   "/MD /Zi /O2 /Ob1 /DNDEBUG               /w34700 /w34701 /w34715 /w34716 /w34717 /w44800 /wd4251 /wd4231 /DQT_NO_DEBUG_OUTPUT")

    ELSE()
	    SET(CMAKE_CXX_FLAGS_DEBUG          "/D_DEBUG /RTC1 /Gm /MDd /ZI /W3 /Od /bigobj     /w34700 /w34701 /w34715 /w34716 /w34717 /w44800 /wd4251 /wd4231")
	    SET(CMAKE_C_FLAGS_DEBUG            "/D_DEBUG /RTC1 /Gm /MDd /ZI /W3 /Od /bigobj     /w34700 /w34701 /w34715 /w34716 /w34717 /w44800 /wd4251 /wd4231")
	    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /Zi /O2 /Ob1 /DNDEBUG               /w34700 /w34701 /w34715 /w34716 /w34717 /w44800 /wd4251 /wd4231 /DQT_NO_DEBUG_OUTPUT")
	    SET(CMAKE_C_FLAGS_RELWITHDEBINFO   "/MD /Zi /O2 /Ob1 /DNDEBUG               /w34700 /w34701 /w34715 /w34716 /w34717 /w44800 /wd4251 /wd4231 /DQT_NO_DEBUG_OUTPUT")
        SET( BPS_NO_WARNING_AS_ERROR TRUE )
    ENDIF()

    IF( ${BITSIZE} EQUAL 32 )
	    SET(CMAKE_EXE_LINKER_FLAGS_DEBUG          "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /STACK:8388608")
	    SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /STACK:8388608")   
    ELSE()
	    SET(CMAKE_EXE_LINKER_FLAGS_DEBUG          "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /STACK:8388608 /HIGHENTROPYVA:NO")
	    SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /STACK:8388608 /HIGHENTROPYVA:NO")   
    ENDIF()

    SET(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} /WX /w34100")
    SET(CMAKE_C_FLAGS_DEBUG            "${CMAKE_C_FLAGS_DEBUG} /WX /w34100")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /WX /w34100")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /WX /w34100")

    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /DQT_NO_DEBUG /DQT_NO_NDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DQT_DEBUG")
ENDIF()

add_definitions(-DQT_STRICT_ITERATORS)

add_definitions(
       -D_CRT_SECURE_NO_WARNINGS
       -D_CRT_SECURE_NO_DEPRECATE
       -D_CRT_NONSTDC_NO_WARNINGS
)
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /SAFESEH:NO")
SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
SET(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} /SAFESEH:NO")
SET(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
SET(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO} /SAFESEH:NO")
add_definitions( -DQT_CC_WARNINGS -DQT_NO_WARNINGS )

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/UI)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

source_group("Generated Files" FILES ${qtproject_UIS_H} ${qtproject_MOC_SRCS} ${qtproject_QRC_SRCS} )
source_group("Resource Files" FILES ${qtproject_QRC} ${qtproject_QRC_SOURCES} )
source_group("Designer Files" FILES ${qtproject_UIS} )
source_group("Header Files" FILES ${qtproject_H} )


