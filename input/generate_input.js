const clusterLatitudes = [
  -34.6037, // Buenos Aires
   40.7128, // New York
   48.8566, // Paris
   35.6895  // Tokyo
];

const clusterLongitudes = [
  -58.3816, // Buenos Aires
  -74.0060, // New York
    2.3522, // Paris
  139.6917  // Tokyo
];

function randAround(center, rng) {
  const maxClusterVariation = 10;
  const variation = maxClusterVariation * (rng / 100);
  return center + (Math.random() * 2 - 1) * variation;
}

function randRange(min, max, rng) {
  max = max - (max / 100 * (100 - rng));
  min = min - (min / 100 * (100 - rng));
  return min + Math.random() * (max - min);
}

function randomClusterIndex() {
  return Math.floor(Math.random() * clusterLatitudes.length);
}

function generatePairs(count, range, mode) {
  const pairs = [];

  for (let i = 0; i < count; i++) {
    let x0, y0, x1, y1;

    if (mode === "cluster") {
      const rand_x0 = randomClusterIndex();
      const rand_y0 = randomClusterIndex();
      const rand_x1 = randomClusterIndex();
      const rand_y1 = randomClusterIndex();

      x0 = randAround(clusterLongitudes[rand_x0], range);
      y0 = randAround(clusterLatitudes[rand_y0], range);
      x1 = randAround(clusterLongitudes[rand_x1], range);
      y1 = randAround(clusterLatitudes[rand_y1], range);
    } else {
      x0 = randRange(-180, 180, range);
      y0 = randRange(-90, 90, range);
      x1 = randRange(-180, 180, range);
      y1 = randRange(-90, 90, range);
    }

    pairs.push({ x0, y0, x1, y1 });
  }

  return pairs;
}

module.exports = {
  generatePairs,
};
