all: build

cmake:
	cmake -H. -Bbuild
	make --directory=./build/ --no-print-directory

build: cmake
	cp ./build/mdp ./mdp

clean:
	rm -rf ./build ./mdp
