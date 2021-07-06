# jpg-loaders-comparison

libjpeg-turbo installation: `sudo apt-get install libjpeg-turbo8-dev libjpeg8-dev libturbojpeg0-dev`
build: `meson build ; ninja -C build`
run: `build/main`

Result:
```
loadRichgel(100) time 9.529s
loadTurbo(100) time 0.906s
```
