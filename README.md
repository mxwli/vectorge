# VectorGE

Rewrite of old 2d vector based game I wrote in highschool.

Instead of olc::PixelGameEngine, I'll be using raylib, and instead of C++, I'll be using C

If you take a look at the code, you'll find most function definitions have the following comment:
`//testing: [none/OK/good]`
- **none** means the function was never tested
- **OK** means the function was used, and appears to be OK
- **good** means the function is almost certainly not going to be a source of a bug

NOTE:
currently the implementation of node.c is extremely suspicious. I will need to fix it **immediately**

TODO:
- optimize node.c
- optimize game.c
