@echo g++ cho s ebalom? Needs something mgw libs
g++ -v files/*.cpp graphics/*.cpp lighting/*.cpp loaders/*.cpp objects/*.cpp physics/*.cpp voxels/*.cpp window/*.cpp *.cpp -o voxel_engine -lopengl32 -lspng -lglew32 -lglu32 -lglfw3 -luser32 -lkernel32 -lgdi32
voxel_engine.exe
@pause