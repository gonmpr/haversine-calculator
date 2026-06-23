const EARTH_RADIUS = 6372.8;

function square(x) {
  return x * x;
}

function degToRad(degrees) {
  return degrees * Math.PI / 180;
}

function haversine(x0, y0, x1, y1) {
  const lon0 = degToRad(x0);
  const lat0 = degToRad(y0);
  const lon1 = degToRad(x1);
  const lat1 = degToRad(y1);

  const dLon = lon1 - lon0;
  const dLat = lat1 - lat0;

  const a =
    square(Math.sin(dLat / 2)) +
    Math.cos(lat0) * Math.cos(lat1) * square(Math.sin(dLon / 2));

  const c = 2 * Math.asin(Math.sqrt(a));

  return EARTH_RADIUS * c;
}

module.exports = {
  haversine,
};
