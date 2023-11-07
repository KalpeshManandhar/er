# er
A software renderer

## Finished
- rasterization (edge function)
- interpolation (using barycentric coordinates)
- depth testing (simple z buffer)
- projection (perspective)
- visible surface detection
- texture sampling (wrap around + nearest pixels)
- some way to add shader globals (shader classes with static variables and shader functions)
- generalize    -vertex shader (with a new class derived from Shader)
                -pixel shader

## In progress
- .obj loader

## TODO: 
- perspective correct interpolation
- binary space partitioning?
- default shaders
- some way to generalize points
- multithreading triangle rasterization using thread pools
- SIMD in rasterization