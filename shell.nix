{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.gcc
    pkgs.sfml
    pkgs.libglvnd.dev
  ];

  # Set the C++ flags, pointing directly to the sfml path
  CXXFLAGS = "-I${pkgs.sfml}/include -I${pkgs.libglvnd.dev}/include";
  LDFLAGS = "-L${pkgs.sfml}/lib";
}

