===
Modifed for MingW G++ 5.3

add following option to CMakeLists.txt

 SET_TARGET_PROPERTIES(archive_static PROPERTIES COMPILE_FLAGS "-Wno-error -D_SSIZE_T_DEFINED")