# boids

Birds are pretty cool. A flock of thousands moves like a single fluid organism, turning sharply, splitting around obstacles and reforming. No bird is in charge and no bird knows the plan and each one is just reacting to its immediate neighbors. Aaand somehow the whole thing looks alive.
In 1986, Craig Reynolds figured out the rules (apparently). He called his agents _boids_ and showed that three local forces applied every frame were enough to reproduce what we see in the sky. Kind of cool, uh?

Soo I wanted to build this from scratch. I thought, "if the birds are just vectors and the rules are just other vectors (like forces), you just add them up every frame and see what happens". So yeah, I wrote it.
![boids demo](demo.gif)

---

## how it works
Every boid has a position $p$ and a velocity $v$. At each frame, the engine queries the local neighborhood of each boid to compute three distinct steering vectors:

- **Separation:** Prevents crowding. For every neighbor within a minimal radius, compute a repulsive vector $p - p_{neighbor}$. Normalize it and scale it inversely by distance so closer boids push harder. Sum these up.
- **Alignment:** Matches the local heading. Compute the average velocity $v_{avg}$ of all neighbors in range. The steering force applied is simply $v_{avg} - v$.
- **Cohesion:** Keeps the flock together. Compute the center of mass $p_{avg}$ of the local neighborhood. The steering force is the vector pointing to that center: $p_{avg} - p$.

Each rule produces a vector. You weight them, add them up, and that sum becomes the acceleration for the curr frame:
$$a = (w_s * S) + (w_a * A) + (w_c * C)$$

Then velocity updates, position updates and you have to repeat that sixty times a second... and _voilà!_, you have a simulation running at 60fps!
This is a flock.


---

## controls

| Input | Action |
|---|---|
| Left click | Spawn a boid at cursor |
| Enter | Spawn 10 boids at center |
| Tab | Toggle controls panel |

The panel lets you move the separation, alignment and cohesion sliders in real time and watch the flock change. It's a good way to feel what each rule actually does.

---

## building

Dependencies: [SFML 3](https://www.sfml-dev.org/), [Dear ImGui](https://github.com/ocornut/imgui) (>= 1.91.1, < 1.92.0), [ImGui-SFML](https://github.com/SFML/imgui-sfml).

Download ImGui and ImGui-SFML manually and place the source files in the project root (they are excluded from the repo). Then compile:

```bash
g++ main.cpp imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp imgui-SFML.cpp \
    -o boids -I. \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lopengl32 \
    -fopenmp
```

---

## design & implementation notes
 
**Everything is a vector.** The first decision was to build a minimal `Vec2` struct from scratch instead of using SFML's built-in `sf::Vector2f`. This obviously forced me to think carefully about what operations I actually needed, such as dot product, normalization, magnitude; rather than just inheriting a black box. It also made the physics layer completely independent of the rendering layer, so that is cool.

**Mass = 1, so force = acceleration.** Each rule produces a steering force. The three forces are weighted and summed into a single acceleration vector per boid, per frame. Since all boids are assumed to have equal mass, Newton's second law simplifies cleanly because you just add the acceleration to the velocity, then the velocity to the position.
 
**Why O(n²) and not something smarter?** Each boid checks every other boid to find its neighbors. With *n* boids, that's *n²* comparisons per frame. A proper spatial partitioning structure like a grid or a quadtree would bring this down to roughly O(n log n). I kept the naive approach deliberately because it's easier to reason about correctness and high levelness (?), and for the simulation scales I was targeting it runs fine. OpenMP handles the rest.
 
**Parallelism with OpenMP.** The outer loop of each rule (iterating over boids) is embarrassingly parallel: the force on boid *i* depends only on reading the state of boid *j*, never on writing to it. Each thread gets its own local accumulator, writes to its own index in the force array, and there are no race conditions.
 
**The trail effect is a lie.** There are no stored positions, no history buffers. Each frame, before drawing any boid, a semi-transparent dark rectangle is drawn over the entire window. Previous frames fade out gradually as new ones pile on top. It looks just right and it was kinda easy to implement.
 
**Color encodes speed.** Boid color is mapped linearly from velocity magnitude to an RGB range: deep blue at low speed, bright cyan at high speed. This makes the simulation readable at a glance: tight, slow clusters read differently from fast, scattered individuals.
 
**The weights matter more than you'd think.** Tuning separation, alignment and cohesion by hand, and watching the flock collapse, scatter, or suddenly self-organize, is where the simulation becomes intuitive. That's why the ImGui panel exists: because changing values mid-simulation teaches you what each rule actually does. And that's fun.
