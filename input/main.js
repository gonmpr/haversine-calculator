const fs = require("fs");
const count = Number(process.argv[2]);

if (!Number.isInteger(count) || count <= 0) {
  console.error("Uso: node generate.js <cantidad>");
  console.error("Ejemplo: node generate.js 10000");
  process.exit(1);
}

const pairs = [];

function randRange(min, max) {
  return min + Math.random() * (max - min);
}

for (let i = 0; i < count; i++) {
  pairs.push({
    x0: randRange(-180, 180), // longitud 1
    y0: randRange(-90, 90),   // latitud 1
    x1: randRange(-180, 180), // longitud 2
    y1: randRange(-90, 90)    // latitud 2
  });
}

const data = { pairs };

fs.writeFileSync("input.json", JSON.stringify(data, null, 2));

console.log(`Generated ${count} pairs in input.json`);
