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

auto metallic_texture = make_shared<image_texture>("metallic.jpeg");
auto metal_material = make_shared<lambertian>(metallic_texture);

auto marble_texture = make_shared<image_texture>("marble.jpg");
auto marble_material = make_shared<lambertian>(marble_texture);

auto mirror_material = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);

auto metallic_material = make_shared<metal>(color(0.8, 0.6, 0.2), 0.3);
auto glass_material = make_shared<dielectric>(1.5);

auto pertext = make_shared<noise_texture>(0.2);
auto noise_material = make_shared<lambertian>(pertext);

point3 generate_3d_point(int min, int max, int max_y = 5) {
    std::uniform_real_distribution<> dis(min, max);
    auto x = dis(gen);
    auto y = dis(gen);
    auto z = dis(gen);

    if (y < 0) {
        y = 0;
    }

    return point3(x, y, z);
}

int generate_int(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

double random_double(int max) {
    return std::rand() / (max + 1.0);
}

void add_random_sphere() {
    auto choose_mat = random_double();
    point3 center = generate_3d_point(-30, 30);

    if (choose_mat < 0.25) {
        world.add(make_shared<sphere>(center, generate_int(1, 4), noise_material));
    } else if (choose_mat < 0.5) {
        world.add(make_shared<sphere>(center, generate_int(1, 4), marble_material));
    } else if (choose_mat < 0.75) {
        world.add(make_shared<sphere>(center, generate_int(1, 4), metallic_material));
    } else if (choose_mat < 0.9) {
        world.add(make_shared<sphere>(center, generate_int(1, 4), metal_material));
    } else {
        world.add(make_shared<sphere>(center, generate_int(1, 4), glass_material));
    }
}

void add_random_quad() {
    auto choose_mat = random_double();
    point3 center = generate_3d_point(-30, 30);

    vec3 u = vec3(random_double() * 2 + 1, 0, 0);
    vec3 v = vec3(0, random_double() * 2 + 1, 0);

    if (choose_mat < 0.25) {
        world.add(make_shared<quad>(center, u, v, noise_material));
    } else if (choose_mat < 0.5) {
        world.add(make_shared<quad>(center, u, v, marble_material));
    } else if (choose_mat < 0.75) {
        world.add(make_shared<quad>(center, u, v, metallic_material));
    } else if (choose_mat < 0.9) {
        world.add(make_shared<quad>(center, u, v, metal_material));
    } else {
        world.add(make_shared<quad>(center, u, v, glass_material));
    }
}

int main() {

    
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto yeallow = make_shared<lambertian>(color(.82, .82, .12));
    auto blue = make_shared<lambertian>(color(.12, .15, .82));
    auto red = make_shared<lambertian>(color(.65, .05, .05));

    world.add(make_shared<sphere>(point3(0,-1000,-250), 1000, white));
    world.add(make_shared<sphere>(point3(0,-1000,250), 1000, yeallow));

    world.add(make_shared<quad>(point3(-200,-100,0), vec3(0,900,0), vec3(0,0,1000), blue));
    world.add(make_shared<quad>(point3(-200,-100,-500), vec3(0,900,0), vec3(0,0,500), red));

    auto light_material = make_shared<diffuse_light>(color(4,4,4));
    world.add(make_shared<quad>(point3(-25, 50, -25), vec3(50, 0, 0), vec3(0, 0, 50), light_material));

    for (int i = 0; i < 20; ++i) {
        if (random_double() < 0.5) {
            add_random_quad();
        } else {
            add_random_sphere();
        }
    }

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 10;
    
    cam.vfov     = 75;
    
    cam.defocus_angle = 0.2;
    cam.focus_dist    = 15.0;
    
    cam.vup      = vec3(0,1,0);

    cam.lookfrom = point3(35,2,3);
    cam.lookat   = point3(0,2,0);

    cam.render(world, "scene1.ppm");

    cam.lookfrom = point3(-8,7,3);
    cam.lookat   = point3(3,0,-3);

    cam.render(world, "scene2.ppm");

    cam.lookfrom = point3(6,1,3);
    cam.lookat   = point3(3,7,0);

    cam.render(world, "scene3.ppm");

    cam.lookfrom = point3(-12, 1, -13);
    cam.lookat   = point3(15,2,-4);

    cam.render(world, "scene4.ppm");

    cam.lookfrom = point3(13, 15, 5);
    cam.lookat   = point3(-5, -5, 5);

    cam.render(world, "scene5.ppm");
}