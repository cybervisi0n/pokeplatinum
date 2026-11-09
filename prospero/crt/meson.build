prospero_crt_lib = static_library('crt',
    pic: true,
    sources: files(
    'app_cpp_runtime.cpp',
    'app_crt.cpp',
    'kernel.c',
    'kernel_iommu.c',
    'kernel_procio.c',
    'klog.c',
    'mdbg.c',
    'nid.c',
    'patch.c',
    'rtld.c',
    'rtld_dlfcn.c',
    'rtld_payload.c',
    'rtld_so.c',
    'rtld_sprx.c',
    'syscall.c'
    ),
    c_args: [
        '-fms-extensions'
    ]
)
