# Ray Tracer

**Ray-tracing application with triangle meshes, complex lighting and scenes.**

Developed in c++ with stb_image library.

**Target Platform**: Linux

**Usage:** 

- Run make file.
- ./ray scene.txt (bvh) (parallel)
- use flag "t" to enable bvh and parallel



### Work result

**Refraction:** Before/After

<img src=".\spheres4.png" alt=" spheres5" style="zoom: 33%;" />

<img src=".\spheres5.png" alt=" spheres5" style="zoom: 33%;" />



**Jittered Supersampling:** Before/After

<img src=".\gear.png" alt=" gear" style="zoom: 33%;" /><img src="./gear2.png" alt=" gear2" style="zoom: 33%;" />

**Speedup:**

- Parallelism: OpenMP used.
- Geometric grouping structure: Bounding volume hierarchy tree structure adopted to reduce the time complexity in ray-object collision detection to the logarithmic of total objects in the scene.