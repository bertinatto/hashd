# hashd - a simple in-memory cache

hashd is simple in-memory cache developed in C. It aims to be a testbed for my own learning.

## FAQ

### Why a in-memory hash table?

It seemed like a perfect opportunity to implement several concepts that I had little or none experience with. Among them:

* Free lists
* Memory fragmentation
* OpenBSD coding practices
* Benchmark and optimization of several data structures in C
* Protocol design and implementation
* Socket programming

### Any inspirations?

I find the source code of OpenBSD networking daemons pretty perfect. The code quality we find in OpenBSD is probably the highest than any other project. That being said, I like to strictly follow their practices, and *hashd* is playground to help me getting to that level.

### Is it good to go?

Not at all. For now it's basically a proof-of-concept. The fun starts now.
