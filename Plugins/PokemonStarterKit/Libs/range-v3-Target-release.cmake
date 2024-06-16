# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(range-v3_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(range-v3_FRAMEWORKS_FOUND_RELEASE "${range-v3_FRAMEWORKS_RELEASE}" "${range-v3_FRAMEWORK_DIRS_RELEASE}")

set(range-v3_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET range-v3_DEPS_TARGET)
    add_library(range-v3_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET range-v3_DEPS_TARGET
             PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${range-v3_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${range-v3_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:range-v3::range-v3-meta>
             APPEND)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### range-v3_DEPS_TARGET to all of them
conan_package_library_targets("${range-v3_LIBS_RELEASE}"    # libraries
                              "${range-v3_LIB_DIRS_RELEASE}" # package_libdir
                              range-v3_DEPS_TARGET
                              range-v3_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "range-v3")    # package_name

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${range-v3_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Release ########################################

    ########## COMPONENT range-v3::range-v3-concepts #############

        set(range-v3_range-v3_range-v3-concepts_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(range-v3_range-v3_range-v3-concepts_FRAMEWORKS_FOUND_RELEASE "${range-v3_range-v3_range-v3-concepts_FRAMEWORKS_RELEASE}" "${range-v3_range-v3_range-v3-concepts_FRAMEWORK_DIRS_RELEASE}")

        set(range-v3_range-v3_range-v3-concepts_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET range-v3_range-v3_range-v3-concepts_DEPS_TARGET)
            add_library(range-v3_range-v3_range-v3-concepts_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET range-v3_range-v3_range-v3-concepts_DEPS_TARGET
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-concepts_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-concepts_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-concepts_DEPENDENCIES_RELEASE}>
                     APPEND)

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'range-v3_range-v3_range-v3-concepts_DEPS_TARGET' to all of them
        conan_package_library_targets("${range-v3_range-v3_range-v3-concepts_LIBS_RELEASE}"
                                      "${range-v3_range-v3_range-v3-concepts_LIB_DIRS_RELEASE}"
                                      range-v3_range-v3_range-v3-concepts_DEPS_TARGET
                                      range-v3_range-v3_range-v3-concepts_LIBRARIES_TARGETS
                                      "_RELEASE"
                                      "range-v3_range-v3_range-v3-concepts")

        ########## TARGET PROPERTIES #####################################
        set_property(TARGET range-v3::range-v3-concepts
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-concepts_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-concepts_LIBRARIES_TARGETS}>
                     APPEND)

        if("${range-v3_range-v3_range-v3-concepts_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET range-v3::range-v3-concepts
                         PROPERTY INTERFACE_LINK_LIBRARIES
                         range-v3_range-v3_range-v3-concepts_DEPS_TARGET
                         APPEND)
        endif()

        set_property(TARGET range-v3::range-v3-concepts PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-concepts_LINKER_FLAGS_RELEASE}> APPEND)
        set_property(TARGET range-v3::range-v3-concepts PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-concepts_INCLUDE_DIRS_RELEASE}> APPEND)
        set_property(TARGET range-v3::range-v3-concepts PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-concepts_COMPILE_DEFINITIONS_RELEASE}> APPEND)
        set_property(TARGET range-v3::range-v3-concepts PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-concepts_COMPILE_OPTIONS_RELEASE}> APPEND)

    ########## COMPONENT range-v3::range-v3-meta #############

        set(range-v3_range-v3_range-v3-meta_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(range-v3_range-v3_range-v3-meta_FRAMEWORKS_FOUND_RELEASE "${range-v3_range-v3_range-v3-meta_FRAMEWORKS_RELEASE}" "${range-v3_range-v3_range-v3-meta_FRAMEWORK_DIRS_RELEASE}")

        set(range-v3_range-v3_range-v3-meta_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET range-v3_range-v3_range-v3-meta_DEPS_TARGET)
            add_library(range-v3_range-v3_range-v3-meta_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET range-v3_range-v3_range-v3-meta_DEPS_TARGET
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-meta_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-meta_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-meta_DEPENDENCIES_RELEASE}>
                     APPEND)

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'range-v3_range-v3_range-v3-meta_DEPS_TARGET' to all of them
        conan_package_library_targets("${range-v3_range-v3_range-v3-meta_LIBS_RELEASE}"
                                      "${range-v3_range-v3_range-v3-meta_LIB_DIRS_RELEASE}"
                                      range-v3_range-v3_range-v3-meta_DEPS_TARGET
                                      range-v3_range-v3_range-v3-meta_LIBRARIES_TARGETS
                                      "_RELEASE"
                                      "range-v3_range-v3_range-v3-meta")

        ########## TARGET PROPERTIES #####################################
        set_property(TARGET range-v3::range-v3-meta
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-meta_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-meta_LIBRARIES_TARGETS}>
                     APPEND)

        if("${range-v3_range-v3_range-v3-meta_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET range-v3::range-v3-meta
                         PROPERTY INTERFACE_LINK_LIBRARIES
                         range-v3_range-v3_range-v3-meta_DEPS_TARGET
                         APPEND)
        endif()

        set_property(TARGET range-v3::range-v3-meta PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-meta_LINKER_FLAGS_RELEASE}> APPEND)
        set_property(TARGET range-v3::range-v3-meta PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-meta_INCLUDE_DIRS_RELEASE}> APPEND)
        set_property(TARGET range-v3::range-v3-meta PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-meta_COMPILE_DEFINITIONS_RELEASE}> APPEND)
        set_property(TARGET range-v3::range-v3-meta PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${range-v3_range-v3_range-v3-meta_COMPILE_OPTIONS_RELEASE}> APPEND)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET range-v3::range-v3 PROPERTY INTERFACE_LINK_LIBRARIES range-v3::range-v3-concepts APPEND)
    set_property(TARGET range-v3::range-v3 PROPERTY INTERFACE_LINK_LIBRARIES range-v3::range-v3-meta APPEND)

########## For the modules (FindXXX)
set(range-v3_LIBRARIES_RELEASE range-v3::range-v3)
