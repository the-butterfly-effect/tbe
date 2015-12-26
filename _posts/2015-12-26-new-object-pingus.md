---
layout: post
author: Klaas van Gend
---

As an hommage to the open source game [Pingus](pingus.seul.org), we have added the penguin of the game to TBE. So we now have a small penguin (or groups of penguins) that can walk around in levels. We have limited features compared to the original Pingus, though:

 * our Pingus can walk (0.0-0.5 m/s) and slide (>0.5 m/s)
 * our Pingus can fall down
 * our Pingus can be asleep sleep or waiting

Like Pingus, if a penguin reaches the exit, he'll happily jump in, escaping the level.

We borrowed all images from Pingus as well. This is why the penguin looks pixelated, whereas the other objects in TBE should be rendered to match the resolution of your screen.

Because TBE has a different approach to game play, a few things didn't make it:

 * you cannot during play adjust the penguin. In the original game, you could click a Pingus to start building a bridge, block, explode or have a parachute
 * Pingus cannot drown
 * Pingus cannot climb

The penguin will be a major new feature of the upcoming Milestone B. Right now, the only way to see the new Pingus in action, is to clone the github repo and look at the various testing 'levels' in the `test/Pingus` directory. We've also added the Pingus to a variant of the 'my brother plays soccer' level, as shown below:

We're very proud of our new helper and we hope you'll like him, too!
