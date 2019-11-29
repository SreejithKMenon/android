# Defines the root to all other relative paths
    # The macro function my-dir, provided by the build system,
    # specifies the path of the current directory (i.e. the
    # directory containing the Android.mk file itself)
    LOCAL_PATH := $(call my-dir)

    # Clear all LOCAL_XXX variables with the exception of
    # LOCAL_PATH (this is needed because all variables are global)
    include $(CLEAR_VARS)

    # List all of our C/C++ files to be compiled (header file
    # dependencies are automatically computed)
    LOCAL_SRC_FILES := ndp-wrapper.cpp
    FILE_LIST := $(wildcard $(LOCAL_PATH)/ndp_codebase/src/*.cpp)
    LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

    FILE_LIST := $(wildcard $(LOCAL_PATH)/ndp_codebase/submodules/json-cpp/src/json/*.cpp)
    LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)


    # The name of our shared module (this name will be prepended
    # by lib and postfixed by .so)
    LOCAL_MODULE := ndp-detector2

    LOCAL_C_INCLUDES := $(LOCAL_PATH)/ndp_codebase/include
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/ndp_codebase/submodules/json-cpp/include/
    LOCAL_CPPFLAGS := -fexceptions -DNDEBUG -O2 -fopenmp
    LOCAL_CFLAGS := -DNDEBUG -O2 -fopenmp
    #COMMON_CFLAGS := -I$(LOCAL_PATH)/ndp_codebase/include


    # We need to tell the linker about our use of the liblog.so
    LOCAL_LDLIBS := -lm -llog -latomic -landroid -ljnigraphics -fopenmp

    # Collects all LOCAL_XXX variables since "include $(CLEAR_VARS)"
    #  and determines what to build (in this case a shared library)
    include $(BUILD_SHARED_LIBRARY)
