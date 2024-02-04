{
  lib,
  stdenv,
  hyprland,
}:
stdenv.mkDerivation {
  pname = "hych";
  version = "0.1";
  src = ./.;

  inherit (hyprland) nativeBuildInputs;

  buildInputs = [hyprland] ++ hyprland.buildInputs;

  meta = with lib; {
    homepage = "https://github.com/DreamMaoMao/hych";
    description = "Hyprland Plugin for minimizing windows";
    license = licenses.mit;
    platforms = platforms.linux;
  };
}
