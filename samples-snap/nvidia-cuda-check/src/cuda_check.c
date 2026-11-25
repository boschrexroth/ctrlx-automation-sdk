/*
 * Minimal CUDA access check using the CUDA driver API.
 * libcuda.so.1 is loaded at runtime from the host driver, so no CUDA
 * toolkit or headers are needed to build this.
 */
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int CUresult;
typedef int CUdevice;
typedef struct CUctx_st *CUcontext;
typedef unsigned long long CUdeviceptr;

#define LOAD(name)                                              \
    name##_t name = (name##_t)dlsym(lib, #name);                \
    if (!name) { fprintf(stderr, "missing symbol %s\n", #name); return 1; }

typedef CUresult (*cuGetErrorName_t)(CUresult, const char **);
static cuGetErrorName_t get_error_name;

/* Show which device node cannot be opened (device cgroup/AppArmor/permissions). */
static void probe_devices(void)
{
    static const char *nodes[] = {
        "/dev/nvidiactl", "/dev/nvidia0", "/dev/nvidia-uvm", "/dev/nvidia-uvm-tools"
    };
    fprintf(stderr, "Probing device nodes:\n");
    for (unsigned i = 0; i < sizeof(nodes) / sizeof(nodes[0]); i++) {
        int fd = open(nodes[i], O_RDWR);
        fprintf(stderr, "  %-24s %s\n", nodes[i], fd >= 0 ? "open OK" : strerror(errno));
        if (fd >= 0)
            close(fd);
    }
}

#define CHECK(call)                                             \
    do {                                                        \
        CUresult r_ = (call);                                   \
        if (r_ != 0) {                                          \
            const char *n_ = "unknown";                         \
            if (get_error_name) get_error_name(r_, &n_);        \
            fprintf(stderr, "FAIL: %s returned %d (%s)\n", #call, r_, n_); \
            probe_devices();                                    \
            return 1;                                           \
        }                                                       \
    } while (0)

typedef CUresult (*cuInit_t)(unsigned int);
typedef CUresult (*cuDriverGetVersion_t)(int *);
typedef CUresult (*cuDeviceGetCount_t)(int *);
typedef CUresult (*cuDeviceGet_t)(CUdevice *, int);
typedef CUresult (*cuDeviceGetName_t)(char *, int, CUdevice);
typedef CUresult (*cuDeviceGetAttribute_t)(int *, int, CUdevice);
typedef CUresult (*cuDeviceTotalMem_t)(size_t *, CUdevice);
typedef CUresult (*cuCtxCreate_t)(CUcontext *, unsigned int, CUdevice);
typedef CUresult (*cuCtxDestroy_t)(CUcontext);
typedef CUresult (*cuMemAlloc_t)(CUdeviceptr *, size_t);
typedef CUresult (*cuMemFree_t)(CUdeviceptr);
typedef CUresult (*cuMemcpyHtoD_t)(CUdeviceptr, const void *, size_t);
typedef CUresult (*cuMemcpyDtoH_t)(void *, CUdeviceptr, size_t);

int main(void)
{
    void *lib = dlopen("libcuda.so.1", RTLD_NOW);
    if (!lib) {
        fprintf(stderr, "Cannot load libcuda.so.1: %s\n", dlerror());
        return 1;
    }

    get_error_name = (cuGetErrorName_t)dlsym(lib, "cuGetErrorName");
    LOAD(cuInit)
    LOAD(cuDriverGetVersion)
    LOAD(cuDeviceGetCount)
    LOAD(cuDeviceGet)
    LOAD(cuDeviceGetName)
    LOAD(cuDeviceGetAttribute)
    LOAD(cuDeviceTotalMem)
    LOAD(cuCtxCreate)
    LOAD(cuCtxDestroy)
    LOAD(cuMemAlloc)
    LOAD(cuMemFree)
    LOAD(cuMemcpyHtoD)
    LOAD(cuMemcpyDtoH)

    CHECK(cuInit(0));

    int ver = 0, count = 0;
    CHECK(cuDriverGetVersion(&ver));
    CHECK(cuDeviceGetCount(&count));
    printf("CUDA driver version: %d.%d\n", ver / 1000, (ver % 1000) / 10);
    printf("CUDA devices: %d\n", count);
    if (count == 0)
        return 1;

    for (int i = 0; i < count; i++) {
        CUdevice dev;
        char name[256];
        int major = 0, minor = 0;
        size_t mem = 0;
        CHECK(cuDeviceGet(&dev, i));
        CHECK(cuDeviceGetName(name, sizeof(name), dev));
        CHECK(cuDeviceGetAttribute(&major, 75, dev)); /* COMPUTE_CAPABILITY_MAJOR */
        CHECK(cuDeviceGetAttribute(&minor, 76, dev)); /* COMPUTE_CAPABILITY_MINOR */
        CHECK(cuDeviceTotalMem(&mem, dev));
        printf("  [%d] %s, compute %d.%d, %zu MiB\n", i, name, major, minor, mem >> 20);
    }

    /* Round-trip data through GPU memory to prove real access. */
    CUdevice dev;
    CUcontext ctx;
    CHECK(cuDeviceGet(&dev, 0));
    CHECK(cuCtxCreate(&ctx, 0, dev));

    enum { N = 1024 };
    unsigned char in[N], out[N];
    for (int i = 0; i < N; i++)
        in[i] = (unsigned char)i;
    memset(out, 0, sizeof(out));

    CUdeviceptr d;
    CHECK(cuMemAlloc(&d, N));
    CHECK(cuMemcpyHtoD(d, in, N));
    CHECK(cuMemcpyDtoH(out, d, N));
    CHECK(cuMemFree(d));
    CHECK(cuCtxDestroy(ctx));

    if (memcmp(in, out, N) != 0) {
        fprintf(stderr, "FAIL: GPU memory round trip mismatch\n");
        return 1;
    }
    printf("GPU memory round trip: OK\n");
    return 0;
}
