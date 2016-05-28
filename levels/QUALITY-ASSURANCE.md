For quality assurance, levels are structured in multiple directories
which reflect their quality, completeness and correctness.


## Significance of the directories

* `draft`: WIP levels which are not playable yet or have serious
  gameplay or graphical issues. Can change radically at
  any time
* `needs-polish`: Solvable and has proper goal text but has some minor
  issues, mostly graphical (caused by level itself, not by the engine).
  Also, a needs-polish level should be expected to not change significantly
  (esp. in gameplay)
* `finished`: Solvable and highly polished. High quality requirements, can be
  presented in levels.xml and releases without embarrassing ourselves.
  Requirements:
    * Must be solvable
    * Must have a unique name
    * Proper goal description. Succinct and to the point.
      Story-telling goes to Post-It notes.
    * No customized object is allowed if a standard object would have been
      appropriate as well. For example, no using RectObject with old wood
      texture since Floor does the same thing.
    * Appropriate scene size (test by resizing the window)
    * All object descriptions/tooltips must be correct, if present
    * Custom non-trivial objects must have tooltips
    * Reuse existing texts to be nice to translators
    * Post-it notes are optional, but if present, all text must fit into
      the box
    * No graphical issues or oddities (caused by the level)
    * For levels supposed to be difficult: No easy shortcuts should be
      possible. Must be decided on a case-on-case basis
    * Correct spelling and grammar
    * (Add your own requirements here if you have more ideas)
  Soon to become a requirement (thus optional for now):
    * Hints should be supplied so correct operation of the level can be
      validated by the regression framework
* `attic`: Old retired levels. These levels have been given up on: mechanics
   don't work, the level isn't fun or we have no ideas on how to make it good.
* `test`: Levels for testing the engine and other things

Levels in `finished` should generally go into levels.xml.
Levels in `needs-polish` may go into levels.xml, this needs to be decided on a 
case-by-case basis. Levels from other directories should not be included.


## Images

Custom images should generally NOT go into `needs-polish` or `finished`.
Instead, move them directly to the main imagery directory so these
images become available for all levels, no matter the directory.
It is okay to have custom images in draft, attic and test since it is
often easier and quicker to handle.


## Suggested workflow

Here's the suggested workflow of creating a new level and adding it into
the repository:
* Save new levels into draft, including custom images if you're lazy
* If the level is in a playable state, move it either to needs-polish or
  directly to finished if it meets the requirements (important!)
* Polish the level and fix the remaining issues
* If you consider the level finished, do one final check and then issue 
  a pull request to move it into finished
* Custom resources of finished levels should be made available globally
* If the level is good, add it to levels.xml
* Later on: If any level in finished must be changed for any reason,
  ideally it should also be checked again before committing to avoid
  regressions. If the check fails, fix the problems, then commit. If this
  is not possible or you are too lazy, either don't commit or move the
  level back to needs-polish.
* Add finished levels into the top-level Makefile for regression testing
