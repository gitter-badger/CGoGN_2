add_subdirectory(core)
add_subdirectory(io)
add_subdirectory(geometry)

if(CGOGN_USE_QT)
	add_subdirectory(rendering)
endif(CGOGN_USE_QT)

