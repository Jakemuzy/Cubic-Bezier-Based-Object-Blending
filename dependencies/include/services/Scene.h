#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

//  my stuff
#include "Model.h"

/*  
    This class essentially encapsulates the whole world, or scene.
    Allows for user to specify which objects are currently rendered,
    and switch scenes dynamically.
*/

struct Scene
{
private:
    std::vector<Model> models; 

public:
    Scene()
    {
        Model backpack("C:\\Users\\jmuzy\\OneDrive\\Desktop\\Projects\\Object Blending\\backpack", "\\backpack.obj");
    }

    void AddObject(std::string path, std::string filename);
};

#endif