#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"
#include "texture.h"

#include <iostream>
#include <random>

std::mt19937 gen(42);

double random_float() {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

point3 generate_random_point(double min, double max) {
    std::uniform_real_distribution<> dis(min, max);
    return point3(dis(gen), 0.5, dis(gen));
}

int main() {
    hittable_list world;

    auto metallic_texture_png = make_shared<image_texture>("metallic.jpeg");
    auto metallic_lambertian_png = make_shared<lambertian>(metallic_texture_png);

    auto marble_texture_png = make_shared<image_texture>("marble.jpg");
    auto marble_lambertian_png = make_shared<lambertian>(marble_texture_png);

    auto procedural_metal = make_shared<metal>(color(0.8, 0.7, 0.6), 0.0);

    auto pertext = make_shared<noise_texture>(4.0);
    auto procedural_marble = make_shared<lambertian>(pertext);

    auto ground_material = make_shared<lambertian>(make_shared<solid_color>(color(0.9, 0.9, 0.9)));
    
    auto main_light_material = make_shared<diffuse_light>(make_shared<solid_color>(color(10.0, 10.0, 10.0)));
    
    auto fill_light_material = make_shared<diffuse_light>(make_shared<solid_color>(color(3.0, 3.0, 3.0)));
    
    auto sky_material = make_shared<lambertian>(make_shared<solid_color>(color(0.6, 0.7, 1.0)));


    world.add(make_shared<quad>(point3(-100, 0, -100), vec3(200, 0, 0), vec3(0, 0, 200), ground_material));

    world.add(make_shared<quad>(point3(-100, 0, -100), vec3(200, 0, 0), vec3(0, 100, 0), sky_material));
    world.add(make_shared<quad>(point3(-100, 0, -100), vec3(0, 100, 0), vec3(0, 0, 200), sky_material));
    world.add(make_shared<quad>(point3(100, 0, -100), vec3(0, 100, 0), vec3(0, 0, 200), sky_material));

    world.add(make_shared<sphere>(point3(30, 50, 20), 5, main_light_material));
    
    world.add(make_shared<sphere>(point3(-20, 15, 10), 2, fill_light_material));

    double square_size = 8.0;
    double spacing = 10.0;
    double start_x = - (1.5 * spacing);
    double y_height = 5.0;


    world.add(make_shared<quad>(point3(start_x, y_height, 0), vec3(square_size, 0, 0), vec3(0, square_size, 0), metallic_lambertian_png));
    world.add(make_shared<quad>(point3(start_x + spacing, y_height, 0), vec3(square_size, 0, 0), vec3(0, square_size, 0), procedural_metal));

    world.add(make_shared<quad>(point3(start_x + 2 * spacing, y_height, 0), vec3(square_size, 0, 0), vec3(0, square_size, 0), marble_lambertian_png));
    world.add(make_shared<quad>(point3(start_x + 3 * spacing, y_height, 0), vec3(square_size, 0, 0), vec3(0, square_size, 0), procedural_marble));
    


    int num_balls = 20;
    double ball_radius = 0.5;

    for (int i = 0; i < num_balls; ++i) {
        auto ball_color = make_shared<solid_color>(color(random_float(), random_float(), random_float()));
        auto ball_material = make_shared<lambertian>(ball_color);


        point3 ball_center = generate_random_point(-50, 50);


        world.add(make_shared<sphere>(ball_center, ball_radius, ball_material));
    }

    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 250;
    cam.max_depth         = 50;

    cam.vfov              = 75;
    cam.lookfrom          = point3(0, 15, 30);
    cam.lookat            = point3(0, 5, 0);
    cam.vup               = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 30.0;

    cam.render(world, "scene1.ppm");

    cam.vfov              = 75;
    cam.lookfrom          = point3(-20, 10, 15);
    cam.lookat            = point3(0, 5, 0);
    cam.render(world, "scene2.ppm");

    cam.vfov              = 75;
    cam.lookfrom          = point3(20, 10, 15);
    cam.lookat            = point3(0, 5, 0);
    cam.render(world, "scene3.ppm");

    cam.vfov              = 75;
    cam.lookfrom          = point3(0, 20, -10);
    cam.lookat            = point3(0, 5, 0);
    cam.render(world, "scene4.ppm");
    
    cam.lookfrom          = point3(10, 25, 20);
    cam.lookat            = point3(0, 5, 0);
    cam.render(world, "scene5.ppm");
    return 0;
}
