#define DEBUG

#ifdef DEBUG
#include "./timeMeasure.h"

#define TIME(x) n_time x = get_current_time()
#define TIME_DIFF(st,end)  t_diff(st, end)


#ifdef FPS_CAP
#define FPS 60.0f
#define FRAME_LIMIT 1.0f/FPS
#endif  // FPS_CAP

#include <assert.h>

#define ASSERT(condition)   assert(condition)

#include <stdio.h>
#else
#define TIME(x) 
#define TIME_DIFF(st,end)  0

#define ASSERT(condition)   

#endif //DEBUG