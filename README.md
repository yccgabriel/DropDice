Drop Dice
=========

Introduction
------------

Drop Dice is a project Gabriel worked on in order to learn OpenGL and 3D graphics programming. The project is not yet considered finished as not all features are written. This read-me is written for the purpose of showing the public that Gabriel's knowledge and skill set on computer graphics.


Implemented Features
--------------------
* Camera move and rotate freely
* Dash-board added with limited buttons added (buttons were not implemented because the logic behind was not ready)
* A light-weight physics engine(qu3e) is applied
* Cubic objects dropped on regular intervals
* Object-selection using cursor in 3D space(no external library were use)
* Further extend object-selction to drag and move objects in 3D space(the black floor)
![camera](/screenshots/1.gif?raw=true)
![dashboard](/screenshots/2.gif?raw=true)
![moving ground](/screenshots/3.gif?raw=true)
![moving ground](/screenshots/4.gif?raw=true)

Planned Features
----------------
* Rotation of floor (currently the floor can only translate but no rotate)
* Scaling of floor
* Saving and loading the scene (the saving function was written: serialize the objects position, i.e. the floors, and save it to a single file. Unfortunately the floor implementation was not complete so this feature was delayed)

Outstanding Bug
---------------
* The Object-selection doesn't work once dragged and released on the RGB rods.

Dependency
----------
The Project was developed on Windows Platform with OpenGL and C++. Theoratically it could be modified to portable across platform but that was not a top priority.
The following list summarized the libraries and software were used for development
* Windows 7
* Visual Studio 2015
* GLEW
* GLFW
* qu3e (physics engine)
* GLM (OpenGL Maths Library)
* ImGui (Dashboard)