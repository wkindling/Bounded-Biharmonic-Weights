#pragma once
#ifndef READFILE_HPP
#define READFILE_HPP

#include <igl/pathinfo.h>
#include <igl/readOBJ.h>
#include <igl/readOFF.h>
#include <igl/readMESH.h>
#include <igl/readTGF.h>
#include <Eigen/Dense>


// Read a surface mesh from a {.obj|.off|.mesh} files
// Inputs:
//   mesh_filename  path to {.obj|.off|.mesh} file
// Outputs:
//   V  #V by 3 list of mesh vertex positions
//   F  #F by 3 list of triangle indices
// Returns true only if successfuly able to read file
bool load_mesh_from_file(
	const std::string mesh_filename,
	Eigen::MatrixXd & V,
	Eigen::MatrixXi & F)
{
	using namespace std;
	using namespace igl;
	using namespace Eigen;
	string dirname, basename, extension, filename;
	pathinfo(mesh_filename, dirname, basename, extension, filename);
	transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	bool success = false;
	if (extension == "obj")
	{
		success = readOBJ(mesh_filename, V, F);
	}
	else if (extension == "off")
	{
		success = readOFF(mesh_filename, V, F);
	}
	else if (extension == "mesh")
	{
		// Unused Tets read from .mesh file
		MatrixXi Tets;
		success = readMESH(mesh_filename, V, Tets, F);
		// We're not going to use any input tets. Only the surface
		if (Tets.size() > 0 && F.size() == 0)
		{
			// If Tets read, but no faces then use surface of tet volume
		}
		else
		{
			// Rearrange vertices so that faces come first
			VectorXi IM;
			faces_first(V, F, IM);
			// Dont' bother reordering Tets, but this is how one would:
			//Tets = 
			//  Tets.unaryExpr(bind1st(mem_fun( static_cast<VectorXi::Scalar&
			//  (VectorXi::*)(VectorXi::Index)>(&VectorXi::operator())),
			//  &IM)).eval();
			// Don't throw away any interior vertices, since user may want weights
			// there
		}
	}
	else
	{
		cerr << "Error: Unknown shape file format extension: ." << extension << endl;
		return false;
	}
	return success;
}

// Load a skeleton (bones, points and cage edges) from a {.bf|.tgf} file
//
// Inputs:
//   skel_filename  path to skeleton {.bf|.tgf} file
// Outputs:
//  C  # vertices by 3 list of vertex positions
//  P  # point-handles list of point handle indices
//  BE # bone-edges by 2 list of bone-edge indices
//  CE # cage-edges by 2 list of cage-edge indices
bool load_skeleton_from_file(
	const std::string skel_filename,
	Eigen::MatrixXd & C,
	Eigen::MatrixXi &E,
	Eigen::VectorXi & P,
	Eigen::MatrixXi & BE,
	Eigen::MatrixXi & CE)
{
	using namespace std;
	using namespace igl;
	using namespace Eigen;
	string dirname, basename, extension, filename;
	pathinfo(skel_filename, dirname, basename, extension, filename);
	transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	bool success = false;
	if (extension == "tgf")
	{
		// Phony space for unused all edges and pseudo edges
		MatrixXi PE;
		success = readTGF(skel_filename, C, E, P, BE, CE, PE);
	}
	else
	{
		cerr << "Error: Unknown skeleton file format extension: ." << extension << endl;
		return false;
	}
	return success;
}






#endif