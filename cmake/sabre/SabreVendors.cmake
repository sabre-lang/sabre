# --  MODULE PROPERTIES  -- #

# Expose some global properties here
set_property(GLOBAL PROPERTY SABRE_VENDOR_VERSIONS
    "X(\"${SABRE_TARGET_SUPER}\",\"${SABRE_VERSION_SHORT}\")")

# --  PUBLIC METHODS  -- #

# Allows requesting all the vendors
function(sabre_vendors_require)
    # get the boost vendor name as well
    get_property(_boost GLOBAL PROPERTY __MONO_BOOST_TARGET)

    foreach (_target IN LISTS ARGN)
        if (${_target} STREQUAL ${_boost})
            __sabre_vendors_boost()
        else ()
            if (${_target} STREQUAL "LIEF")
                __sabre_vendors_lief()
            elseif (${_target} STREQUAL "asmjit")
                __sabre_vendors_asmjit()
            endif ()

            # and require the incoming item now
            mono_vendor_require(${_target})

            if (${_target} STREQUAL "spdlog")
                __sabre_vendors_spdlog()
            endif ()
        endif ()
    endforeach ()
endfunction()

function(sabre_vendors_inject _target)
    # attempt bundling the vendors together
    get_property(_vendors GLOBAL PROPERTY SABRE_VENDOR_VERSIONS)
    list(JOIN _vendors " " _vendors) # join our values now
    string(TOUPPER ${_target} _upper)
    set(_template
        "#ifndef _${_upper}_VENDORS_VERSIONS_HPP"
        "#define _${_upper}_VENDORS_VERSIONS_HPP"
        ""
        "//  X-MACROS  //"
        ""
        "#define ${_upper}_XX_VENDOR_VERSIONS(X) ${_vendors}"
        ""
        "#endif"
    )

    list(JOIN _template "\n" _template) # write the necessary output to be used now
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/autogen/include/${_target}/vendors/versions.hpp" ${_template})
endfunction()

# --  PRIVATE METHODS  -- #

# Handles fetching necessary vendors
function(__sabre_vendors_declare _target _vtag)
    # get the underlying boost name
    get_property(_boost GLOBAL PROPERTY __MONO_BOOST_TARGET)

    # attempt resolving our vendors now
    if (_target STREQUAL ${_boost})
        mono_boost_declare(${_vtag} ${ARGN})
    else ()
        mono_vendor_declare(${_target} ${_vtag} ${ARGN})
    endif ()

    # attempt parsing the base version value now
    __mono_vendor_version(${_vtag} _version)

    # append the vendor to the available list now
    get_property(_vendors GLOBAL PROPERTY SABRE_VENDOR_VERSIONS)
    list(APPEND _vendors "X(\"${_target}\",\"${_version}\")")
    list(SORT _vendors COMPARE STRING CASE INSENSITIVE)
    set_property(GLOBAL PROPERTY SABRE_VENDOR_VERSIONS ${_vendors})
endfunction()

# Allow requesting in "boost" libraries
macro(__sabre_vendors_boost)
    # Prepare boost features
    mono_boost_static(ON)
    mono_boost_multithreaded(ON)
    mono_boost_libraries(context interprocess locale)

    # ensure we ignore using ICU for locale
    set(BOOST_LOCALE_ENABLE_ICU OFF)

    # and require boost as necessary
    mono_boost_require()
endmacro()

# Handles setting required "asmjit" options
macro(__sabre_vendors_asmjit)
    set(ASMJIT_EMBED OFF)
    set(ASMJIT_STATIC ON)
endmacro()

# Handles setting required "LIEF" options
macro(__sabre_vendors_lief)
    set(LIEF_C_API OFF)
    set(LIEF_EXAMPLES OFF)
    set(LIEF_USE_CCACHE OFF)
    set(LIEF_LOGGING OFF)
    set(LIEF_LOGGING_DEBUG OFF)
    set(LIEF_ENABLE_JSON OFF)
    set(LIEF_EXTERNAL_SPDLOG ON)

    set(LIEF_DEX OFF)
    set(LIEF_ART OFF)
endmacro()

# Handles setting required "spdlog" options
macro(__sabre_vendors_spdlog)
    target_compile_definitions(spdlog PUBLIC
        "SPDLOG_SHORT_LEVEL_NAMES={ \"T\", \"D\", \"I\", \"W\", \"E\", \"F\", \"O\" }"
        "SPDLOG_LEVEL_NAMES={ \"TRACE\", \"DEBUG\", \"INFO \", \"WARN \", \"ERROR\", \"FATAL\", \"OFF\" }"
    )
endmacro()

# --  MODULE DEFINITION  -- #

# We require ensuring some items are installed
set(FMT_INSTALL ON)
set(SPDLOG_INSTALL ON)
set(SPDLOG_FMT_EXTERNAL ON)

# Declare all the necessary repositories
__sabre_vendors_declare(ankerl v4.9.2 REPO "martinus/unordered_dense.git")
__sabre_vendors_declare(asmjit v1.21.0 REPO "asmjit/asmjit.git" TAG master)
__sabre_vendors_declare(aster v1.1.3 REPO "rroessler/aster.git")
__sabre_vendors_declare(CLI11 v2.7.2 REPO "CLIUtils/CLI11.git")
__sabre_vendors_declare(fmt 12.2.0 REPO "fmtlib/fmt.git")
__sabre_vendors_declare(function2 4.2.5 REPO "naios/function2.git")
__sabre_vendors_declare(glaze v8.1.0 REPO "stephenberry/glaze.git")
__sabre_vendors_declare(lief 1.0.0 REPO "lief-project/LIEF.git")
__sabre_vendors_declare(muuid v2.5.0 REPO "gershnik/modern-uuid.git" TAG v2.3)
__sabre_vendors_declare(spdlog v1.17.0 REPO "gabime/spdlog.git")

# Also expose "boost" as a vendor
__sabre_vendors_declare(boost 1.92.0 REPO "boostorg/boost.git.git" TAG 1.92.0)
