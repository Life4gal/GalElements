# [A copy of Elements C++ GUI library](https://github.com/cycfi/elements)
# Original Author: Joel de Guzman(Distributed under the MIT License)

# ![Elements-Logo](docs/assets/images/elements.png) Elements C++ GUI library

![alt Photon Sampler](docs/assets/images/photon_sampler.jpg)

## Introduction

Elements is a lightweight, fine-grained, resolution independent, modular GUI library.
Elements is designed with these requirements in mind:

1. It should be open source with a liberal, non-viral license.
2. It should be usable in any application and should play well with other GUI
   libraries and frameworks.
3. Corollary to the second requirement is that it can also be used to develop
   plugins. It should not own the event loop and should be able to co-exist
   with components within a plugin host such as VST and AU.
4. It should be resolution independent and allow for HDPI displays.
5. Should not rely on a “visual” GUI editor or code generator.
6. It should have a declarative API using modern C++. Declarative C++ code
   tells you *what* rather than *how* (imperative). The GUI should be
   declared in C++ code.
7. Keep it simple. Make it easy to use right out of the box, with a lot of examples.
8. Porting to a new host target should be as easy as possible, requiring
   porting of only a few files.

Elements, is extremely lightweight… and modular. You compose very
fine-grained, flyweight “elements” to form deep element hierarchies using a
declarative interface with heavy emphasis on reuse.

### [More detail](https://github.com/cycfi/elements/blob/master/README.md)

## Note: although this repository is based on **elements**, there may be many implementations that are completely different from the original
### why not fork: As mentioned above, the implementation of this repository has changed a lot, so it is not intended to be pushed to the original repository