#include "rtweekend.h"

#include "camera.h"
#include "constant_medium.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"

int main() {
    hittable_list world;

    auto ground_texture = make_shared<image_texture>("rock.jpg");
    auto ground_surface = make_shared<lambertian>(ground_texture);

    auto sky_blue = make_shared<lambertian>(color(0.53, 0.81, 0.92));
    
    auto vibrant_magenta_metal = make_shared<metal>(color(0.8, 0.05, 0.8), 0.1);
    auto shiny_gold_metal = make_shared<metal>(color(0.8, 0.6, 0.2), 0.05);
    auto glass_material = make_shared<dielectric>(1.5);
    auto cyan_lambertian = make_shared<lambertian>(color(0.05, 0.85, 0.9));

    auto marble_texture = make_shared<noise_texture>(4);
    auto marble_surface = make_shared<lambertian>(marble_texture);
    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    auto checker_surface = make_shared<lambertian>(checker);

    auto light_material = make_shared<diffuse_light>(color(25, 25, 25));

    world.add(make_shared<quad>(point3(-1000, 0, 1000), vec3(2000, 0, 0), vec3(0, 0, -2000), ground_surface));


    world.add(make_shared<quad>(point3(-1000, 0, 1000), vec3(0, 2000, 0), vec3(2000, 0, 0), sky_blue));
    world.add(make_shared<quad>(point3(-1000, 0, -1000), vec3(0, 2000, 0), vec3(0, 0, 2000), sky_blue));
    world.add(make_shared<quad>(point3(1000, 0, 1000), vec3(0, 2000, 0), vec3(0, 0, -2000), sky_blue));


    world.add(make_shared<quad>(point3(-200, 554, -200), vec3(400, 0, 0), vec3(0, 0, 400), light_material));

    
    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), vibrant_magenta_metal);
    box1 = make_shared<rotate_y>(box1, 20);
    box1 = make_shared<translate>(box1, vec3(150, 0, -150));
    world.add(box1);

    world.add(make_shared<sphere>(point3(-200, 120, 100), 120, glass_material));

    world.add(make_shared<sphere>(point3(0, 80, 150), 80, marble_surface));


    world.add(make_shared<sphere>(point3(350, 70, 50), 70, checker_surface));

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(100,100,100), shiny_gold_metal);
    box2 = make_shared<rotate_y>(box2, -30);
    box2 = make_shared<translate>(box2, vec3(380, 0, 250));
    world.add(box2);

    world.add(make_shared<sphere>(point3(-350, 250, -180), 100, cyan_lambertian));


    auto fog_boundary = box(point3(130, -1, -170), point3(400, 340, 270), glass_material);
    world.add(make_shared<constant_medium>(fog_boundary, 0.001, color(1.0, 1.0, 1.0)));


    hittable_list lights;
    auto m = shared_ptr<material>();
    lights.add(make_shared<quad>(point3(-200, 554, -200), vec3(400,0,0), vec3(0,0,400), m));


    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 250;
    cam.max_depth         = 50;

    cam.background        = color(0.53, 0.81, 0.92);

    cam.vfov     = 80.0;
    cam.lookfrom = point3(0, 250, -600);
    cam.lookat   = point3(0, 120, 0);
    cam.vup      = vec3(0, 1, 0);

    cam.defocus_angle = 0.2;
    cam.focus_dist    = 650.0;

    cam.render(world, lights, "test1.ppm");


    cam.lookfrom = point3(-500, 100, 600);
    cam.lookat   = point3(0, 120, 0);

    cam.render(world, lights, "test2.ppm");

    cam.lookfrom = point3(450, 450, 820);
    cam.lookat   = point3(0, 120, 0);

    cam.render(world, lights, "test3.ppm");

    cam.lookfrom = point3(320, 250, -500);
    cam.lookat   = point3(0, 120, 0);

    cam.render(world, lights, "test4.ppm");

    cam.lookfrom = point3(0, 250, 600);
    cam.lookat   = point3(0, 120, 0);

    cam.render(world, lights, "test5.ppm");
}