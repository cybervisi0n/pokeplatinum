ps5_native_tool_srcs = files(
    'elf_object.cpp',
    'native_app_builder.cpp',
    'sce_module_writer.cpp',
    'self_container.cpp'
)


ps5_native_tool_exe = executable(
    'ps5-native-tool',
    sources: ps5_native_tool_srcs,
    cpp_args: [ '-std=c++20' ],
    dependencies: [],
    link_args: ['-lz'],
    native: true
)
