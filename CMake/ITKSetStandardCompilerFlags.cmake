# trict-null-sentinel Check the set of common warning flags supported by C and C++ compilers
# check_compiler_warning_flags(<c_flags_var> <cxx_flags_var>)
#  <c_flags_var> - variable to store valid C warning flags
#  <cxx_flags_var> - variable to store valid CXX warning flags
# This internally calls the check_c_compiler_flag and check_cxx_compiler_flag macros.


# To create a portable build system, it is best to not
# test for platforms, but to test for features.
#
# Instead of testing "if Windows then do this", test for
# "if the -Wno-invalid-offsetof flag works then use it".
#
# Typical use of this module is:
#
#  include(CheckCompilerWarningFlags)
#  check_compiler_warning_flags(C_WARNING_FLAGS CXX_WARNING_FLAGS)
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_WARNING_FLAGS}")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNING_FLAGS}")


include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

function(check_c_compiler_warning_flags c_flag_var)
  set(local_c_flags "")
  set(flag_list "${ARGN}")
  foreach(flag IN LISTS flag_list)
    CHECK_C_COMPILER_FLAG(${flag} C_HAS_WARNING${flag})
    if(${C_HAS_WARNING${flag}})
      set(local_c_flags "${local_c_flags} ${flag}")
    endif()
  endforeach()
  set(${c_flag_var} "${local_c_flags}" PARENT_SCOPE)
endfunction()


function(check_cxx_compiler_warning_flags cxx_flag_var)
  set(local_cxx_flags "")
  set(flag_list "${ARGN}")
  foreach(flag IN LISTS flag_list)
    CHECK_CXX_COMPILER_FLAG(${flag} CXX_HAS_WARNING${flag})
    if(${CXX_HAS_WARNING${flag}})
      set(local_cxx_flags "${local_cxx_flags} ${flag}")
    endif()
  endforeach()
  set(${cxx_flag_var} "${local_cxx_flags}" PARENT_SCOPE)
endfunction()


function(check_compiler_warning_flags c_warning_flags_var cxx_warning_flags_var)
  set(${c_warning_flags_var} "" PARENT_SCOPE)
  set(${cxx_warning_flags_var} "" PARENT_SCOPE)

  # Check this list on C compiler only
  set(c_flags
    -Wno-uninitialized
    -Wno-unused-parameter
  )

  ## On windows, the most verbose compiler options
  ## is reporting 1000's of wanings in windows
  ## header files, for now, limit the number of
  ## warnings to level 3
  if( WIN32 )
    set(VerboseWarningsFlag -W3 )
    ## A better solution would be to use -Wall,
    ## and then disable warnings one by one
    ## set(VerboseWarningsFlag -Wall -wd4820 -wd4682 )
  else()
    set(VerboseWarningsFlag -Wall )
  endif()

  # Check this list on both C and C++ compilers
  set(c_and_cxx_flags
    ${VerboseWarningsFlag}
    -Wno-long-double        #Needed on APPLE
    -Wcast-align
    -Wdisabled-optimization
    -Wextra
    -Wformat=2
    -Winvalid-pch
    -Wno-format-nonliteral
    -Wpointer-arith
    -Wshadow
    -Wunused
    -Wwrite-strings
    -funit-at-a-time
  )

  # Check this list on C++ compiler only
  set(cxx_flags
    -Wno-deprecated
    -Wno-invalid-offsetof
    -Woverloaded-virtual
    -Wstrict-null-sentinel
  )
##-Wno-c++0x-static-nonintegral-init
    ## Clang compiler likes to warn about this feature that is technically only in
    ## c++0x, but works on many compilers, and if it fails, then alternate methods are used

  check_c_compiler_warning_flags(CMAKE_C_WARNING_FLAGS ${c_flags} ${c_and_cxx_flags})
  check_cxx_compiler_warning_flags(CMAKE_CXX_WARNING_FLAGS ${c_and_cxx_flags} ${cxx_flags})

  set(${c_warning_flags_var} "${CMAKE_C_WARNING_FLAGS}" PARENT_SCOPE)
  set(${cxx_warning_flags_var} "${CMAKE_CXX_WARNING_FLAGS}" PARENT_SCOPE)
