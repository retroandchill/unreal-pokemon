########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND range-v3_COMPONENT_NAMES range-v3::range-v3-meta range-v3::range-v3-concepts)
list(REMOVE_DUPLICATES range-v3_COMPONENT_NAMES)
set(range-v3_FIND_DEPENDENCY_NAMES "")

########### VARIABLES #######################################################################
#############################################################################################
set(range-v3_PACKAGE_FOLDER_RELEASE "C:/Users/fcors/.conan/data/range-v3/0.12.0/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9")
set(range-v3_BUILD_MODULES_PATHS_RELEASE )


set(range-v3_INCLUDE_DIRS_RELEASE "${range-v3_PACKAGE_FOLDER_RELEASE}/include")
set(range-v3_RES_DIRS_RELEASE )
set(range-v3_DEFINITIONS_RELEASE )
set(range-v3_SHARED_LINK_FLAGS_RELEASE )
set(range-v3_EXE_LINK_FLAGS_RELEASE )
set(range-v3_OBJECTS_RELEASE )
set(range-v3_COMPILE_DEFINITIONS_RELEASE )
set(range-v3_COMPILE_OPTIONS_C_RELEASE )
set(range-v3_COMPILE_OPTIONS_CXX_RELEASE /permissive-)
set(range-v3_LIB_DIRS_RELEASE "${range-v3_PACKAGE_FOLDER_RELEASE}/lib")
set(range-v3_LIBS_RELEASE )
set(range-v3_SYSTEM_LIBS_RELEASE )
set(range-v3_FRAMEWORK_DIRS_RELEASE )
set(range-v3_FRAMEWORKS_RELEASE )
set(range-v3_BUILD_DIRS_RELEASE )

# COMPOUND VARIABLES
set(range-v3_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${range-v3_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${range-v3_COMPILE_OPTIONS_C_RELEASE}>")
set(range-v3_LINKER_FLAGS_RELEASE
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${range-v3_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${range-v3_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${range-v3_EXE_LINK_FLAGS_RELEASE}>")


set(range-v3_COMPONENTS_RELEASE range-v3::range-v3-meta range-v3::range-v3-concepts)
########### COMPONENT range-v3::range-v3-concepts VARIABLES ############################################

set(range-v3_range-v3_range-v3-concepts_INCLUDE_DIRS_RELEASE "${range-v3_PACKAGE_FOLDER_RELEASE}/include")
set(range-v3_range-v3_range-v3-concepts_LIB_DIRS_RELEASE "${range-v3_PACKAGE_FOLDER_RELEASE}/lib")
set(range-v3_range-v3_range-v3-concepts_RES_DIRS_RELEASE )
set(range-v3_range-v3_range-v3-concepts_DEFINITIONS_RELEASE )
set(range-v3_range-v3_range-v3-concepts_OBJECTS_RELEASE )
set(range-v3_range-v3_range-v3-concepts_COMPILE_DEFINITIONS_RELEASE )
set(range-v3_range-v3_range-v3-concepts_COMPILE_OPTIONS_C_RELEASE "")
set(range-v3_range-v3_range-v3-concepts_COMPILE_OPTIONS_CXX_RELEASE "")
set(range-v3_range-v3_range-v3-concepts_LIBS_RELEASE )
set(range-v3_range-v3_range-v3-concepts_SYSTEM_LIBS_RELEASE )
set(range-v3_range-v3_range-v3-concepts_FRAMEWORK_DIRS_RELEASE )
set(range-v3_range-v3_range-v3-concepts_FRAMEWORKS_RELEASE )
set(range-v3_range-v3_range-v3-concepts_DEPENDENCIES_RELEASE range-v3::range-v3-meta)
set(range-v3_range-v3_range-v3-concepts_SHARED_LINK_FLAGS_RELEASE )
set(range-v3_range-v3_range-v3-concepts_EXE_LINK_FLAGS_RELEASE )
# COMPOUND VARIABLES
set(range-v3_range-v3_range-v3-concepts_LINKER_FLAGS_RELEASE
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${range-v3_range-v3_range-v3-concepts_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${range-v3_range-v3_range-v3-concepts_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${range-v3_range-v3_range-v3-concepts_EXE_LINK_FLAGS_RELEASE}>
)
set(range-v3_range-v3_range-v3-concepts_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${range-v3_range-v3_range-v3-concepts_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${range-v3_range-v3_range-v3-concepts_COMPILE_OPTIONS_C_RELEASE}>")
########### COMPONENT range-v3::range-v3-meta VARIABLES ############################################

set(range-v3_range-v3_range-v3-meta_INCLUDE_DIRS_RELEASE "${range-v3_PACKAGE_FOLDER_RELEASE}/include")
set(range-v3_range-v3_range-v3-meta_LIB_DIRS_RELEASE "${range-v3_PACKAGE_FOLDER_RELEASE}/lib")
set(range-v3_range-v3_range-v3-meta_RES_DIRS_RELEASE )
set(range-v3_range-v3_range-v3-meta_DEFINITIONS_RELEASE )
set(range-v3_range-v3_range-v3-meta_OBJECTS_RELEASE )
set(range-v3_range-v3_range-v3-meta_COMPILE_DEFINITIONS_RELEASE )
set(range-v3_range-v3_range-v3-meta_COMPILE_OPTIONS_C_RELEASE "")
set(range-v3_range-v3_range-v3-meta_COMPILE_OPTIONS_CXX_RELEASE "/permissive-")
set(range-v3_range-v3_range-v3-meta_LIBS_RELEASE )
set(range-v3_range-v3_range-v3-meta_SYSTEM_LIBS_RELEASE )
set(range-v3_range-v3_range-v3-meta_FRAMEWORK_DIRS_RELEASE )
set(range-v3_range-v3_range-v3-meta_FRAMEWORKS_RELEASE )
set(range-v3_range-v3_range-v3-meta_DEPENDENCIES_RELEASE )
set(range-v3_range-v3_range-v3-meta_SHARED_LINK_FLAGS_RELEASE )
set(range-v3_range-v3_range-v3-meta_EXE_LINK_FLAGS_RELEASE )
# COMPOUND VARIABLES
set(range-v3_range-v3_range-v3-meta_LINKER_FLAGS_RELEASE
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${range-v3_range-v3_range-v3-meta_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${range-v3_range-v3_range-v3-meta_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${range-v3_range-v3_range-v3-meta_EXE_LINK_FLAGS_RELEASE}>
)
set(range-v3_range-v3_range-v3-meta_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${range-v3_range-v3_range-v3-meta_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${range-v3_range-v3_range-v3-meta_COMPILE_OPTIONS_C_RELEASE}>")