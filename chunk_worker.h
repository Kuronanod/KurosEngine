#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <unordered_set>
#include <atomic>
#include "world.h"

void start_chunk_worker(int num_thread = 2);

void stop_chunk_worker();

void enqueue_chunk_job(int chunk_position_x , int chunk_position_y);