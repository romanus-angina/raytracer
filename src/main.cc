# include "constants.h"
# include "hittable.h"
# include "hittable_list.h"
# include "material.h"
# include "sphere.h"
# include "interval.h"
# include "camera.h"


int main(){
    // World materials
    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right =std::make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // World
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0,0,-1.2), 0.5, material_center));
    world.add(std::make_shared<sphere>(point3(0,-100.5,-1.0), 100, material_ground));
    world.add(std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(std::make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    camera cam;
    
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    // Render
    cam.render(world);

}

