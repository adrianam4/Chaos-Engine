# Chaos-Engine

An amazing Engine created by [David Lira](https://github.com/davidlira19), [Adrián Aroca](https://github.com/adrianam4) and [Unai Díaz](https://github.com/unaidiaz), three university students from CITM-UPC for the subject Game Engines. 
With this engine you can create and transform 3D objects and your imported FBX, load textures and much more!


## Github Accounts:

- [David Lira](https://github.com/davidlira19)
- [Adrián Aroca](https://github.com/adrianam4)
- [Unai Díaz](https://github.com/unaidiaz)


## Functions:

- Free Camera Movement
- Create primitives (Cube, Pyramid, Cyilinder and Sphere)
- Load your FBXs (Drag and Drop)
- Load your Textures (Drag and Drop)
- Transform objects (Scale, Position and Rotation) with the Inspector Menu
- Enable/Disable wireframe for the objects
- Enable/Disable normals for the objects
- Enable/Disable bounding boxes (AABB) for the objects
- Configuration Menu with many options such as a Frame Limiter, Window Size Configuration, Input and Hardware Info, etc.
- View and manage your objects in an Hierarchy
- A Console that shows your actions on the Engine
- 


## Controls:

- ESC: Exit the Engine
- WASD: Camera Movement
- Q: Camera Up
- E: Camera Down
- F: Focus the Camera Around the Selected Object
- MOUSE WHEEL: Zoom In / Zoom Out
- RIGHT MOUSE BUTTON: Camera Orientation
- LEFT MOUSE BUTTON: Select Object
- LEFT MOUSE BUTTON + LEFT ALT: Orbit Camera
- LEFT SHIFT: Duplicates Camera Movement
- P + Select Object: Makes the First Selected Object Children of the Second One
- SUPR: Deletes Selected Game Object
- F1: Enable/Disable Deph Test
- F2: Enable/Disable Cull Face
- F3: Enable/Disable Lightning
- F4: Enable/Disable Color Material
- F5: Enable/Disable Textures
- F10: Save Config
- F11: Load Config
- BUTTON (1): Translation Guizmo
- BUTTON (2): Rotation Guizmo
- BUTTON (3): Scale Guizmo


## Additional Functionality

- You can view the Bounding Boxes (AABB) of each Object
- You can set the Scale and Position before creating Objects


## Additional Comments

- You need to keep pressing the P key to assign one Children to the Father
- If you don't see a window you can make it visible with the top menu selecting View
- Our two extra FBX can be loaded with the Create Menu


## Important Note

- We used the ModuleFrameBuffer.ccp and ModuleFrameBuffer.h from the provided engine (https://github.com/solidajenjo/Engine3D) in order to make the Window Docking/Viewport!!!!!