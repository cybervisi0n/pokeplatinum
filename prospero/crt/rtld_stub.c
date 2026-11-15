// Stub functions for rtld since PS5 native apps don't currently support them'

void* __dlopen(const char *filename, int flags) {
    return (void*)0;
}

void* __dlsym(void *handle, const char *symbol) {
    return (void*)0;
}

int __dladdr(void *addr, void *info) {
    return 0;
}

int __dlclose(void *handle) {
    return 0;
}

char * __dlerror(void) {
    return "Not implemented";
}