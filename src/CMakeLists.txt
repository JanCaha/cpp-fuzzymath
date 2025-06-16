set(HEADERS
    api/Interval.h
    api/FuzzyMembership.h
    api/PossibilisticMembership.h
    api/FuzzyNumber.h
)

set(SOURCES
    AlphaCut.cpp
    Functions.cpp
    FuzzyNumber.cpp
    FuzzyNumberFactory.cpp
    FuzzyMembership.cpp
    Interval.cpp
    PossibilisticMembership.cpp
)

macro(library_settings target_name)
    string(TOLOWER ${LIBRARY_NAME} LIBRARY_NAME_LOWER)

    get_target_property(target_type ${target_name} TYPE)

    if (WIN32)
        if(target_type STREQUAL "STATIC_LIBRARY")
            set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "${LIBRARY_NAME_LOWER}_static")
        else()
            set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${LIBRARY_NAME_LOWER})
            target_compile_definitions(${target_name} PRIVATE FUZZYMATH_EXPORTS)
        endif()
    else()
        set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${LIBRARY_NAME_LOWER})
    endif()

    set_target_properties(${target_name} PROPERTIES VERSION ${PROJECT_VERSION})

    include(GNUInstallDirs)
  
    target_include_directories(${target_name}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/api>
        $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
    )

    target_include_directories(${target_name} PRIVATE
        ${Boost_INCLUDE_DIRS}
    )
endmacro(library_settings)


add_library(FuzzyMath_shared SHARED
    ${SOURCES}
)

add_library(FuzzyMath_static STATIC
    ${SOURCES}
)

library_settings(FuzzyMath_shared)
library_settings(FuzzyMath_static)

include(GNUInstallDirs)

install(
    TARGETS FuzzyMath_shared FuzzyMath_static
    EXPORT FuzzyMathTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_NAME}
)