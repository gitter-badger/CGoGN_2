/*******************************************************************************
* CGoGN: Combinatorial and Geometric modeling with Generic N-dimensional Maps  *
* Copyright (C) 2015, IGG Group, ICube, University of Strasbourg, France       *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Web site: http://cgogn.unistra.fr/                                           *
* Contact information: cgogn@unistra.fr                                        *
*                                                                              *
*******************************************************************************/

#ifndef CORE_BASIC_CELL_MARKER_H_
#define CORE_BASIC_CELL_MARKER_H_

#include <core/container/chunk_array.h>
#include <core/cmap/map_base_data.h>
#include <type_traits>

namespace cgogn
{

//class CGOGN_CORE_API CellMarkerGen
//{
//public:
//	typedef CellMarkerGen Self;
//	CellMarkerGen()
//	{}

//	virtual ~CellMarkerGen();

//	CellMarkerGen(const Self& dm) = delete;
//	CellMarkerGen(Self&& dm) = delete;
//	CellMarkerGen& operator=(Self&& dm) = delete;
//	CellMarkerGen& operator=(const Self& dm) = delete;
//};

template <typename MAP, Orbit ORBIT>
class CellMarker_T // : public CellMarkerGen
{
	static_assert(ORBIT < NB_ORBITS, "Unknown orbit parameter");

public:
	static const unsigned int CHUNKSIZE = MAP::CHUNKSIZE;
	using Self = CellMarker_T<MAP, ORBIT>;
	using Map = MAP;
	using ChunkArrayBool = ChunkArray<CHUNKSIZE, bool>;

protected:

	MAP& map_;
	ChunkArrayBool* mark_attribute_;

public:

	CellMarker_T(Map& map) :
//		Inherit(),
		map_(map)
	{
		mark_attribute_ = map_.template get_mark_attribute<ORBIT>();
	}

	CellMarker_T(const MAP& map) :
//		Inherit(),
		map_(const_cast<MAP&>(map))
	{
		mark_attribute_ = map_.template get_mark_attribute<ORBIT>();
	}

	virtual ~CellMarker_T() // override
	{
		if (MapGen::is_alive(&map_))
			map_.template release_mark_attribute<ORBIT>(mark_attribute_);
	}

	CellMarker_T(const Self& dm) = delete;
	CellMarker_T(Self&& dm) = delete;
	Self& operator=(const Self& dm) = delete;
	Self& operator=(Self&& dm) = delete;

	inline void mark(Cell<ORBIT> c)
	{
		cgogn_message_assert(mark_attribute_ != nullptr, "CellMarker has null mark attribute");
		mark_attribute_->set_true(map_.get_embedding(c));
	}

	inline void unmark(Cell<ORBIT> c)
	{
		cgogn_message_assert(mark_attribute_ != nullptr, "CellMarker has null mark attribute");
		mark_attribute_->set_false(map_.get_embedding(c));
	}

	inline bool is_marked(Cell<ORBIT> c) const
	{
		cgogn_message_assert(mark_attribute_ != nullptr, "CellMarker has null mark attribute");
		return (*mark_attribute_)[map_.get_embedding(c)];
	}
};

template <typename MAP, Orbit ORBIT>
class CellMarker : public CellMarker_T<MAP, ORBIT>
{
public:

	typedef CellMarker_T<MAP, ORBIT> Inherit;
	typedef CellMarker< MAP, ORBIT > Self;
	typedef typename Inherit::Map Map;

	CellMarker(Map& map) :
		Inherit(map)
	{}

	CellMarker(const MAP& map) :
		Inherit(map)
	{}

	~CellMarker() override
	{
		unmark_all();
	}

	CellMarker(const Self& dm) = delete;
	CellMarker(Self&& dm) = delete;
	CellMarker<MAP, ORBIT>& operator=(Self&& dm) = delete;
	CellMarker<MAP, ORBIT>& operator=(const Self& dm) = delete;

	inline void unmark_all()
	{
		cgogn_message_assert(this->mark_attribute_ != nullptr, "CellMarker has null mark attribute");
		this->mark_attribute_->all_false();
	}
};

template <typename MAP, Orbit ORBIT>
class CellMarkerStore : public CellMarker_T<MAP, ORBIT>
{
public:

	typedef CellMarker_T<MAP, ORBIT> Inherit;
	typedef CellMarkerStore< MAP, ORBIT > Self;

	typedef typename Inherit::Map Map;

protected:

	std::vector<unsigned int>* marked_cells_;

public:

	CellMarkerStore(const MAP& map) :
		Inherit(map)
	{
		marked_cells_ = cgogn::get_uint_buffers()->get_buffer();
	}

	~CellMarkerStore() override
	{
		unmark_all();
		cgogn::get_uint_buffers()->release_buffer(marked_cells_);
	}

	CellMarkerStore(const Self& dm) = delete;
	CellMarkerStore(Self&& dm) = delete;
	CellMarkerStore<MAP, ORBIT>& operator=(Self&& dm) = delete;
	CellMarkerStore<MAP, ORBIT>& operator=(const Self& dm) = delete;

	inline void mark(Cell<ORBIT> c)
	{
		cgogn_message_assert(this->mark_attribute_ != nullptr, "CellMarkerStore has null mark attribute");
		Inherit::mark(c);
		marked_cells_->push_back(this->map_.get_embedding(c));
	}

	inline void unmark_all()
	{
		cgogn_message_assert(this->mark_attribute_ != nullptr, "CellMarkerStore has null mark attribute");
		for (unsigned int i : *(this->marked_cells_))
			this->mark_attribute_->set_false(i);
		marked_cells_->clear();
	}

	inline const std::vector<unsigned int>* get_marked_cells() const
	{
		return marked_cells_;
	}
};

} // namespace cgogn

#endif // CORE_BASIC_CELL_MARKER_H_
