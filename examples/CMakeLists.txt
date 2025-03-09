## sp code
add_subdirectory(sp)

## cv code
if(SP_USE_OPENCV)
	add_subdirectory(cv)
endif(SP_USE_OPENCV)

## gl code
if(SP_USE_OPENGL)
	add_subdirectory(gl)
endif(SP_USE_OPENGL)
