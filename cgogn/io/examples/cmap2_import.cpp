
#include <chrono>
#include <ctime>
#include <vector>

#include <core/cmap/cmap2.h>
#include <core/cmap/sanity_check.h>
#include <io/map_import.h>
#include <geometry/algos/normal.h>


#define DEFAULT_MESH_PATH CGOGN_STR(CGOGN_TEST_MESHES_PATH)

struct MyMapTraits : public cgogn::DefaultMapTraits
{
	static const unsigned int CHUNK_SIZE = 8192;
};

using Map2 = cgogn::CMap2<MyMapTraits>;

using Vec3 = Eigen::Vector3d;

template <typename T>
using VertexAttributeHandler = Map2::VertexAttributeHandler<T>;
template <typename T>
using FaceAttributeHandler = Map2::FaceAttributeHandler<T>;

int main(int argc, char** argv)
{
	std::string surfaceMesh;
	if (argc < 2)
	{
		std::cout << "USAGE: " << argv[0] << " [filename]" << std::endl;
		surfaceMesh = std::string(DEFAULT_MESH_PATH) + std::string("aneurysm3D_1.off");
		std::cout << "Using default mesh : " << surfaceMesh << std::endl;
	}
	else
		surfaceMesh = std::string(argv[1]);

	Map2 map;

	for (unsigned int k = 0; k < 2; ++k)
	{
		cgogn::io::import_surface<Vec3>(map, surfaceMesh);

		unsigned int nb_darts = 0;
		map.foreach_dart([&nb_darts] (cgogn::Dart) { nb_darts++; });
		std::cout << "nb darts -> " << nb_darts << std::endl;

		unsigned int nb_darts_2 = 0;
		std::vector<unsigned int> nb_darts_per_thread(cgogn::NB_THREADS - 1);
		for (unsigned int& n : nb_darts_per_thread)
			n = 0;
		map.parallel_foreach_dart([&nb_darts_per_thread] (cgogn::Dart, unsigned int thread_index)
		{
			nb_darts_per_thread[thread_index]++;
		});
		for (unsigned int n : nb_darts_per_thread)
			nb_darts_2 += n;
		std::cout << "nb darts // -> " << nb_darts_2 << std::endl;


		VertexAttributeHandler<Vec3> vertex_position = map.get_attribute<Vec3, Map2::VERTEX>("position");
		VertexAttributeHandler<Vec3> vertex_normal = map.add_attribute<Vec3, Map2::VERTEX>("normal");
		FaceAttributeHandler<Vec3> face_normal = map.add_attribute<Vec3, Map2::FACE>("normal");

		map.enable_topo_cache<Map2::FACE>();
		map.enable_topo_cache<Map2::VERTEX>();
		map.enable_topo_cache<Map2::EDGE>();


		std::cout << "Vertex orbits are well embedded ? -> " << std::boolalpha << cgogn::is_well_embedded<Map2::VERTEX>(map) << std::endl;
		std::cout << "Face orbits are well embedded ? -> " << std::boolalpha << cgogn::is_well_embedded<Map2::FACE>(map) << std::endl;

		std::cout << "Vertex orbit is uniquely embedded ? -> " << std::boolalpha << cgogn::is_orbit_embedding_unique<Map2::VERTEX>(map) << std::endl;
		std::cout << "Face orbit is uniquely embedded ? -> " << std::boolalpha << cgogn::is_orbit_embedding_unique<Map2::FACE>(map) << std::endl;

		std::cout << "Vertex container is well referenced ? -> " << std::boolalpha << cgogn::is_container_well_referenced<Map2::VERTEX>(map) << std::endl;
		std::cout << "Face container is well referenced ? -> " << std::boolalpha << cgogn::is_container_well_referenced<Map2::FACE>(map) << std::endl;


		unsigned int nb_faces = 0;
		map.foreach_cell<Map2::FACE>([&nb_faces] (Map2::Face) { nb_faces++; });
		std::cout << "nb faces -> " << nb_faces << std::endl;

		unsigned int nb_faces_2 = 0;
		std::vector<unsigned int> nb_faces_per_thread(cgogn::NB_THREADS - 1);
		for (unsigned int& n : nb_faces_per_thread)
			n = 0;
		map.parallel_foreach_cell<Map2::FACE>([&nb_faces_per_thread] (Map2::Face, unsigned int thread_index)
		{
			nb_faces_per_thread[thread_index]++;
		});
		for (unsigned int n : nb_faces_per_thread)
			nb_faces_2 += n;
		std::cout << "nb faces // -> " << nb_faces_2 << std::endl;


		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();


		for	(unsigned int i = 0; i < 10; ++i)
		{
//			map.parallel_foreach_cell<Map2::FACE>([&] (Map2::Face f, unsigned int)
//			map.foreach_cell<Map2::FACE>([&] (Map2::Face f)
//			{
//				Vec3 v1 = vertex_position[map.phi1(f.dart)] - vertex_position[f.dart];
//				Vec3 v2 = vertex_position[map.phi_1(f.dart)] - vertex_position[f.dart];
//				Vec3 n = v1.cross(v2);
//				n.normalize();
//				face_normal[f] = v1.cross(v2);
//			});

			cgogn::geometry::template compute_normal_faces<Vec3>(map, vertex_position, face_normal);
		}

		for	(unsigned int i = 0; i < 10; ++i)
		{
//			map.parallel_foreach_cell<Map2::VERTEX>([&] (Map2::Vertex v, unsigned int)
//			map.foreach_cell<Map2::VERTEX>([&] (Map2::Vertex v)
//			{
//				Vec3 sum({0, 0, 0});
//				unsigned int nb_incident = 0;
//				map.foreach_incident_face(v, [&] (Map2::Face f)
//				{
//					++nb_incident;
//					sum += face_normal[f];
//				});
//				vertex_normal[v] = sum / nb_incident;
//			});

//			cgogn::geometry::template compute_normal_vertices<Vec3>(map, vertex_position, vertex_normal);
			cgogn::geometry::template compute_normal_vertices<Vec3>(map, vertex_position, face_normal, vertex_normal);
		}

		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	}

	return 0;
}
