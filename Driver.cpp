#include <cstring>
#include <iostream>

#include "map_info.h"
#include "query_exec.h"
#include "job_scheduler.h"

memory_map rel_mmap[14];

pthread_cond_t JobScheduler::cvar = PTHREAD_COND_INITIALIZER;
pthread_mutex_t JobScheduler::qmtx = PTHREAD_MUTEX_INITIALIZER;
simple_queue<Job*> JobScheduler::job_pool;

JobScheduler js;

int main(int argc, char* argv[]) {
  int64_t relations_count = 0;

  char line[4096];
  while (fgets(line, sizeof(line), stdin)) {
    line[strcspn(line, "\n")] = '\0';
    if (strcmp(line, "Done") == 0) break;

    rel_mmap[relations_count++] = parse_relation(line);
  }

  QueryExec qe;
  while (fgets(line, sizeof(line), stdin)) {
    line[strcspn(line, "\n")] = '\0';
    if (strcmp(line, "F") == 0) {  // End of a batch
      fflush(stdout);
      continue;
    }
    qe.execute(line);
  }

  return 0;
}