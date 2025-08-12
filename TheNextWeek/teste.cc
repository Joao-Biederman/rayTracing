#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"

#include <iostream>
#include <random>

std::random_device rd;
// std::mt19937 gen(rd());
std::mt19937 gen(42);

hittable_list world;

auto white = make_shared<lambertian>(color(.73, .73, .73));
auto yeallow = make_shared<lambertian>(color(.64, .64, .12));
auto blue = make_shared<lambertian>(color(.12, .15, .82));
auto red = make_shared<lambertian>(color(.65, .08, .08));

auto metallic_texture = make_shared<image_texture>("metallic.jpeg");
auto metallic_lambertian = make_shared<lambertian>(metallic_texture);

auto marble_texture = make_shared<image_texture>("marble.jpg");
auto marble_lambertian = make_shared<lambertian>(marble_texture);

auto polished_metal = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);

auto glass_material = make_shared<dielectric>(1.5);

auto pertext = make_shared<noise_texture>(0.2);
auto noise_material = make_shared<lambertian>(pertext);

int generate_int(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

int main() {

    auto ground_material = make_shared<lambertian>(make_shared<solid_color>(color(0.85, 0.85, 0.85)));
    world.add(make_shared<quad>(point3(-100, 0, -100), vec3(200, 0, 0), vec3(0, 0, 200), ground_material));

    auto light_material = make_shared<diffuse_light>(color(4,4,4));

    world.add(make_shared<sphere>(point3(30, 50, 20), 5, light_material));

    double square_size = 8.0;
    double spacing = 10.0;
    double start_x = - (1.5 * spacing);
    double y_height = 5.0;

    world.add(make_shared<quad>(point3(start_x, y_height, 0), vec3(square_size, 0, 0), vec3(0, square_size, 0), metallic_lambertian));

    world.add(make_shared<quad>(point3(start_x + spacing, y_height, 0), vec3(square_size, 0, 0), vec3(0, square_size, 0), polished_metal));

    world.add(make_shared<quad>(point3(start_x + 2 * spacing, y_height, 0), vec3(square_size, 0, 0), vec3(0, square_size, 0), marble_lambertian));
    
    world.add(make_shared<quad>(point3(start_x + 3 * spacing, y_height, 0), vec3(square_size, 0, 0), vec3(0, square_size, 0), noise_material));

    auto wall_color = make_shared<lambertian>(make_shared<solid_color>(color(0.85, 0.85, 0.85)));

    world.add(make_shared<quad>(point3(-100, 0, -100), vec3(200, 0, 0), vec3(0, 100, 0), wall_color));
    world.add(make_shared<quad>(point3(-100, 0, 100), vec3(200, 0, 0), vec3(0, 100, 0), wall_color));
    world.add(make_shared<quad>(point3(-100, 0, -100), vec3(0, 100, 0), vec3(0, 0, 200), wall_color));
    world.add(make_shared<quad>(point3(100, 0, -100), vec3(0, 100, 0), vec3(0, 0, 200), wall_color));

    for (int i = 0; i < 20; ++i) {
        auto ball_color = make_shared<solid_color>(color(random_double(), random_double(), random_double()));
        auto ball_material = make_shared<lambertian>(ball_color);

        point3 ball_center = point3(generate_int(-8, 8), 0.5, generate_int(-8, 8));

        world.add(make_shared<sphere>(ball_center, 0.5, ball_material));
    }

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 20;
    cam.max_depth         = 50;
    
    cam.vfov     = 75;
    
    cam.defocus_angle = 0.2;
    cam.focus_dist    = 15.0;
    
    cam.vup      = vec3(0,1,0);

    cam.lookfrom = point3(35,5,3);
    cam.lookat   = point3(0,5,0);

    cam.render(world, "teste1.ppm");

    cam.lookfrom = point3(-8,7,3);
    cam.lookat   = point3(3,0,-3);

    cam.render(world, "teste2.ppm");

    cam.lookfrom = point3(6,1,3);
    cam.lookat   = point3(3,7,0);

    cam.render(world, "teste3.ppm");

    cam.lookfrom = point3(-12, 1, -13);
    cam.lookat   = point3(15,2,-4);

    cam.render(world, "teste4.ppm");

    cam.lookfrom = point3(13, 15, 5);
    cam.lookat   = point3(-5, -5, 5);

    cam.render(world, "teste5.ppm");
}