endfunction()


macro(check_compiler_platform_flags)
  # On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
  # warnings
  if(WIN32)
       if(NOT MINGW)
         if(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
           add_definitions(
             -D_CRT_FAR_MAPPINGS_NO_DEPRECATE
             -D_CRT_IS_WCTYPE_NO_DEPRECATE
             -D_CRT_MANAGED_FP_NO_DEPRECATE
             -D_CRT_NONSTDC_NO_DEPRECATE
             -D_CRT_SECURE_NO_DEPRECATE
             -D_CRT_SECURE_NO_DEPRECATE_GLOBALS
             -D_CRT_SETERRORMODE_BEEP_SLEEP_NO_DEPRECATE
             -D_CRT_TIME_FUNCTIONS_NO_DEPRECATE
             -D_CRT_VCCLRIT_NO_DEPRECATE
             -D_SCL_SECURE_NO_DEPRECATE
             )
         endif(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
       endif(NOT MINGW)
  endif(WIN32)

  if(WIN32)
    # Some libraries (e.g. vxl libs) have no dllexport markup, so we can
    # build full shared libraries only with the GNU toolchain. For non
    # gnu compilers on windows, only Common is shared.  This allows for
    # plugin type applications to use a dll for ITKCommon which will contain
    # the static for Modified time.
    if(CMAKE_COMPILER_IS_GNUCXX)
      # CMake adds --enable-all-exports on Cygwin (since Cygwin is
      # supposed to be UNIX-like), but we need to add it explicitly for
      # a native windows build with the MinGW tools.
      if(MINGW)
        set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS
          "-shared -Wl,--export-all-symbols -Wl,--enable-auto-import")
        set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS
          "-shared -Wl,--export-all-symbols -Wl,--enable-auto-import")
        set(CMAKE_EXE_LINKER_FLAGS "-Wl,--enable-auto-import")
      endif(MINGW)
    else(CMAKE_COMPILER_IS_GNUCXX)
      if(BUILD_SHARED_LIBS)
        set(ITK_LIBRARY_BUILD_TYPE "SHARED")
      else(BUILD_SHARED_LIBS)
        set(ITK_LIBRARY_BUILD_TYPE "STATIC")
      endif(BUILD_SHARED_LIBS)
      set(BUILD_SHARED_LIBS OFF)
    endif(CMAKE_COMPILER_IS_GNUCXX)
  endif(WIN32)
  #-----------------------------------------------------------------------------
  #ITK requires special compiler flags on some platforms.
  if(CMAKE_COMPILER_IS_GNUCXX)
   if(APPLE)
     option(ITK_USE_64BITS_APPLE_TRUNCATION_WARNING "Turn on warnings on 64bits to 32bits truncations." OFF)
     mark_as_advanced(ITK_USE_64BITS_APPLE_TRUNCATION_WARNING)

     execute_process(COMMAND "${CMAKE_C_COMPILER}" --version
       OUTPUT_VARIABLE _version ERROR_VARIABLE _version)

     # -fopenmp breaks compiling the HDF5 library in shared library mode
     # on the OS X platform -- at least with gcc 4.2 from XCode.
     set(compile_flag_lists CMAKE_C_FLAGS CMAKE_CXX_FLAGS
       CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_MINSIZEREL
       CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO
       CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_MINSIZEREL
       CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELWITHDEBINFO)
     foreach(listname ${compile_flag_lists})
       if("${${listname}}" MATCHES ".*-fopenmp.*")
         string(REPLACE "-fopenmp" "" tmpFlags "${${listname}}")
         set(${listname} "${tmpFlags}")
         message("-fopenmp causes incorrect compliation of HDF, removing from ${listname}")
       endif()
     endforeach(listname ${compile_flag_lists})

   endif(APPLE)

   # gcc must have -msse2 option to enable sse2 support
   if(VNL_CONFIG_ENABLE_SSE2 OR VNL_CONFIG_ENABLE_SSE2_ROUNDING)
     set(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -msse2")
   endif(VNL_CONFIG_ENABLE_SSE2 OR VNL_CONFIG_ENABLE_SSE2_ROUNDING)

  endif(CMAKE_COMPILER_IS_GNUCXX)

  #-----------------------------------------------------------------------------

  # for the gnu compiler a -D_PTHREADS is needed on sun
  # for the native compiler a -mt flag is needed on the sun
  if(CMAKE_SYSTEM MATCHES "SunOS.*")
    if(CMAKE_COMPILER_IS_GNUCXX)
      set(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -D_PTHREADS")
      set(ITK_REQUIRED_LINK_FLAGS "${ITK_REQUIRED_LINK_FLAGS} -lrt")
    else()
      set(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -mt")
      set(ITK_REQUIRED_C_FLAGS "${ITK_REQUIRED_C_FLAGS} -mt")
    endif()
    # Add flags for the SUN compiler to provide all the methods for std::allocator.
    #
    CHECK_CXX_SOURCE_COMPILES("-features=no%anachronisms" SUN_COMPILER)
    if(SUN_COMPILER)
      CHECK_CXX_SOURCE_COMPILES("-library=stlport4" SUN_COMPILER_HAS_STL_PORT_4)
      if(SUN_COMPILER_HAS_STL_PORT_4)
        set(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -library=stlport4")
      endif(SUN_COMPILER_HAS_STL_PORT_4)
     endif(SUN_COMPILER)
  endif()

  # mingw thread support
  if(MINGW)
    set(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -mthreads")
    set(ITK_REQUIRED_C_FLAGS "${ITK_REQUIRED_C_FLAGS} -mthreads")
    set(ITK_REQUIRED_LINK_FLAGS "${ITK_REQUIRED_LINK_FLAGS} -mthreads")
  endif()


  #-----------------------------------------------------------------------------
  # The frename-registers option does not work due to a bug in the gnu compiler.
  # It must be removed or data errors will be produced and incorrect results
  # will be produced.  This is first documented in the gcc4 man page.
  if(CMAKE_COMPILER_IS_GNUCXX)
    set(ALL_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_MODULE_LINKER_FLAGS}" )
    separate_arguments(ALL_FLAGS)
    foreach(COMP_OPTION ${ALL_FLAGS})
      if("${COMP_OPTION}" STREQUAL "-frename-registers")
        message(FATAL_ERROR "-frename-registers causes runtime bugs.  It must be removed from your compilation options.")
      endif("${COMP_OPTION}" STREQUAL "-frename-registers")
    endforeach(COMP_OPTION)
  endif(CMAKE_COMPILER_IS_GNUCXX)

  #-----------------------------------------------------------------------------
  # Set the compiler-specific flag for disabling optimization.
  if(MSVC)
    set(ITK_CXX_DISABLE_OPTIMIZATION_FLAG "/Od")
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(GNU|Intel)$")
    set(ITK_CXX_DISABLE_OPTIMIZATION_FLAG "-O0")
  endif()
  if(DEFINED ITK_CXX_DISABLE_OPTIMIZATION_FLAG)
    CHECK_CXX_SOURCE_COMPILES(${ITK_CXX_DISABLE_OPTIMIZATION_FLAG} CXX_HAS_DISABLE_OPTIMIZATION_FLAG)
  endif()
endmacro()#End the platform check function


#-----------------------------------------------------------------------------
#Check the set of warning flags the compiler supports
check_compiler_warning_flags(C_WARNING_FLAGS CXX_WARNING_FLAGS)

# Append ITK warnings to the CMake flags.
# We do not set them in ITK_REQUIRED FLAGS because all project which
# use ITK don't require these flags .
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNING_FLAGS}")

#-----------------------------------------------------------------------------
#Check the set of platform flags the compiler supports
check_compiler_platform_flags()
