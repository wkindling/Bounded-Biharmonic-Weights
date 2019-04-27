#include <igl/jet.h>
#include <igl/copyleft/tetgen/mesh_with_skeleton.h>
#include <igl/faces_first.h>
#include <igl/boundary_conditions.h>
#include <igl/bbw.h>

#include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Dense>
#include <iostream>
#include <string>

#include "readfile.hpp"
#include "writefile.hpp"

Eigen::MatrixXd W;
igl::opengl::glfw::Viewer viewer;

const Eigen::RowVector3d sea_green(70. / 255., 252. / 255., 167. / 255.);
int selected = 0;

void set_color(igl::opengl::glfw::Viewer &viewer)
{
	Eigen::MatrixXd C;
	igl::jet(W.col(selected).eval(), true, C);
	viewer.data().set_colors(C);
}

bool key_down(igl::opengl::glfw::Viewer &viewer, unsigned char key, int mods)
{
	switch (key)
	{
	case '.':
		selected++;
		selected = std::min(std::max(selected, 0), (int)W.cols() - 1);
		set_color(viewer);
		break;
	case ',':
		selected--;
		selected = std::min(std::max(selected, 0), (int)W.cols() - 1);
		set_color(viewer);
		break;
	}
	return true;
}

int main()
{
	using namespace std;
	using namespace Eigen;
	using namespace igl;

	string mesh_name = "C:/Users/dell/Desktop/singlerod/singlerod.mesh";
	string tgf_name = "C:/Users/dell/Desktop/singlerod/singlerod.tgf";
	
	// #V by 3 list of mesh vertex positions
	MatrixXd V;
	// #F by 3 list of triangle indices
	MatrixXi F;
	// load mesh from .obj, .off or .mesh
	if (!load_mesh_from_file(mesh_name, V, F))
	{
		cout << "Read Mesh Failed" << endl;
		return 1;
	}
	// "Skeleton" (handles) descriptors:
	// List of control and joint (bone endpoint) positions
	MatrixXd C;
	// List of Bone Edges.
	MatrixXi E;
	// List of point handles indexing C
	VectorXi P;
	// List of bone edges indexing C
	MatrixXi BE;
	// List of cage edges indexing *P*
	MatrixXi CE;
	// load skeleton (.tgf or .bf)

	if (!load_skeleton_from_file(tgf_name, C, E, P, BE, CE))
	{
		cout << "Read Bone Failed!" << endl;
		return 1;
	}

	// Mesh with samples on skeleton
	// New vertices of tet mesh, V prefaces VV
	MatrixXd VV;
	// Tetrahedra
	MatrixXi TT;
	// New surface faces FF
	MatrixXi FF;

	if (!igl::copyleft::tetgen::mesh_with_skeleton(V, F, C, P, BE, CE, 10, VV, TT, FF))
	{
		cout << "Tetgen Failed!" << endl;
		return 1;
	}

	// Compute boundary conditions (aka fixed value constraints)
	// List of boundary indices (aka fixed value indices into VV)
	VectorXi b;
	// List of boundary conditions of each weight function
	MatrixXd bc;
	if (!boundary_conditions(VV, TT, C, P, BE, CE, b, bc))
	{
		return 1;
	}

	// compute BBW 
	// Default bbw data and flags
	BBWData bbw_data;
	// Weights matrix

	if (!bbw(VV, TT, b, bc, bbw_data, W))
	{
		return 1;
	}

	// Save output
	ofstream out("C:/Users/dell/Desktop/model.txt");
	write_file(out, V, F, C, E, W);
	cout << "Export Information Success!" << endl;

	viewer.data().set_mesh(VV, FF);
	set_color(viewer);
	viewer.data().set_edges(C, E, sea_green);
	viewer.data().show_lines = false;
	viewer.data().show_overlay_depth = false;
	viewer.data().line_width = 1;
	viewer.callback_key_down = &key_down;
	viewer.core.is_animating = false;
	viewer.core.animation_max_fps = 30.;
	cout <<
		"Press '.' to show next weight function." << endl <<
		"Press ',' to show previous weight function." << endl;
	viewer.launch();
	return EXIT_SUCCESS;
}
