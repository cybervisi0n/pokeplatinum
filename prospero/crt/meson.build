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
    'memstream.c',
    'nid.c',
    'patch.c',
    'posix_fallocate.c',
    'rtld_stub.c',
    'syscall.c'
    ),
    c_args: [
        '-fms-extensions'
    ]
)
