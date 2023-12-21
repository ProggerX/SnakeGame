{
	inputs = {
		nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
	};

	outputs = {self, nixpkgs, ...}:
	let system = "x86_64-linux";
	pkgs = import nixpkgs { inherit system; };
	in {
		packages."${system}".default = pkgs.stdenv.mkDerivation {
			name = "snake";

			nativeBuildInputs = with pkgs; [gcc gnumake];
			buildInputs = with pkgs; [ncurses];

			buildPhase = "make";
			installPhase = "cp a.out $out";
			
		};
	};
}
