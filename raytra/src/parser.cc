
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <memory>
#include <string>

#include "parser.h"


#include "basic_math.h"
#include "sphere.h"
#include "camera.h"
#include "point_light.h"
#include "area_light.h"



using namespace std;

class MeshSupportStruct {
public:
    Triangle t;
    int p1normal_idx;
    int p2normal_idx;
    int p3normal_idx;
};


// In this code, you can see how Sphere and Camera are set up - you
// will need to do similar for the other classes.
//
// You will also need to keep track of the last material defined, so
// that you can connect it to any surfaces that are subsequently defined.
//
void Parser::parse(
    const char *file,
    Camera &cam,
    Model &model){
    
    ifstream in(file);
    char buffer[1025];
    string cmd;

    int mat_id = -1;
    
    int num_cams = 0; // keep track of how many cameras we read in
    
    for (int line=1; in.good(); line++) {
        in.getline(buffer,1024);
        buffer[in.gcount()]=0;
        

        cmd="";
        
        istringstream iss(buffer);
        
        iss >> cmd;
        
        if (cmd[0]=='/' or cmd.empty()) {
            // ignore comments or blank lines
            continue;
        } 
        else if (cmd=="s") {
            // Sphere:
            
            // read in the parameters:
            Point3d pos; 
            Rational r;
            iss >> pos >> r;
            
            model.add_sphere(Sphere(pos, r, mat_id));
        } 
        else if (cmd=="t") {
            // Triangle
            Point3d p1, p2, p3;
            iss >> p1 >> p2 >> p3;
            model.add_triangle(Triangle(p1, p2, p3, mat_id));
        }
        else if (cmd=="p") {
            // Plane
        }
        else if (cmd=="c") {
            // Camera:
            ++num_cams; // keep track of how many we read in
 
            // read in the parameters:
            Point3d pos; Vector3d dir; 
            Rational d,iw,ih; 
            int pw,ph;
            iss >> pos >> dir >> d >> iw >> ih >> pw >> ph;

            // construct it:
            cam = Camera(pos,dir,d,iw,ih,pw,ph);
        } 
        else if (cmd=="l") {
            string light_type;
            iss >> light_type;

            if(light_type == "p"){
                Point3d light_pos;
                Rational r, g, b;
                iss >> light_pos;
                iss >> r >> g >> b;
                auto lsp = new PointLight(r, g, b, light_pos);
                model.add_light(lsp);

            }else if (light_type == "s"){
                Point3d light_pos;
                Vector3d dir;
                Vector3d udir;
                Rational len;
                Rational r, g, b;

                iss >> light_pos;
                iss >> dir;
                iss >> udir;
                iss >> len;
                iss >> r >> g >> b;
                auto lsp = new AreaLight(light_pos, dir, udir, len, r, g, b);
                model.add_light(lsp);



            }else{
                std::cout << "Light type %s not supported" << light_type << endl;
            }

        }
        else if (cmd=="m") {
            Rational sr, sg, sb, r, dr, dg, db, ir, ig, ib;
            iss >> dr >> dg >> db >> sr >> sg >> sb >> r >> ir >> ig >> ib;
            model.add_material(Material(dr,dg,db,sr,sg,sb,r, ir,ig,ib));
            mat_id++;
        }
        else if (cmd=="w"){
            std::string file_name;
            iss >> file_name;

            std::cout << "Reading " << file_name << std::endl;
            
            std::vector< int > tris;
            std::vector< float > verts;

            auto ok = read_wavefront_file(file_name.c_str(), tris, verts);
            if (!ok){
                std::cerr << "Quiting the parser...\n";
                exit(1);
            } 

            // Build the triangles
            unsigned int i = 0;

            // The vector will accumulate the vertex normals
            std::vector<Vector3d> vertex_normals;
            vertex_normals.reserve(verts.size() / 3);

            std::vector<MeshSupportStruct> sup;

            Rational tri = tris.size()/3;
            std::cout << tri << std::endl;
            for(i = 0; i < tris.size()/3; i++){
                auto p1 = Point3d(verts[3*tris[3*i]], verts[3*tris[3*i]+1], verts[3*tris[3*i]+2]);
                auto p2 = Point3d(verts[3*tris[3*i+1]], verts[3*tris[3*i+1]+1], verts[3*tris[3*i+1]+2]);
                auto p3 = Point3d(verts[3*tris[3*i+2]], verts[3*tris[3*i+2]+1], verts[3*tris[3*i+2]+2]);
                auto t = Triangle(p1, p2, p3, mat_id);

                MeshSupportStruct s;
                s.t = t;
                s.p1normal_idx = tris[3*i];
                s.p2normal_idx = tris[3*i+1];
                s.p3normal_idx = tris[3*i+2];

                
                vertex_normals[s.p1normal_idx] = vertex_normals[s.p1normal_idx] + t.normal;
                vertex_normals[s.p2normal_idx] = vertex_normals[s.p2normal_idx] + t.normal;
                vertex_normals[s.p3normal_idx] = vertex_normals[s.p3normal_idx] + t.normal;

                sup.push_back(s);
            }

            std::cout << "Read successful: " << i << " t built" << std::endl;

            for (auto s : sup){
                
                // Add the triangle to the surface vector
                auto tr = s.t;
                tr.p1normal = normalize(vertex_normals[s.p1normal_idx]);

                //std::cerr << s.p1normal_idx << "  " << vertex_normals[s.p1normal_idx] << std::endl;
                tr.p2normal = normalize(vertex_normals[s.p2normal_idx]);
                //std::cerr << s.p2normal_idx << "  " << vertex_normals[s.p2normal_idx] << std::endl;
                tr.p3normal = normalize(vertex_normals[s.p3normal_idx]);
                //std::cerr << s.p3normal_idx << "  " << vertex_normals[s.p3normal_idx] << std::endl;
                
                model.add_triangle(tr);
                //std::cerr << "Traingle added\n";
            }

            
        }

        else {
            std::cerr << "Parser error: invalid command at line " << line << endl;
        }

    }
    
    in.close();
    
    // make sure we read in 1 camera, no more no less 8).
    if (num_cams != 1) {
        std::cerr << "scene file error: exactly ONE camera must be defined." << endl;
    }
}



