# SimpleProceduralGeneration
Simple generation of dungeon

Creates a sequence of rooms on the edges while checking if there is any overlap. If there is, it tries to re-place the room in a different edge of the previous one. You can add an offset in order to not spawn the next room right next to it so there is more variety. It then generates walls. (in this test I have them as buttons in the Details tab, but you can just have a procedure to call each method). After generating Rooms and walls you can generate gates that will check where the shortest distance between the 2 rooms and replaces some walls to turn them into a gate.

![Showcase](https://github.com/NickChalvatzakis/SimpleProceduralGeneration/blob/main/UE4Editor_EaA5p3OfBe.png?raw=true)
