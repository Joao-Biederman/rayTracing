//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"

int main() {
    hittable_list world;

    std::cout << "Creating sun" << std::endl;
    auto sun_color = make_shared<solid_color>(color(3, 2.5, 2));
    auto sun_material = make_shared<diffuse_light>(sun_color);
    world.add(make_shared<sphere>(point3(-15, 70, -9), 40, sun_material));

    std::cout << "Creating sky" << std::endl;
    auto sky_color = make_shared<solid_color>(color(0.5, 0.7, 1.0));
    auto sky_box = make_shared<lambertian>(sky_color);
    world.add(make_shared<quad>(point3(-40, -5, 15), vec3(0, 0, -50), vec3(0, 30, 0), sky_box));

    std::cout << "Creating ground" << std::endl;
    auto snow_texture = make_shared<image_texture>("snow.jpg");
    auto ground_material = make_shared<lambertian>(snow_texture);
    auto ground = make_shared<quad>(point3(-500, -500, -500), vec3(500, 500, 500), vec3(0, 0, -50), ground_material);
    
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<snow>()));

    world.add(make_shared<sphere>(point3(-8,-2,-4), 3, ground_material));
    world.add(make_shared<sphere>(point3(-10,-3,-10), 5, ground_material));
    world.add(make_shared<sphere>(point3(-6, 0, 6), 2, ground_material));
    world.add(make_shared<sphere>(point3(-3, -0.5, 8), 1, ground_material));
    world.add(make_shared<sphere>(point3(-3, -3, 6), 5, ground_material));
    
    world.add(make_shared<sphere>(point3(-13, -5, 7), 8, ground_material));
    
    world.add(make_shared<sphere>(point3(-3, -2, 5), 4, ground_material));
    world.add(make_shared<sphere>(point3(-3, -0.4, -1), 1, ground_material));

    std::cout << "Creating snowman" << std::endl;
    auto snowball_texture = make_shared<image_texture>("snowball.jpg");
    auto snowballs = make_shared<lambertian>(snowball_texture);
    world.add(make_shared<sphere>(point3(-5, 0.55, 0), 0.6, snowballs));
    world.add(make_shared<sphere>(point3(-5, 1.3, 0), 0.4, snowballs));
    world.add(make_shared<sphere>(point3(-5, 1.8, 0), 0.3, snowballs));

    auto stones = make_shared<rock>();
    world.add(make_shared<sphere>(point3(-4.4, 0.55, 0), 0.1, stones));
    world.add(make_shared<sphere>(point3(-4.6, 1.3, 0), 0.1, stones));
    world.add(make_shared<sphere>(point3(-4.8, 1.9, 0.2), 0.1, stones));
    world.add(make_shared<sphere>(point3(-4.8, 1.9, -0.2), 0.1, stones));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 10;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 15.0;

    std::cout << "Rendering snowman scene..." << std::endl;

    cam.render(world, "snowman1.ppm");

    cam.samples_per_pixel = 200;
    cam.render(world, "snowman1-1.ppm");
    //===========================================================\\ 
    
    cam.vfov     = 50;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,2,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world, "snowman2.ppm");

    //===========================================================\\ 

    cam.vfov     = 50;
    cam.lookfrom = point3(5,0.5,-13);
    cam.lookat   = point3(-5,3,2);
    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world, "snowman3.ppm");

    //===========================================================\\ 

    cam.vfov     = 90;
    cam.lookfrom = point3(-5,12,1);
    cam.lookat   = point3(-5,1,-1);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 8.0;

    cam.render(world, "snowman4.ppm");

    //===========================================================\\ 

    cam.vfov     = 90;
    cam.lookfrom = point3(-3,8,8);
    cam.lookat   = point3(1,1,-1);

    cam.defocus_angle = 0.8;
    cam.focus_dist    = 3.0;

    cam.render(world, "snowman5.ppm");


}
