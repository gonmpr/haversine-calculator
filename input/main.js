const fs = require("fs");

const count = Number(process.argv[2]);
let range = process.argv[3] === undefined ? 100: Number(process.argv[3]);
const mode = process.argv[4] === "cluster" ? "cluster": "global";


if (range < 0) range *= -1;
if (range > 100) range %= 100;


if (!Number.isInteger(count) || count <= 0 || process.argv == 1) {
  console.error("Uso: node generate.js <cantidad> <rango> <modo>");
  console.error("RANGO:\n\tdesviacion posible <0-100>");
  console.error("MODOS:\n\tcluster: areas locales\n\tglobal: todo el globo");
  process.exit(1);
}

if (!Number.isInteger(range)) {
  console.error("<rango> debe ser un numero");
  process.exit(1);
  }


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
  const maxClusterVariation = 10; //grades
  const variation = maxClusterVariation * (rng / 100);
  return center + (Math.random() * 2 - 1) * variation;
}

function randRange(min, max, rng) {
  max = max - (max/100 * (100 - rng));
  min = min - (min/100 * (100 - rng));
  return min + Math.random() * (max - min);
}

function randomClusterIndex() {
  return Math.floor(Math.random() * clusterLatitudes.length);
}

const pairs = [];


for (let i = 0; i < count; i++) {
  let x0, y0, x1, y1;

  if(mode == "cluster"){
    let rand = randomClusterIndex();
    x0 = randAround(clusterLongitudes[rand], range); // lon 1
    y0 = randAround(clusterLatitudes[rand],range);  // lat 1
    x1 = randAround(clusterLongitudes[rand], range); // lon 1
    y1 = randAround(clusterLatitudes[rand],range);   // lat 1
  }
  else{
    x0 = randRange(-180, 180, range); // lon 1
    y0 = randRange(-90, 90, range);   // lat 1
    x1 = randRange(-180, 180, range); // lon 2
    y1 = randRange(-90, 90, range);   // lat 2
  }

  pairs.push({
    x0, y0,
    x1, y1,
  });
}

const data = { pairs };

fs.writeFileSync("input.json", JSON.stringify(data, null, 2));

console.log(`Generados ${count} pares en input.json`);
