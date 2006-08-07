// examples/Timer/Profile_counter.C

#define CGAL_PROFILE

#include <CGAL/Profile_counter.h>

int main()
{
  for (int i=0; i<10; ++i)
  {
    CGAL_PROFILER("iterations of the for-loop");
  }
  return 0;
}
