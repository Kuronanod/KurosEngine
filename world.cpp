#include <vector>
#include "world.h"

using namespace std;

unordered_map<chunk_position , Chunk , pair_hash> world_chunk_map;
mutex world_map_mutex;