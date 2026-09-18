# PeepHole 👁️

> **Not Windows, hence a Peephole only** 😄

A Small Piggy-Back OS on top of __ FreeDOS.__
A tiny 32-bit OS that peeks from real-mode DOS through `PIGGY.COM` into its own world at 1MB.



### The Idea

We don't try to be Windows. We just open a small hole from __FreeDOS__ and look inside.

Main idea is learn, learn and learn. Then Learn more ,more and More 😄
 
### Current Capabilities 
1. FreeDOS boots then  from `D:\`
2. You run `PIGGY.COM` - a tiny 16-bit DOS program
3. PIGGY loads `KERNEL.BIN` to `0x100000` (1MB)
4. Switches to 32-bit Protected Mode
5. Jumps to kernel → you see text at `0xB8000`

#### How to try it
1. setup your FreeDOS VM. 
2. mount a directory to it
3. execute build.sh on the source tree
4. copy KERNEL.BIN and PIGGY.com to the mounted directory 
5. now from mounted Directory(D:for FreeDOS) execute PIGGY.COM.

__That's it. A peephole, not a window.__
### Future plan
1. adding a memory manager
2. thread support in kernel
3. adding a scheduler (roundrobbin)
4. a command shell
5. a gui
6. many more ideas...!




