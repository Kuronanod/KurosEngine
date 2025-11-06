#include "chunk_worker.h"
#include "world_generate.h"
#include <iostream>

using namespace std;

struct Chunkpositionhash {
    size_t operator()(const chunk_position& p) const{
        size_t h1 = hash<int>()(p.x);
        size_t h2 = hash<int>()(p.y);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1<<6) + (h1>>2));
    }
};

static deque<chunk_position> job_queue;
static unordered_set<chunk_position,pair_hash> pending_jobs;
static mutex job_mutex;
static condition_variable job_cv;
static vector<thread> worker;
static atomic<bool> worker_running(false);

extern unordered_map<chunk_position,Chunk,pair_hash> world_chunk_map;
extern mutex world_map_mutex;

static void worker_thread_func(){
    while(worker_running.load()){
        chunk_position job;
        {
    
            unique_lock <mutex> lk(job_mutex);
            job_cv.wait(lk, []{return !job_queue.empty() || !worker_running.load();});
            if(!worker_running.load() && job_queue.empty()) break;
            job = job_queue.front();
            job_queue.pop_front();
        }

        Chunk generate = generate_chunk(job.x,job.y);

        {
            lock_guard<mutex> lk(world_map_mutex);
            if(world_chunk_map.find(job) == world_chunk_map.end()){
                world_chunk_map[job] = move(generate);
            }
        }

        {
            lock_guard<mutex> lk(job_mutex);
            pending_jobs.erase(job);
        }

    }
}

void start_chunk_worker(int num_thread){
    if(worker_running.load()) return;
    worker_running.store(true);
    worker.reserve(num_thread);
    for (int i = 0 ; i < num_thread ; ++i){
        worker.emplace_back(worker_thread_func);
    }
}

void stop_chunk_worker(){
    if (worker_running.load()) return;
    worker_running.store(false);
    job_cv.notify_all();
    for(auto& t : worker){
        if (t.joinable()) t.join();
    }

    worker.clear();

    {
        lock_guard<mutex> lk(job_mutex);
        job_queue.clear();
        pending_jobs.clear();
    }
}

void enqueue_chunk_job(int chunk_position_x , int chunk_position_y){
    chunk_position key{chunk_position_x,chunk_position_y};

    {
        lock_guard<mutex> lk(world_map_mutex);
        if (world_chunk_map.find(key) != world_chunk_map.end()) return;
    }

    {
        lock_guard<mutex> lk(job_mutex);
        if (pending_jobs.find(key) != pending_jobs.end()) return;
        pending_jobs.insert(key);
        job_queue.push_back(key);
    }

    job_cv.notify_all();

}