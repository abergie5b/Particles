# Particles
a particle emitter optimization project

Techniques:

- SIMD for vector and matrix operations
- custom memory allocator using placement_new
- proxy pattern for matrix multiplication
- custom SSE rand() function
- return value optimization

<img a="data/screenshot.PNG" width=400 height=250>

Running with up to 200k particles with max life of 5 seconds and spawn frequency of 0.00005 produces an update time of ~5ms with most of the total time spent on rendering.

<img a="data/screenshot2.PNG" width=400 height=250>
