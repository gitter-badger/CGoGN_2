if(CGOGN_PROVIDE_EIGEN)
	add_subdirectory(eigen-3.2.7)
endif(CGOGN_PROVIDE_EIGEN)

if(CGOGN_PROVIDE_TINYXML2)
	add_subdirectory(TinyXml2)
endif(CGOGN_PROVIDE_TINYXML2)

if(CGOGN_BUILD_TESTS)
	add_subdirectory(googletest-master)
endif(CGOGN_BUILD_TESTS)

if(CGOGN_USE_QT)
	add_subdirectory(libQGLViewer/QOGLViewer)
endif(CGOGN_USE_QT)
