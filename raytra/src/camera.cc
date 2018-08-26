#include "camera.h"



void Camera::shoot(Film& f, Model& m){
   
    int tenP = this->nx / 10;

    std::cout << "[" << std::flush;
  
    //std::cout << "Samples per primary: " << this->SAMPLES_PER_PRIMARY << std::endl;
    if (this->SAMPLES_PER_PRIMARY == 0){
        for(int i = 0; i < this->nx; i++){
            //std::cout << "Loop " << i << std::endl;
            if (i % tenP == 0){
                std::cout << "=" <<  std::flush;
                
            }
            for(int j = 0; j < this->ny; j++){
                auto ray = ray_through_pixel(i, j);
                auto rgb = this->compute_radiance(m, ray, DEFAULT_REFLECTION_DEPTH);
                f.write_pixel(i, j, rgb.x, rgb.y, rgb.z, 1); 
            }
        } 

    }else{

        //std::cout << "Antialiasing: ON\n";
        auto N = sqrt(this->SAMPLES_PER_PRIMARY);
        for(int i = 0; i < this->nx; i++){
            if (i % tenP == 0){
                std::cout << "=" <<  std::flush;
                
            }
            for(int j = 0; j < this->ny; j++){
                auto rays = mc_ray_through_pixel(i, j, N);
                Vector3d rgb = Vector3d(0,0,0);
                for(auto r: rays){
                    auto trgb = this->compute_radiance(m, r, DEFAULT_REFLECTION_DEPTH);                    
                    rgb = rgb + trgb;
                }
                rgb = rgb / this->SAMPLES_PER_PRIMARY;
                f.write_pixel(i, j, rgb.x, rgb.y, rgb.z, 1); 
            }
        }  
    }

    std::cout << "=]" << std::endl;

}



Vector3d Camera::compute_radiance(Model& m, Ray& ray, unsigned int depth){

    // Find the intersection
    auto in = m.find_intersect(ray);

    // If ray does not intersect anything, we return black
    if(in.intersection_type == NO_P){
        return Vector3d(0,0,0);
    }

    // Get the material
    auto mat = m.materials[in.mat_id];

    // Get the shading normal
#ifdef NOT_SMOOTH
   auto tnormal = in.normal;
#else
    Vector3d tnormal;
    if (in.smooth_normal_available){
        tnormal = in.smooth_normal;
    }else{
        tnormal = in.normal;
    }
#endif

    //TODO: Fix this ugliness
    //TODO: Consider the normal flip
    // If we hit the back of a triangle
    if(dot(ray.dir, in.normal) > 0.0){

        //dprint("Computing warning material");

        //std::cout << "Danger material\n" << std::endl;        
        Rational r = 0;
        Rational  g = 0;
        for(auto light_src : m.lights){
            auto warning_p = ray.value_at(in.t1 - 0.001);
            auto to_light_v = light_src->to_light(warning_p);
            auto to_light_r = Ray(warning_p, to_light_v);
            auto d = to_light_v.l2norm();

            if(m.shadow_intersect(to_light_r, d)){
                continue;
            }

            // Get the angle of the light source
            to_light_v = normalize(to_light_v);
            auto theta = -dot(tnormal, to_light_v);

            // Make sure theta is non-negative
            if(theta < 0){
                theta = 0;
            }
            
            auto ldr = light_src->r * theta;
            auto ldg = light_src->g * theta;

            Rational dsq = d*d;
            r += ldr / dsq;
            g += ldg / dsq;
            //std::cout << "From some light R,G: " << r << g << std::endl;        
            

        }
        //std::cout << "R,G: " << r << g << std::endl;        
        
        return Vector3d(r,g,0);
    }

    Rational r = 0;
    Rational g = 0;
    Rational b = 0;

    auto SAMPLES_PER_SHADOW = this->SAMPLES_PER_SHADOW;
    auto N = sqrt(SAMPLES_PER_SHADOW);

    for(auto light_src : m.lights){

        auto light_samples = light_src->sample_light(N);
        auto sample_divisor = light_samples.size();
        //std::vector<Point3d> light_samples;


        Rational rr = 0;
        Rational gg = 0;
        Rational bb = 0;

        for(auto light_sample : light_samples){

            // Need to check if we can actually reach the light src
            //auto to_light_v = light_src->to_light(in.inter_p);
            auto to_light_v = light_sample - in.inter_p;

            // Build the ray to the light source
            auto to_light_r = Ray(in.inter_p + 0.00001 * to_light_v, to_light_v);
            auto d = to_light_v.l2norm();

            // Computing the shadow ray intersection with model
            if(m.shadow_intersect(to_light_r, d)){
                continue;
            }
            
            // Get the angle of the light source
            to_light_v = normalize(to_light_v);
            auto theta = dot(tnormal, to_light_v);
            
            // Make sure theta is non-negative
            if(theta < 0){
                theta = 0;
            }

            Rational lm = light_src->get_light_multiplier(to_light_v);

            // Compute the diffuse light
            auto ldr = lm * mat.diff_r * light_src->r * theta;
            auto ldg = lm * mat.diff_g * light_src->g * theta;
            auto ldb = lm * mat.diff_b * light_src->b * theta;

            // Compute vector to camera and the bisector
            auto v = normalize(this->eye - in.inter_p);
            auto h = normalize(v + to_light_v);

            auto alpha = dot(tnormal, h);

            // Check the negativity and apply phong
            if(alpha > 0){
                alpha = pow(alpha, mat.phong);
            }else{
                alpha = 0;
            }

            // Compute the spec coefficient
            auto lsr = lm * mat.spec_r * light_src->r * alpha;
            auto lsg = lm * mat.spec_g * light_src->g * alpha;
            auto lsb = lm * mat.spec_b * light_src->b * alpha;

            //TODO: Ambient light
            //TODO: Refractions
            //TODO: Reflections

            // Compute total light on the pixel
            Rational dsq = d*d;
            rr += (lsr + ldr) / dsq;
            gg += (lsg + ldg) / dsq;
            bb += (lsb + ldb) / dsq;

            //std::cerr << rr << gg << bb << std::endl;
        }

        r += (rr / sample_divisor);
        g += (gg / sample_divisor);
        b += (bb / sample_divisor);
    }

    if (depth == 0){
        return Vector3d(r,g,b);
    }

    // Now we need to compute the energy from the reflected light
    if (mat.ideal_r + mat.ideal_g + mat.ideal_b  == 0){
        return Vector3d(r, g, b);
    }

    // Compute the reflected ray
    auto ref_ray_dir = ray.dir - 2*dot(ray.dir, in.normal)*in.normal;
    auto ref_ray = Ray(in.inter_p + 0.0001 * ref_ray_dir, ref_ray_dir);

    // Get the rgb energy from the reflected ray
    auto ref_rgb = this->compute_radiance(m, ref_ray, depth - 1);
    
    r += mat.ideal_r * ref_rgb.x;
    g += mat.ideal_g * ref_rgb.y;
    b += mat.ideal_b * ref_rgb.z;

    return Vector3d(r,g,b);
}


