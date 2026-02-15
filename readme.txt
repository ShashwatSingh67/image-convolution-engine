Need VScode with G++ and Cmake installed just clone the repo and run on wsl

IN order to build the file:

cmake -S . -B build
cmake --build build

drag and drop the photos in to the data folder
in order to get gaussian blur effect run:

./build/imgconv --in data/name.jpg --out name.png --sigma 1.6 --ksize 11 --padding reflect

for example:

./build/imgconv --in data/wolf.jpg --out wolf_out.png --sigma 1.6 --ksize 11 --padding reflect


FYI:
--in data/name.jpg (name of the jpg saved in data folder)
--out name.png (name of output file)
--sigma 1.6 (gaussian standard deviation controls the blur strenght)
--ksize 11 (kernel size, must be odd, the kernel is basically the grid that the convolution uses to calculated what amount of amount of neighbouring vals are used to perform blur 
            higher ksize increases radius, increasing blur and vice versa)
--padding reflect
    reflect: (mirrors pixels at edge)
--padding zero
    zero: (performs gaussian blur with outside image vals which are zero creating dark blur on edges)


