#!/bin/bash
docker run \
  --rm \
  -v $(pwd):/src \
  -u $(id -u):$(id -g) \
  emscripten/emsdk \
  emcc led_control.cpp -o led_control.js \
   -s WASM=1 -s EXPORTED_FUNCTIONS='["_set_config", "_execute"]' -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'