void Camera::create_frame(Point3d e, Vector3d dir){
    

    // TODO: Make sure we do not extra normalie
    dir = normalize(dir);
    
    // If the camera is pointing staright up
    if(dot(dir, Vector3d(0,1,0)) == 1){
        this->u = Vector3d(-1, 0, 0);
    // If the camera is pointing straight down
    }else if(dot(dir, Vector3d(0,1,0)) == -1){
        this->u = Vector3d(1, 0, 0);
    }else{
        this->u = normalize(cross(dir, Vector3d(0,1,0)));
    }

    this->v = normalize(cross(u, dir));
    this->w = normalize(-dir);
}

Camera::Camera(Point3d e, Vector3d dir, Rational focal, 
    Rational width, Rational height,
    unsigned int pixel_width, unsigned int pixel_height){

        // Build the frame
        this->create_frame(e, dir);
        this->eye = e;

        // Pixel widths
        this->nx = pixel_width;
        this->ny = pixel_height;

        // Left and right corner
        this->l = -width / 2;
        this->r = width / 2;

        // Bottom and top corner
        this->t = height / 2;
        this->b = -height / 2;

        // Focal length
        this->d = focal;

        this->ref_depth = DEFAULT_REFLECTION_DEPTH;
}

std::vector<Ray> Camera::mc_ray_through_pixel(unsigned int i, unsigned int j, unsigned int n){

    std::vector<Ray> rays;
    for(unsigned int p = 0; p < n; p++){
        for(unsigned int q = 0; q < n; q++){
            auto uscalar = (this->l + (this->r - this->l)*(i + (p + random_number())/n )/this->nx);
            auto vscalar = (this->b + (this->t - this->b)*(j + (q + random_number())/n)/this->ny);
            auto dir = -this->d*this->w + uscalar*this->u + vscalar*this->v;
            rays.push_back(Ray(this->eye, dir));
        }
    }

    return rays;
    
}

Ray Camera::ray_through_pixel(unsigned int i, unsigned int j){
    auto dir = -this->d*this->w + get_u_scalar(i)*this->u + get_v_scalar(j)*this->v;
    return Ray(this->eye, normalize(dir));
}

Rational Camera::get_u_scalar(unsigned int i){
    return (this->l + (this->r - this->l)*(i + 0.5)/this->nx);
}

Rational Camera::get_v_scalar(unsigned int j){
    return (this->b + (this->t - this->b)*(j + 0.5)/this->ny);
}

std::ostream& operator<<(std::ostream &os, const Camera& cam){
    return std::cout << "eye: " << cam.eye << std::endl
    << "u: " << cam.u << std::endl
    << "v: " << cam.v << std::endl
    << "w: " << cam.w << std::endl
    << "left: " << cam.l << "\t right: " << cam.r << std::endl
    << "bottom: " << cam.b << "\t top: " << cam.t << std::endl
    << "focal: " << cam.d << std::endl;
}




