#include "reference_haversine.h"

#define EARTH_RADIUS 6372.8
#define PI 3.14159265358979323846

static double square(double x){
  return x*x;
}

static double deg_to_rad(double deg){
  double rad = deg * PI / 180.0;
  return rad;
}

double reference_haversine(pair_t pairs){

  double lon0 = deg_to_rad(pairs.x0);
  double lat0 = deg_to_rad(pairs.y0);
  double lon1 = deg_to_rad(pairs.x1);
  double lat1 = deg_to_rad(pairs.y1);

  double diff_lon = lon1 - lon0;
  double diff_lat = lat1 - lat0;


  double a =
    square(sin(diff_lat / 2)) + cos(lat0) * cos(lat1) * square(sin(diff_lon / 2));

  double c = 2 * asin(sqrt(a));

  return EARTH_RADIUS * c;
}
