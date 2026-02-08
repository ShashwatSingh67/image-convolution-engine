Need VScode with G++ and Cmake installed ran on wsl


IN order to build the file:

cmake -S . -B build
cmake --build build

drag and drop the photos in to the data folder
to run the demo: this gets the build and passes in the img with a padding reflect

./build/imgconv --in data/name.jpg --out name.png --sigma 1.6 --ksize 11 --padding reflect


./build/imgconv --in data/wolf.jpg --out wolf_out.png --sigma 1.6 --ksize 11 --padding reflect


BUG TESTING:
ls -lah data - to see all images in data folder

