#pragma once

#include <vector>
#include "Vec3.h"
#include <cassert>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include "tiny_obj_loader.h"

template<typename T>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList( std::vector<T> vertices,std::vector<size_t> indices )
		:
		vertices( std::move( vertices ) ),
		indices( std::move( indices ) )
	{
		assert( this->vertices.size() > 2 );
		assert( this->indices.size() % 3 == 0 );
	}
	static IndexedTriangleList<T> Load( const std::string& filename )
	{
		IndexedTriangleList<T> tl;

		// check first line of file to see if CCW winding comment exists
		bool isCCW = false;
		{
			std::ifstream file( filename );
			std::string firstline;
			std::getline( file,firstline );
			std::transform( firstline.begin(),firstline.end(),firstline.begin(),std::tolower );
			if ( firstline.find( "ccw" ) != std::string::npos )
			{
				isCCW = true;
			}
		}

		// these will be filled by obj loading function
		using namespace tinyobj;
		attrib_t attrib;
		std::vector<shape_t> shapes;
		std::string err;

		// load/parse the obj file
		const bool ret = LoadObj( &attrib,&shapes,nullptr,&err,filename.c_str() );

		// check for errors
		if ( !err.empty() && err.substr( 0,4 ) != "WARN" )
		{
			throw std::runtime_error( ( "LoadObj returned error:" + err + " File:" + filename ).c_str() );
		}
		if ( !ret )
		{
			throw std::runtime_error( ( "LoadObj returned false  File:" + filename ).c_str() );
		}
		if ( shapes.size() == 0u )
		{
			throw std::runtime_error( ( "LoadObj object file had no shapes  File:" + filename ).c_str() );
		}

		// extract vertex data
		// attrib.vertices is a flat std::vector of floats corresponding
		// to vertex positions, laid out as xyzxyzxyz... etc.
		// first preallocate required space in OUR std::vector
		tl.vertices.reserve( attrib.vertices.size() / 3u );
		// iterate over individual vertices, construct Vec3s in OUR vector
		for ( size_t i = 0; i < attrib.vertices.size(); i += 3 )
		{
			tl.vertices.emplace_back( Vec3{
				attrib.vertices[i + 0],
				attrib.vertices[i + 1],
				attrib.vertices[i + 2] 
			} );
		}

		// extract index data
		// obj file can contain multiple meshes, we assume just 1
		const auto& mesh = shapes[0].mesh;
		// mesh contains a std::vector of num_face_vertices (uchar)
		// and a flat std::vector of indices. If all faces are triangles
		// then for any face f, the first index of that faces is [f * 3n]
		tl.indices.reserve( mesh.indices.size() );
		for ( size_t f = 0; f < mesh.num_face_vertices.size(); f++ )
		{
			// make sure there are no non-triangle faces
			if ( mesh.num_face_vertices[f] != 3u )
			{
				std::stringstream ss;
				ss << "LoadObj error face #" << f << " has "
					<< mesh.num_face_vertices[f] << " vertices";
				throw std::runtime_error( ss.str().c_str() );
			}

			// load set of 3 indices for each face into OUR index std::vector
			for ( size_t vn = 0; vn < 3u; vn++ )
			{
				const auto idx = mesh.indices[f * 3u + vn];
				tl.indices.push_back( size_t( idx.vertex_index ) );
			}

			// reverse winding if file marked as CCW
			if ( isCCW )
			{
				// swapping any two indices reverse the winding dir of triangle
				std::swap( tl.indices.back(),*std::prev( tl.indices.end(),2 ) );
			}
		}

		return std::move( tl );
	}
	IndexedTriangleList& operator+=( const IndexedTriangleList& rhs )
	{
		const size_t nVerts = vertices.size();
		std::copy( rhs.vertices.begin(),rhs.vertices.end(),std::back_inserter( vertices ) );
		std::transform( rhs.indices.begin(),rhs.indices.end(),
						std::back_inserter( indices ),
						[&nVerts]( size_t i )
						{
							return i + nVerts;
						} );
		return *this;
	}
	IndexedTriangleList operator+( const IndexedTriangleList& rhs ) const
	{
		return IndexedTriangleList( *this ) += rhs;
	}

	std::vector<T> vertices;
	std::vector<size_t> indices;
};
