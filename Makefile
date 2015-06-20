all: build

cmake:
	cmake -H. -Bbuild
	make --directory=./build/ --no-print-directory

build: cmake
	cp ./build/mdp ./mdp

clean:
	rm -rf ./build ./mdp

extract-cases:
	7z x ./testcases.7z -o./testcases

clean-cases:
	rm -rf ./testcases