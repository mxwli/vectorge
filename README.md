# VectorGE

Rewrite of old 2d vector based game I wrote in highschool.

Instead of olc::PixelGameEngine, I'll be using raylib, and instead of C++, I'll be using C

If you take a look at the code, you'll find most function definitions have the following comment:
`//testing: [none/OK/good]`
- **none** means the function was never tested
- **OK** means the function was used, and appears to be OK
- **good** means the function is almost certainly not going to be a source of a bug


the individual implementation of:
- game.c
- node.c
- advvec.c

need to be optimized in the future




NOTE: the backbone of this engine - the node tree - is massively overengineered and I suspect it significantly slows down performance.

It was built in a way that was optimizable, so in the future, there are two things we can do

1. use these optimization opportunities
2. rewrite the whole thing without the node tree
