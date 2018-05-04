#Use an official Arch Linux image as a parent image
FROM archlinux/base

#Set the working directory to /app
WORKDIR /app

#Install any needed packages specified in requirements.txt
RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm base-devel cmake qt5 glew glm assimp bullet ninja clang

#Copy the current directory contents into the container at /app
ADD . /app

#Define environment variables
ENV CC clang
ENV CXX clang++

#Start the build when the container launches
CMD mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE="RelWithDebInfo" -DCMAKE_INSTALL_PREFIX="install" -G "Ninja" .. && ninja && ninja install && cp -r install installnodeps && ninja installprerequisites && cp -r install installwithdeps

