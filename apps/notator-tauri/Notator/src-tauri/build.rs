fn main() {
    tauri_build::build();

    // Path from src-tauri/ back to the repo root containing the notation CMakeLists.txt.
    let core_root = "../../../../";

    let dst = cmake::Config::new(core_root)
        .build_target("notation")
        .build();

    println!("cargo:rustc-link-search=native={}/build", dst.display());
    println!("cargo:rustc-link-lib=static=notation");

    cxx_build::bridge("src/ffi.rs")
        .include(format!("{core_root}/include"))
        .include("cxx")
        .file("cxx/bridge.cpp")
        .std("c++20")
        .compile("notator-bridge");

    println!("cargo:rerun-if-changed=src/ffi.rs");
    println!("cargo:rerun-if-changed=cxx/bridge.hpp");
    println!("cargo:rerun-if-changed=cxx/bridge.cpp");
    println!("cargo:rerun-if-changed={core_root}/include/notation/layout/score.hpp");
}
