# Ember - A C++ Game Development Framework

Ember is a Modern C++ Framework for Game Development. By itself, it isn't a game engine, as it doesn't do anything, but it's structures and concepts
can be used to facilitate the development of a game, providing useful abstractions to work with.

## Currently Ember gives you:

- Core architectural concepts of a component based engine, such as Scenes, Systems, GameObjects and game object Behaviours
- Scene Wide, System Wide, or Object Wide Event dispatching
- Behaviour Addons to provide further, modular, functionality, such as Serialization.

## Requirements

- C++11 compliant compiler

## How to Use

The Ember framework is reliant on it's Scene/System/GameObject/Behaviour structure, as such, development with the ember framework is focused around implementing Behaviours and/or Systems.
Based on the presented structure, create your own Behaviours (as subclasses of Behaviour.hpp), and easily attach them to game objects inside scenes.
Afterwards repeateadly run the RunUpdateCycle method, on your scene object, to make it all tick.

That's the gist of it really.

## No, but, where do I start?

So, right of the bat, you want to make sure you can run the lib properly. So let's start by compiling it.

### Compiling
Simply run the following commands:
```
make
make install DESTDIR=<your install directory>
```
If unset, DESTDIR will default to /usr/local

### Linking
Obviously, you need to make sure your include path is able to find DESTDIR, be it where it may.
Ember results will be present in two forms, both headers for inclusion, and the compiled library (libember.a).
You must link your project against this lib file.


## Usage Examples - Scenes, Objects and Behaviours

Nothing in ember happens without a scene, so that's the first place to start.

```
#include "ember/Core.h"

using namespace ember;

int main(int argc, char[] argv) {
  Scene my_scene;
}
```

Excellent!... Except this really does nothing at all, the Scene is a means to an end, so by itself, there's nothing it can do.
Let's add an object to the scene.

```
#include "ember/Core.h"

using namespace ember;

int main(int argc, char[] argv) {
  Scene my_scene;

  my_scene.addGameObject();
}
```

Now the scene contains an object it can work with. However, objects by default are blank slates, they have no data, or any discernable behaviour.
This object, will do nothing. For it to be able to do something, we must give it a behaviour. So, let's write one, and let's assign it to our new object.

```
#include <iostream>
#include "ember/Core.h"

using namespace ember;

class Tenor : public Behaviour {
public:
  Tenor(const std::string& name) : _name(name) {}

  virtual void onUpdate(double deltaT) override {
    std::cout << "[" << deltaT << "] " << _name <<  ": La Donna È Mobile!" << std::endl;
  }

private:
  std::string _name;
}

int main(int argc, char[] argv) {
  Scene my_scene;

  my_scene.addGameObject().withBehaviour<Tenor>("Pavarotti");
}
```

We have now defined the behaviour of a Tenor (to sing la donna é mobile on every update), and we made it so our newly created object would behave as one.
Still, this is not enough to get an output from our program. The stage is set, and the singer is in place, but we need to roll up the curtains and make the show start. To do so, we must call RunUpdateCycle on our scene object, to make the scene move.

```
#include <iostream>
#include "ember/Core.h"

using namespace ember;

class Tenor : public Behaviour {
public:
  Tenor(const std::string& name) : _name(name) {}

  virtual void onUpdate(double deltaT) override {
    std::cout << "[" << deltaT << "] " << _name <<  ": La Donna È Mobile!" << std::endl;
  }

private:
  std::string _name;
}

int main(int argc, char[] argv) {
  Scene my_scene;

  my_scene.addGameObject().withBehaviour<Tenor>("Pavarotti");

  my_scene.RunUpdateCycle(1);
  my_scene.RunUpdateCycle(2);
  my_scene.RunUpdateCycle(3);
}
```

Output:
```
[1] Pavarotti: La Donna È Mobile!
[2] Pavarotti: La Donna È Mobile!
[3] Pavarotti: La Donna È Mobile!
```

Congrats, you now have a very basic ember application running!

## Usage Examples - Systems

Behaviours might be the bread and butter of Ember, but Systems allow for much more complex functionality.
A System gives you a way to perform common actions on a subset of filtered GameObjects. If you need to write code that depends on multiple Behaviours
existing in an object, Systems are likely to be the easier solution.
Take a look a the example below, where we model a Conductor system, that is responsible for picking which musicians it wants to play:

```
#include <iostream>
#include "ember/Core.h"

using namespace ember;

struct Musician : public Behaviour {
  std::string name;

  Musician(const std::string& name) : name(name) {}
}

struct Voice : public Behaviour {
  std::string voice_type;

  Voice(const std::string& voice_type) : voice_type(voice_type) {}
}

struct Strings : public Behaviour {
  std::string instrument_name;

  Strings(const std::string& instrument_name) : instrument_name(instrument_name) {}
}

struct Partiture : public Behaviour {
  std::string musical_notes;

  Partiture(const std::string& musical_notes) : musical_notes(musical_notes) {}
}

class Conductor : public System<RequiresBehaviours< Musician, Strings, Partiture >> {
  virtual void onUpdate(double deltaT, GameObject& object) override {
    std::cout << "[" << deltaT << "] " <<
      object.refBehaviour<Musician>().name << "-" <<
      object.refBehaviour<Strings>().instrument_name << ": " <<
      object.refBehaviour<Partiture>().musical_notes << std::endl;
  }
}

int main(int argc, char[] argv) {
  Scene my_scene;

  my_scene.attachSystem<Conductor>();

  my_scene.addGameObject().withBehaviour<Musician>("Pavarotti").
                            withBehaviour<Voice>("Tenor").
                            withBehaviour<Partiture>("La Donna È Mobile!");

  my_scene.addGameObject().withBehaviour<Musician>("Vivaldi").
                            withBehaviour<Strings>("Violin").
                            withBehaviour<Partiture>("~Winter~");

  auto& beethoven_reference = my_scene.addGameObject().withBehaviour<Musician>("Beethoven").
                                                  withBehaviour<Strings>("Violin");

  my_scene.RunUpdateCycle(1);
  beethoven_reference.withBehaviour<Partiture>("~9th~");
  my_scene.RunUpdateCycle(2);

}
```

Output:
```
[1] Vivaldi-Violin: ~Winter~
[2] Vivaldi-Violin: ~Winter~
[2] Beethoven-Violin: ~9th~
```

So, what happened here? Our conductor is only interested in Musicians that play string instruments, and already
have a partiture they can play.
On the first run of 'RunUpdateCycle', only Vivaldi matched the criteria, Pavarotti uses his Voice, not Strings, and Beethoven doesn't have a Partiture.
Before the second run, we add a Partiture to Beethoven, so when we call 'RunUpdateCycle' again both Vivaldi and Beethoven match the requirements
of the Conductor, so both of them play.

# Documentation

No formal documentation exists for ember (yet) however, the entire public API of the framework is commented, so you're free to peruse it to
understand all the options available. A good place to start are the core modules (core/GameObject.hpp, core/Scene.hpp, core/System.hpp and core/Behaviour.hpp).
