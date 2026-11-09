libc_prx_builder_srcs = files(
    'libc_builder.cpp'
)


libc_prx_builder_exe = executable(
    'libc-prx-builder',
    sources: libc_prx_builder_srcs,
    cpp_args: [ '-std=c++20' ],
    dependencies: [],
    link_args: ['-lz'],
    native: true
)

libc_api_surface_txt = files('api-surface.txt')
libc_imports_txt = files('imports.txt')

libc_elf = custom_target(
    'libc.elf',
    output: 'libc.elf',
    command: [ libc_prx_builder_exe, 
               libc_api_surface_txt,
               libc_imports_txt,
               '@OUTPUT@' ]
)