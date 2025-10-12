import os
import platform
import subprocess
import shutil

def copy_windows_dlls(build_dir):
    mingw_bin = os.environ.get("MINGW_PREFIX", "C:/msys64/mingw64/bin")
    dlls = [
        "libcurl-4.dll",
        "libssl-3.dll",
        "libssl-3-x64.dll",
        "libcrypto-3.dll",
        "libcrypto-3-x64.dll",
        "zlib1.dll",
        "libzstd.dll",
        "libwinpthread-1.dll",
        "libidn2-0.dll",
        "libbrotlidec.dll",
        "libbrotlicommon.dll",
        "libnghttp2-14.dll",
        "libnghttp3-9.dll",
        "libngtcp2-16.dll",
        "libngtcp2_crypto_ossl-0.dll",
        "libpsl-5.dll",
        "libssh2-1.dll",
        "libiconv-2.dll",
        "libintl-8.dll",
        "libunistring-5.dll"
    ]
    for dll in dlls:
        dll_path = os.path.join(mingw_bin, dll)
        if os.path.exists(dll_path):
            shutil.copy2(dll_path, build_dir)

def buildBlush(src, args=None):
    cpp_files = []
    for root, _, files in os.walk(src):
        for f in files:
            if f.endswith(".cpp"):
                cpp_files.append(os.path.join(root, f))

    build_dir = os.path.join(src, "..", "build", "debug-x64")
    os.makedirs(build_dir, exist_ok=True)

    system_platform = platform.system()
    if system_platform == "Windows":
        build_path = os.path.join(build_dir, "blush.exe")
    else:
        build_path = os.path.join(build_dir, "blush")

    compile_cmd = ["g++", "-std=c++23"]
    if system_platform == "Windows":
        compile_cmd += ["-static-libgcc", "-static-libstdc++"]
        from compile_icon import compile_icon
        res_file = compile_icon()
    else:
        res_file = None

    compile_cmd += cpp_files
    if res_file:
        compile_cmd.append(res_file)

    compile_cmd += ["-o", build_path]

    if system_platform == "Windows":
        compile_cmd += ["-LC:/msys64/mingw64/lib", "-lcurl", "-lssl", "-lcrypto", "-lz", "-lws2_32", "-lwinmm", "-lbcrypt", "-lcrypt32"]

    print(f"Compiling for {system_platform}...")
    subprocess.run(compile_cmd, check=True)

    if res_file:
        os.remove(res_file)

    if system_platform == "Windows":
        copy_windows_dlls(build_dir)

    print(f"Build finished: {build_path}")
    return build_path
