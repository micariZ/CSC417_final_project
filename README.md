# CSC417_final_project
## build
I was able to build the project on Linux 18.04 and windows 10 with cmake version `3.10.2`,`3.18.3` gcc version `7.5`, Visual Studio 16 2019

Clone this repository https://github.com/micariZ/CSC417_final_project.git using the `--recursive` flag, or run `git submodule update --init --recursive`. I include the source code in the zip as well just incase, but if you want to use the zip, make sure to create a dir called extern and place libigl(https://github.com/libigl/libigl.git) under there.

run the following command:
 
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
    ./final-project scene1

final-project takes one argument. `scene1` simulates a snowman hit by a snowball. `scene2` and `scene3` simulates two snowballs smashing with different speed.

## Demo Video
I put the demo video here(avi):  https://drive.google.com/file/d/1NyvO8dm2BuWeS7N06rSulBNg1_eI_6Kz/view?usp=sharing

and a back up copy(mp4): https://drive.google.com/file/d/16S8yVG1dx5OqginWxCDrkXeFS2kDFLiu/view?usp=sharing

## change parameter
All settings are stored in `includes/SimConstants.h`. Change them can change snow properties(chunkiness, hardness etc), but you need to rebuild the project to see the change. Also some settings can cause the program to crush, so try change in small quantity. I will put the values I used below.

`CRIT_COMPRESS`:  small critical compress gives wet snow that looks sticky. Reference: `2.5e-2`. For lower value try number aourd `1.8e-2`. For higher value try `3.1e-2` (Note: don't remove the 1-, just change the second part).

`CRIT_STRETCH`:   small critical stretch gives smooth, sand like snow. Reference: `7.5e-3`. For lower value try number aourd `5.0e-3`. For higher value try number aourd `9.5e-3` (Note: don't remove the 1+, just change the second part).

`HARDENING`:  small hardening gives muddy snow that doesn't break easily. Reference: `5`. For lower value try number aourd `3`. For higher value try number less than 9 (`10` crushed program)
