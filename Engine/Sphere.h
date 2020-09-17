#pragma once

#include "IndexedTriangleList.h"
#include "Vec3.h"
#include "Mat3.h"
#include "ChiliMath.h"

class Sphere
{
public:
	template<class V>
	static IndexedTriangleList<V> MakePlain( float radius,int nLat,int nLon )
	{
		IndexedTriangleList<V> itlist;

		Vec3 base = { 0.0f,0.0f,radius };
		const float dThetaLat = PI / (float)nLat;
		const float dThetaLon = 2.0f * PI / (float)nLon;

		for ( int iLat = 1; iLat < nLat; ++iLat )
		{
			const Vec3 latVert = base * Mat3::RotationX( iLat * dThetaLat );
			for ( int iLon = 0; iLon < nLon; ++iLon )
			{
				itlist.vertices.push_back( latVert * Mat3::RotationZ( iLon * dThetaLon ) );
			}
		}

		// add poles
		const auto northPoleIdx = itlist.vertices.size();
		itlist.vertices.push_back( base );
		const auto southPoleIdx = itlist.vertices.size();
		itlist.vertices.push_back( -base );

		const auto getIdx = [&nLon]( int iLat,int iLon ) -> size_t
		{
			return (size_t)( iLat - 1 ) * nLon + iLon;
		};
		for ( int iLat = 1; iLat < nLat - 1; ++iLat )
		{
			for ( int iLon = 0; iLon < nLon - 1; ++iLon )
			{
				itlist.indices.emplace_back( getIdx( iLat,iLon ) );
				itlist.indices.emplace_back( getIdx( iLat + 1,iLon ) );
				itlist.indices.emplace_back( getIdx( iLat,iLon + 1 ) );
				itlist.indices.emplace_back( getIdx( iLat,iLon + 1 ) );
				itlist.indices.emplace_back( getIdx( iLat + 1,iLon ) );
				itlist.indices.emplace_back( getIdx( iLat + 1,iLon + 1 ) );
			}

			// close the sphere
			itlist.indices.emplace_back( getIdx( iLat,nLon - 1 ) );
			itlist.indices.emplace_back( getIdx( iLat + 1,nLon - 1 ) );
			itlist.indices.emplace_back( getIdx( iLat,0 ) );
			itlist.indices.emplace_back( getIdx( iLat,0 ) );
			itlist.indices.emplace_back( getIdx( iLat + 1,nLon - 1 ) );
			itlist.indices.emplace_back( getIdx( iLat + 1,0 ) );
		}

		// cap the sphere
		for ( int iLon = 0; iLon < nLon - 1; ++iLon )
		{
			itlist.indices.emplace_back( northPoleIdx );
			itlist.indices.emplace_back( getIdx( 1,iLon ) );
			itlist.indices.emplace_back( getIdx( 1,iLon + 1 ) );

			itlist.indices.emplace_back( getIdx( nLat - 1,iLon ) );
			itlist.indices.emplace_back( southPoleIdx );
			itlist.indices.emplace_back( getIdx( nLat - 1,iLon + 1 ) );
		}
		itlist.indices.emplace_back( northPoleIdx );
		itlist.indices.emplace_back( getIdx( 1,nLon - 1 ) );
		itlist.indices.emplace_back( getIdx( 1,0 ) );

		itlist.indices.emplace_back( getIdx( nLat - 1,nLon - 1 ) );
		itlist.indices.emplace_back( southPoleIdx );
		itlist.indices.emplace_back( getIdx( nLat - 1,0 ) );

		return std::move( itlist );
	}
};