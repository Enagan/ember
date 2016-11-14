# Ember - A C++ Game Development Framework

Ember is a Modern C++ Framework for Game Development. By itself, it isn't a game engine, as it doesn't do anything by itself, but it's structures and concepts
can be used to facilitate the development of a game, providing useful abstractions to reason around.

## Currently Ember gives you:

- Core architectural concepts of a component based engine, such as Scenes, GameObjects and game object Behaviours
- Scene Wide or Object Wide Event dispatching
- Behaviour Addons to provide further, modular, functionality, such as Serialization.

## Requirements

- C++14 compliant compiler

## Compiling
Simply run the following commands:
```
make
make install DESTDIR=<your install directory>
```
If unset, DESTDIR will default to /usr/local
## How to Use

The Ember framework is reliant on it's Scene/GameObject/Behaviour structure, as such, development with the ember framework is focused around implementing Behaviours.
Based on the presented structure, create your own Behaviours (as subclasses of Behaviour.hpp), and easily attach them to game objects inside scenes.
From your own application, call the OnStart, OnUpdate and onPostUpdate methods where they make sense to se everything tick.

# Examples

Coming Soon...

## Author

- Pedro Engana (Enagan) <pedro.a.f.engana@gmail.com>

## License

Crier is available under the MIT license (see `LICENSE`).
