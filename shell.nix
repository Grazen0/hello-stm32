{
  pkgs ? import <nixpkgs> { },
}:
pkgs.mkShell {
  packages = with pkgs; [
    bear
    clang-tools
    gcc-arm-embedded
    glibc_multi # Provides gnu/stubs-32.h
    openocd
    usbutils
    xxd
  ];
}
