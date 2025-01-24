#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
    this->red = 0.4;
    this->green = 0;
    this->blue = 0.8;

    this->obj_id = 0;
    this->objects.push_back("box");
    this->objects.push_back("sphere");
    this->objects.push_back("teapot");

    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;

    this->jump = 0.0;
    this->to_jump = 0;
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");


        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.

    {
        Mesh* mesh2 = new Mesh("sphere");
        mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");

        meshes[mesh2->GetMeshID()] = mesh2;
    }

    {
        Mesh* mesh3 = new Mesh("teapot");
        mesh3->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");

        meshes[mesh3->GetMeshID()] = mesh3;
    }

}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    // glClearColor(0.4, 0, 0.8, 1);
    glClearColor(this->red, this->green, this->blue, 1);


    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    // RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    // RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.
    // RenderMesh(meshes["bunny"], glm::vec3(-3, 0.5f, 0), glm::vec3(0.05f));

    if (to_jump)
        jump += 0.05;

    if (jump >= 1.0) {
        to_jump = 0;
    }

    if (to_jump == 0 && jump > 0) {
        jump -= 0.05;
    }

    RenderMesh(meshes[this->objects[this->obj_id]], glm::vec3(this->x, this->y + jump, this->z), glm::vec3(0.5f));

}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.
    if (window->KeyHold(GLFW_KEY_W)) {
        z -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        z += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        x -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        x += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        y += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        y -= deltaTime;
    }

}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        this->red = 0;
        this->green = 0.8;
        this->blue = 0;
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.
    if (key == GLFW_KEY_H) {
        this->obj_id++;
        if (this->obj_id == 3)
            this->obj_id = 0; 
    }

    if (key == GLFW_KEY_J) {
        this->to_jump = 1;
    }
    
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