// Given the name of a wavefront (OBJ) file that consists JUST of
// vertices, triangles, and comments, read it into the tris and verts
// vectors.
//
// tris is a vector of ints that is 3*n long, where n is the number of
// triangles. The ith triangle has vertex indexes 3*i, 3*i+1, and 3*i+2.
//
// The ith triangle has vertices:
//
//     verts[3*tris[3*i]], verts[3*tris[3*i]+1], verts[3*tris[3*i]+2],
//     verts[3*tris[3*i+1]], verts[3*tris[3*i+1]+1], verts[3*tris[3*i+1]+2],
//     verts[3*tris[3*i+2]], verts[3*tris[3*i+2]+1], verts[3*tris[3*i+2]+2]
//
// given in counterclockwise order with respect to the surface normal
//
// If you are using the supplied Parser class, you should probably make this
// a method on it: Parser::read_wavefront_file().
//
bool read_wavefront_file(
    const char *file,
    std::vector< int > &tris,
    std::vector< float > &verts)
{
    
    // clear out the tris and verts vectors:
    tris.clear ();
    verts.clear ();

    ifstream in(file);

    // If something went wrong
    if (in.fail()) {
        std::cerr << "file: " << file << "not found\n";
        return false;
    }

    char buffer[1025];
    string cmd;
    
    
    for (int line=1; in.good(); line++) {
        in.getline(buffer,1024);
        buffer[in.gcount()]=0;
                
        cmd="";
        
        istringstream iss (buffer);
        
        iss >> cmd;
        
        if (cmd[0]=='#' or cmd.empty()) {
            // ignore comments or blank lines
            continue;
        } 
        else if (cmd=="v") {
            // got a vertex:
            
            // read in the parameters:
            Rational pa, pb, pc;
            iss >> pa >> pb >> pc;
 
            verts.push_back (pa);
            verts.push_back (pb);
            verts.push_back (pc);
         } 
        else if (cmd=="f") {
            // got a face (triangle)
            
            // read in the parameters:
            int i, j, k;
            iss >> i >> j >> k;
            
            // vertex numbers in OBJ files start with 1, but in C++ array
            // indices start with 0, so we're shifting everything down by
            // 1
            tris.push_back (i-1);
            tris.push_back (j-1);
            tris.push_back (k-1);
        } 
        else {
            std::cerr << "Parser error: invalid command at line " << line << std::endl;
        }
        
     }
    
    in.close();
    return true;    
}