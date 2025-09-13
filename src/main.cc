# include "constants.h"
# include "hittable.h"
# include "hittable_list.h"
# include "material.h"
# include "sphere.h"
# include "interval.h"
# include "camera.h"


int main(){
    auto R = cos(pi/4);
    
    // World materials
    auto material_left = std::make_shared<lambertian>(color(0, 0, 1));
    auto material_right =std::make_shared<lambertian>(color(1, 0, 0));

    // World
    hittable_list world;

    world.add(std::make_shared<sphere>(point3(-R, 0.0, -1.0), R, material_left));
    world.add(std::make_shared<sphere>(point3(R, 0.0, -1.0), R, material_right));

    // Camera
    camera cam;
    
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.vfov = 90.0;

    // Render
    cam.render(world);

}

